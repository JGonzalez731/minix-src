#include <lib.h>
#include <unistd.h>

int set_access_level(int pid, int new_level)
{
	message m;
	m.m1_i1 = pid;
	m.m1_i2 = new_level;
	
	_syscall(PM_PROC_NR, SETACCESSLEVEL, &m);
	
	return m.m1_i2;
}