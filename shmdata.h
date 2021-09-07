#ifndef _SHMDATA_HEADER_H
#define _SHMDATA_HEADER_H
#define SHARED_BUFFER_SIZE 4096
typedef struct shared_user_st
{
    int written; // 0表示可写 ，非0表示不可写
    char content[SHARED_BUFFER_SIZE];
} shared_user_st;

#endif // _SHMDATA_HEADER_H