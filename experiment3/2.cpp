/**
 * @author:孙
 * @brief:取消掉父子进程的默认同步
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

// 是否开启子进程之间的互斥  注释掉为不互斥
// #define Mutex
// 是否开启父子进程之间的同步   注释掉为不同步
// #define Sync

const int N = 4; // 创建的线程的数量
const int NUM = 512;
int main(int argc, char* argv[]){
    int fd[2];
    pid_t pid;
    if(pipe(fd) == -1){
        fprintf(stderr, "创建管道失败！" );
        return -1;
    }
    for(int i = 1; i <= N; i++){
        pid = fork();
        if(pid == -1){
            fprintf(stderr, "创建进程失败！" );
            return -1;
        }
        else if(pid == 0){
            #ifdef Mutex
                lockf(fd[1], 1, 0); //管道的写锁
            #endif
            
            // 非阻塞
            int flags = fcntl(fd[1], F_GETFL, 0);
            fcntl(fd[1], F_SETFL, flags|O_NONBLOCK );
        
            char p[NUM];
            memset(p, i+'0', sizeof(p));
            p[NUM-1] = '\0';
            // write(fd[1], p, sizeof(p));
            for(int j = 0; j < NUM ; j ++){
                write(fd[1], p+j, sizeof(char));// 写入 NUM个 i
            }
            
            // sleep(1);  // 进入睡眠
            #ifdef Mutex
                lockf(fd[1], 0, 0); // 释放锁0
            #endif
            exit(0);// 结束子进程
        }
    }
    // sleep(0.05);
    
    // 非阻塞
    int flags = fcntl(fd[0], F_GETFL, 0);
    fcntl(fd[0], F_SETFL, flags|O_NONBLOCK );

    for(int i = 1; i <= N ; i++){
        #ifdef Sync
            wait(NULL); // 等待一个子线程结束
        #endif
            char buff[NUM];
            read(fd[0], buff, sizeof(buff)); //从管道中读取
            cout << buff;

    }
    cout<<endl;

    return 0;
}