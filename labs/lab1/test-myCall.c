/*
test-myCall.c

test the call system created
*/

#include <stdio.h>
#include <sys/syscall.h>

int main(){
    int x = syscall(345, 15); // Call my sys call
    printf("%d\n", x); 
    return 0;
}