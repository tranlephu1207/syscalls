#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/sched/task.h>
#include <linux/syscalls.h>
#include <linux/errno.h>
#include <linux/uaccess.h>

struct proc_info { // info about a single process
	pid_t pid; // pid of the process
	char name[16]; // file name of the program executed
};

struct procinfos { // info about processes we need
	long studentID; // for the assignment testing
	struct proc_info proc; // process with pid or current process
	struct proc_info parent_proc; // parent process
	struct proc_info oldest_child_proc; // oldest child process
};

asmlinkage long sys_get_proc_info(pid_t pid, struct procinfos * info) {
	struct procinfos *myinfo = (struct procinfos*)kmalloc(sizeof(struct procinfos), GFP_KERNEL);
	struct task_struct *proc = NULL;
	struct task_struct *parent_proc = NULL;
	struct task_struct *oldest_child_proc = NULL;
	void* isChildProc = NULL;

	if (myinfo == NULL) return EINVAL;

	if (pid == 0) proc = &init_task;
	else if (pid == -1) proc = current;
	else proc = find_task_by_vpid(pid);

	myinfo->proc.pid = proc->pid;
	strcpy(myinfo->proc.name, proc->comm);

	parent_proc = proc->parent;
	myinfo->parent_proc.pid = parent_proc->pid;
	strcpy(myinfo->parent_proc.name, parent_proc->comm);

	// find oldest child process
	isChildProc = list_first_entry_or_null(&proc->children, struct task_struct, sibling);
	if (isChildProc == NULL) {
		myinfo->oldest_child_proc.pid = (pid_t) -1;
		strcpy(myinfo->oldest_child_proc.name, "No Name");
	} else {
		oldest_child_proc = list_first_entry(&proc->children, struct task_struct, sibling);
		myinfo->oldest_child_proc.pid = oldest_child_proc->pid;
		strcpy(myinfo->oldest_child_proc.name, oldest_child_proc->comm);
	}
	myinfo->studentID = 2033980;
	copy_to_user(info, myinfo, sizeof(struct procinfos));
	kfree(myinfo);

	return 0;
}
