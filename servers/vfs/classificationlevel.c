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
#include <stdio.h>

/*===========================================================================*
 *				do_getclasslevel					     *
 *===========================================================================*/
int do_getclasslevel(void)
{
	int r, rfd;
	int level;
	struct filp *flp;
	struct vnode *vp;
	FILE * file;
	
	//Open file
	if((file = fopen(job_m_in.REQ_FILENAME, "r")) == NULL) return(EINVAL);
	rfd = fileno(file);
	
	//Load inode
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
	
	//Open file
	if((file = fopen(job_m_in.REQ_FILENAME, "r")) == NULL) return(EINVAL);
	rfd = fileno(file);
	
	//Load vnode
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