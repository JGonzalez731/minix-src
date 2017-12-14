#include <lib.h>
#include <unistd.h>

int get_access_level(int pid)
{
	message m;
	m.m1_i1 = pid;
	
	_syscall(PM_PROC_NR, GETACCESSLEVEL, &m);
	
	return m.m1_i2;
}