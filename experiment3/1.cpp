/**
 * @author:孙
 * @brief:扩展一  双向发送消息
 **/
#include<bits/stdc++.h>
#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<error.h>
#include<wait.h>
#include<unistd.h>
using namespace std;

#define  BUFF_SIZE  512
const int N = 5; // 创建的线程的数量 可随意更改

int main(int argc, char* argv[]){
    int fd[2], fd_x[2];
    pid_t pid;
    if(pipe(fd) == -1 || pipe(fd_x) == -1){
        fprintf(stderr, "创建管道失败！" );
        return -1;
    }
    for(int i = 1; i <= N; i++){
        pid = fork();
        if(pid == -1){
            fprintf(stderr, "创建进程失败！" );
            return -1;
        }
        /**
         * pipe 的 write 和 read 的长度一定要相同，这里全部设置为了 sizeof(char)*BUFF_SIZE
         * 对于 char str[BUFF_SIZE];出来的栈空间,  sizeof(str)就直接为全部的长度
         * 而 对于 char* str = new char[BUFF_SIZE]; 出来的堆空间  需要sizeof(char)*BUFF_SIZE.
         **/ 
        else if(pid == 0){
            // 读取父进程发送的消息
            char buff[BUFF_SIZE];
            close(fd_x[1]); // 先关闭掉写端 防止阻塞
            if(read(fd_x[0], buff, BUFF_SIZE) != 0) {
                printf("No.%d child process get the father's message! :%s\n",i, buff);
            }

            // 向父进程发送消息
            lockf(fd[1], 1, 0); //管道的写锁
            // char str[BUFF_SIZE];
            char* str = new char[BUFF_SIZE];
            sprintf(str, "Child process %d is sending a message! 它的pid:%d  它的父进程的pid:%d\n", i, getpid(), getppid());
            //cout << str <<endl;
            write(fd[1], str, sizeof(char)*BUFF_SIZE); // 写入管道
            sleep(1);  // 进入睡眠
            lockf(fd[1], 0, 0); // 释放锁

            exit(0);// 结束子进程
        }
    }

    //发送给子进程消息
    char str[] = {"Father process is sending a message!"};
    write(fd_x[1], str, sizeof(str));
    close(fd_x[1]);

    for(int i = 1; i <= N ; i++){
            // 接受子进程的消息
            char buff[BUFF_SIZE];
            wait(NULL); // 等待一个子线程结束
            // cout << wait(NULL) << endl;
            read(fd[0], buff, sizeof(buff)); //从管道中读取
            cout << buff;
    }

    return 0;
}