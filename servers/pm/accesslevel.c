#include "pm.h"
#include "mproc.h"
#include "param.h"

/*===========================================================================*
 *				do_getaccesslevel					     *
 *===========================================================================*/
int do_getaccesslevel(void)
{
	register struct mproc *rmp;
	
	//Reply with access level if PID exists, otherwise reply -1
	mp->mp_reply.access_level = ((rmp = find_proc(m_in.pid)) != NULL) ? rmp->mp_access_level : -1;
	
	return 0;
}

/*===========================================================================*
 *				do_setaccesslevel					     *
 *===========================================================================*/
int do_setaccesslevel(void)
{
	register struct mproc *rmp;
	int new_access_level;
	
	new_access_level = m_in.access_level;
	
	/* Pid or access level is invalid */
	if((rmp = find_proc(m_in.pid)) == NULL || new_access_level < 0)
	{
		mp->mp_reply.access_level = -1;
		return 0;
	}
	
	/* Valid writing scenarios */
	if(mp->mp_effuid == 0 ||																		/* caller has superuser priveleges */
	    (mp->mp_pid == rmp->mp_pid && rmp->mp_access_level >= new_access_level) || 					/* caller is lowering own access level */
		(mp->mp_access_level > rmp->mp_access_level && mp->mp_access_level >= new_access_level))	/* caller is changing level of a lower level process to be <= the lvl of the caller */
	{
		rmp->mp_access_level = new_access_level;
		
		mp->mp_reply.access_level = new_access_level;
		
		message empty;
		_taskcall(VFS_PROC_NR, 56, &empty);
		
		return 0;
	}
	
	/* Illegal write attempted */
	mp->mp_reply.access_level = -1;
	return 0;
}