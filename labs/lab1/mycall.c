/*
mycall.c

implementation of my call system
*/

#include <linux/linkage.h>

asmlinkage long sys_mycall(int i) {
    return i + 314;
}
