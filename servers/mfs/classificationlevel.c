#include "fs.h"
#include "inode.h"
#include "super.h"
#include <minix/vfsif.h>

/*===========================================================================*
 *				fs_getclasslevel				     *
 *===========================================================================*/
int fs_getclasslevel()
{
	register struct inode *rip;

	/* Temporarily open the file. */
	if( (rip = get_inode(fs_dev, (ino_t) fs_m_in.REQ_INODE_NUM)) == NULL)
		return(EINVAL);

	/* Update caller on classification level */
	fs_m_out.RES_CLASSLEVEL = rip->i_zone[9];
	
	put_inode(rip);
	return(OK);
}

/*===========================================================================*
 *				fs_setclasslevel				     *
 *===========================================================================*/
int fs_setclasslevel()
{
	register struct inode *rip;

	/* Temporarily open the file. */
	if( (rip = get_inode(fs_dev, (ino_t) fs_m_in.REQ_INODE_NUM)) == NULL)
		return(EINVAL);

	/* Update classification level */	
	rip->i_zone[9] = fs_m_in.REQ_CLASSLEVEL;
	rip->i_update |= CTIME;
	IN_MARKDIRTY(rip);

	/* Update caller on classification level */
	fs_m_out.RES_CLASSLEVEL = rip->i_zone[9];
	put_inode(rip);

	return(OK);
}