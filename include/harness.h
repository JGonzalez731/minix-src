#include <lib.h>

//Returns the call number of the system call that would be used to set the access level of a process.
int retrieve_set_access_callnum()
{
	return SETACCESSLEVEL;
}

//Returns the call number of the system call that would be used to get the access level of a process.
int retrieve_get_access_callnum()
{
	return GETACCESSLEVEL;
}

//Returns a message structure that could be used to invoke a system call to set the access level of a process.
message retrieve_set_access_message(int pid, int new_level)
{
	message m;
	m.m1_i1 = pid;
	m.m1_i2 = new_level;
	return m;
}

//Returns a message structure that could be used to invoke a system call to get the access level of a process.
message retrieve_get_access_message(int pid)
{
	message m;
	m.m1_i1 = pid;
	return m;
}

//Returns set_access_level’s interpretation of the system call completing with return value retval and with final message state msg.
int interpret_set_access_result(int retval, message msg)
{
	return msg.m1_i2;
}

//Returns set_access_level’s interpretation of the system call completing with return value retval and with final message state msg.
int interpret_get_access_result(int retval, message msg)
{
	return msg.m1_i2;
}