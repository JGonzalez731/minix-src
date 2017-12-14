#include <lib.h>
#include <stdio.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <unistd.h>

int get_classification(const char *filename)
{
	message m;
	int rfd;
	
	//Open file
	if((rfd = open(filename, O_RDONLY)) == -1) return -1;
	
	//Make system call
	m.m1_i1 = rfd;
    _syscall(VFS_PROC_NR, GETCLASSLEVEL, &m);
	
	//Close file
	close(rfd);
	
	return m.m1_i3;
}