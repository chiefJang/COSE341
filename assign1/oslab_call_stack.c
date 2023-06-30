#include <unistd.h>
#include <stdio.h>

#define my_stack_push_syscall 335
#define my_stack_pop_syscall 336

int main(){
    int r = 0;
    int a = 1;
    r = syscall(my_stack_push_syscall, a);
    printf("Push : %d\n", r);
    r = syscall(my_stack_push_syscall, a);
    printf("Push : %d\n", r);
    r = syscall(my_stack_push_syscall, a+1);
    printf("Push : %d\n", r);
    r = syscall(my_stack_push_syscall, a+2);
    printf("Push : %d\n", r);

    r = syscall(my_stack_pop_syscall);
    printf("Pop : %d\n", r);
    r = syscall(my_stack_pop_syscall);
    printf("Pop : %d\n", r);
    r = syscall(my_stack_pop_syscall);
    printf("Pop : %d\n", r);
    r = syscall(my_stack_pop_syscall);
    printf("Pop : %d\n", r);

    return 0;
}
