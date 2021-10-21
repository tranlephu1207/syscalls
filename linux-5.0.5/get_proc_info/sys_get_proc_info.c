#include <linux/kernel.h>
#include <linux/unistd.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/list.h>
#include <linux/syscalls.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/stat.h>

MODULE_LICENSE("GPL");


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

//asmlinkage long sys_get_proc_info(pid_t pid, struct procinfos * info) {
SYSCALL_DEFINE2(get_proc_info, pid_t, pid, struct procinfos *, info) {
	struct task_struct *task_list;
	struct list_head *p;

	info->studentID = 2033980;
	printk("Finding....\n");
	pid_t process_pid = pid;
	if (pid == -1) {
		process_pid = task_pid_nr(current);
	}
	printk("process_pid: %d\n", process_pid);
	for_each_process(task_list) {
		if (task_list->pid == process_pid) {
			printk("task_list pid: %d\n", task_list->pid);
			printk("is same pid\n");
			p = &task_list->children;
			if (list_empty(p)) {
				strcpy(info->oldest_child_proc.name, "NOT");
				info->oldest_child_proc.pid = 0;
			} else {
				info->oldest_child_proc.pid = list_first_entry(p, struct task_struct, sibling)->pid;
				strcpy(info->oldest_child_proc.name, list_first_entry(p, struct task_struct, sibling)->comm);
			}
			info->proc.pid = process_pid;
			strcpy(info->proc.name, task_list->comm);

			info->parent_proc.pid = task_list->parent->pid;
			strcpy(info->parent_proc.name, task_list->parent->comm);
			
			printk("student ID: %lu\n", info->studentID);
			printk("parent: [%d]\n", info->parent_proc.pid);
			printk("child: [%d]\n", info->oldest_child_proc.pid);
			return 0;
		}
	}
	return -EINVAL;
}

