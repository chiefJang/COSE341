#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/linkage.h>
#define MAX_SIZE 500
int stack[MAX_SIZE];
int top = -1;

SYSCALL_DEFINE1(os2023_push, int, a){
	int check = 0;//if trying push existing in stack, check = 1
	int i = 0;
	if(top!=-1){//if stack is empty, no check
		for(i=0; i <= top; i++){//check
			if(stack[i] == a){
				check = 1;
				break;
			}
		}
	}
	printk(KERN_INFO "[System Call] os2023_push : \n");
	if(check == 0){
		if(top == MAX_SIZE-1){//if stack is full, don't push
			printk(KERN_INFO "[ERROR] STACK IS FULL\n");
		}
		else{
			stack[++top] = a;//push
		}
	}
	printk("Stack Top ---------------\n");
	for(i=top; i>= 0; i--){
		printk( "%d\n", stack[i]);
	}
	printk("Stack Bottom ------------\n");
	return a;
}

SYSCALL_DEFINE0(os2023_pop){
	int i = 0;
	printk(KERN_INFO "[System Call] os2023_pop : \n");
	if(top == -1){//if stack is empty, print error
		printk(KERN_INFO "[Error] STACK IS EMPTY\n");
		printk("Stack Top ---------------\n");
		printk("Stack Bottom ------------\n");
		return -1;
	}
	top = top-1;//pop
	printk("Stack Top ---------------\n");
	for(i=top; i>=0; i--){
		printk("%d\n", stack[i]);
	}
	printk("Stack Bottom ------------\n");
	return stack[top+1];
	
}
