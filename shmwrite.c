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
    char buffer[BUFSIZ + 1]; // BUFSIZ == 8192
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
    while (1)
    {
        while (shared->written == 1)
        {
            sleep(1);
            printf("Waiting...\n");
        }

        printf("please input text:\n");
        // 读取输入的数据
        fgets(buffer, BUFSIZ, stdin);

        // 将读取的数据写入共享内存
        strncpy(shared->content, buffer, SHARED_BUFFER_SIZE);

        shared->written = 1;

        // 输入了end，退出循环（程序）
        if (strncmp(buffer, "end", 3) == 0)
        {
            break;
        }
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