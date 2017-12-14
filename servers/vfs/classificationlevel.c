#include "fs.h"
#include <sys/stat.h>
#include <unistd.h>
#include <minix/callnr.h>
#include "file.h"
#include "fproc.h"
#include "path.h"
#include "param.h"
#include <minix/vfsif.h>
#include "vnode.h"
#include "vmnt.h"

/*===========================================================================*
 *				do_getclasslevel					     *
 *===========================================================================*/
int do_getclasslevel(void)
{
	int r, rfd;
	int level;
	struct filp *flp;
	struct vnode *vp;
	
	//Load inode
	rfd = job_m_in.REQ_FD;
	if ((flp = get_filp(rfd, VNODE_READ)) == NULL) return(err_code);
	vp = flp->filp_vno;
	dup_vnode(vp);
	
	//Send request
	r = req_getclasslevel(vp->v_fs_e, vp->v_inode_nr, &level);
	
	//Load reply
	m_out.RES_CLASSLEVEL = (r == OK) ? level : -1;
	
	unlock_filp(flp);
	put_vnode(vp);
	return r;
}

/*===========================================================================*
 *				do_setclasslevel					     *
 *===========================================================================*/
int do_setclasslevel(void)
{
	int r, rfd;
	int level, rlevel;
	struct filp *flp;
	struct vnode *vp;
	
	//Load vnode
	rfd = job_m_in.REQ_FD;
	if ((flp = get_filp(rfd, VNODE_WRITE)) == NULL) return(err_code);
	vp = flp->filp_vno;
	dup_vnode(vp);
	
	//Send request
	rlevel = job_m_in.REQ_CLASSLEVEL;
	r = req_setclasslevel(vp->v_fs_e, vp->v_inode_nr, rlevel, &level);

	//Load reply
	m_out.RES_CLASSLEVEL = (r == OK) ? level : -1;

	unlock_filp(flp);
	put_vnode(vp);
	return r;
}