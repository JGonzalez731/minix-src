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
	int r;
	struct vnode *vp;
	struct vmnt *vmp;
	char fullpath[PATH_MAX];
	struct lookup resolve;
	vir_bytes vname;
	size_t vname_length;
	u16_t level;

	//Get vnode
	vname = (vir_bytes) job_m_in.name;
	vname_length = (size_t) job_m_in.name_length;

	lookup_init(&resolve, fullpath, PATH_NOFLAGS, &vmp, &vp);
	resolve.l_vmnt_lock = VMNT_READ;
	resolve.l_vnode_lock = VNODE_READ;

	if (fetch_name(vname, vname_length, fullpath) != OK) return(err_code);
	if ((vp = eat_path(&resolve, fp)) == NULL) return(err_code);
	
	//Enforce access model
	r = EPERM;
	level = -1;
	if(super_user ||																				//Super user
		((S_IRUSR & vp->v_mode) && (vp->v_uid == fp->fp_effuid)) ||									//User w/ read priveleges
		((S_IRGRP & vp->v_mode) && (vp->v_gid == fp->fp_effgid)) || 								//Group member w/ read priveleges
		((S_IROTH & vp->v_mode) && (vp->v_uid != fp->fp_effuid) && (vp->v_gid != fp->fp_effgid)))	//Other w/ read priveleges	
	{
		r = req_getclasslevel(vp->v_fs_e, vp->v_inode_nr, &level);
	}
	
	//Load reply
	m_out.RES_CLASSLEVEL = (r == OK) ? level : -1;
	
	unlock_vnode(vp);
	unlock_vmnt(vmp);

	put_vnode(vp);
	return r;
}

/*===========================================================================*
 *				do_setclasslevel					     *
 *===========================================================================*/
int do_setclasslevel(void)
{
	int r;
	struct vnode *vp;
	struct vmnt *vmp;
	char fullpath[PATH_MAX];
	struct lookup resolve;
	vir_bytes vname;
	size_t vname_length;
	u16_t level;
	u16_t rlevel;

	//Get vnode
	vname = (vir_bytes) job_m_in.name;
	vname_length = (size_t) job_m_in.name_length;
	

	lookup_init(&resolve, fullpath, PATH_NOFLAGS, &vmp, &vp);
	resolve.l_vmnt_lock = VMNT_READ;
	resolve.l_vnode_lock = VNODE_READ;

	if (fetch_name(vname, vname_length, fullpath) != OK) return(err_code);
	if ((vp = eat_path(&resolve, fp)) == NULL) return(err_code);
	
	//Enforce access model
	r = EPERM;
	level = -1;
	rlevel = job_m_in.m3_i2;
	if(super_user ||																				//Super user
		((S_IWUSR & vp->v_mode) && (vp->v_uid == fp->fp_effuid)) ||									//User w/ write priveleges
		((S_IWGRP & vp->v_mode) && (vp->v_gid == fp->fp_effgid)) || 								//Group member w/ write priveleges
		((S_IWOTH & vp->v_mode) && (vp->v_uid != fp->fp_effuid) && (vp->v_gid != fp->fp_effgid)))	//Other w/ write priveleges	
	{
		r = req_getclasslevel(vp->v_fs_e, vp->v_inode_nr, rlevel, &level);
	}
	
	//Load reply
	m_out.RES_CLASSLEVEL = (r == OK) ? level : -1;

	unlock_vnode(vp);
	unlock_vmnt(vmp);

	put_vnode(vp);
	return r;
}