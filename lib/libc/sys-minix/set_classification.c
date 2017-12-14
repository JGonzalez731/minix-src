#include <lib.h>
#include <stdio.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <unistd.h>

int set_classification(const char *filename, int new_level)
{
	message m;
	int rfd;
	
	//Open file
	if((rfd = open(filename, O_WRONLY)) == -1) return -1;
	
	//Make system call
	m.m1_i1 = rfd;
	m.m1_i2 = new_level;
    _syscall(VFS_PROC_NR, SETCLASSLEVEL, &m);
	
	//Close file
	close(rfd);
	
	return m.m1_i3;
}