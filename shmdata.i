# 1 "shmdata.h"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 32 "<command-line>" 2
# 1 "shmdata.h"



typedef struct shared_user_st
{
    int written;
    char content[4096];
} shared_user_st;
