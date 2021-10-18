#include <get_proc_info.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	int mypid = atoi(argv[1]);
	printf("PID: %d\n", mypid);
	long sys_return_value;
	struct procinfos info;

	sys_return_value = get_proc_info(mypid, &info);
	printf("Student ID: %lu\n", info.studentID);

	if (sys_return_value == 0) {
		printf("Current pid: %d, current process name: %s\n", info.proc.pid, info.proc.name);
		printf("Parent pid: %d, parent process name: %s\n", info.parent_proc.pid, info.parent_proc.name);
		if (info.oldest_child_proc.pid == 0) {
			printf("This process has no child\n");
		} else {
			printf("Child pid: %d, child process name: %s\n", info.oldest_child_proc.pid, info.oldest_child_proc.name);
		}
	} else {
		printf("Cannot get information from the process %d\n", mypid);
	}
	//sleep(100);
	return 0;
}

