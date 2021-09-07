#include <stdio.h>
#include <sys/shm.h> // 共享内存的头文件
#include <unistd.h>
#include <string.h>
#include "shmdata.h"

int main(int argc, char *argv[])
{
    // step1.创建共享内存
    // 210120 共享内存的key需要两个进程都一样
    // 参数2共享内存大小
    // 共享内存权限 r w x
    int shmid = shmget((key_t)12545, sizeof(shared_user_st), 0666 | IPC_CREAT);
    if (shmid == -1)
    {
        fprintf(stderr, "shmget fail \n");
        return -1;
    }

    // step2.连接共享内存
    void *shm = shmat(shmid, NULL, 0); //at --> attach 如果成功就是该共享空间的首地址
    if (shm == (void *)-1)
    {
        fprintf(stderr, "shmat fail \n");
        return -2;
    }

    shared_user_st *shared = (shared_user_st *)shm;
    shared->written = 0; // 可写

    printf("start listen\n");

    while (1)
    {
        if (shared->written == 1)
        {
            // 有数据
            printf("read data: %s\n", shared->content);

            // 改变状态
            shared->written = 0;

            // 结束判断
            if (strncmp(shared->content, "end", 3) == 0)
            {
                break;
            }
        }
        else
        {
            // 还是可写的状态什么也不做
        }
        sleep(1);
    }

    // 从当前进程分离
    if (shmdt(shm) == -1) //dt -->  detach
    {
        fprintf(stderr, "shmdt fail \n");
        return -3;
    }

    // 删除共享内存
    if (shmctl(shmid, IPC_RMID, 0) == -1) // ctl --> control
    {
        fprintf(stderr, "shmctl fail \n");
        return -4;
    }

    return 0;
}