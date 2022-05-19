/**
 * @author:孙
 * @brief:扩展点一 观察FIFO的Belady现象 两个线程分别使用不同大小的物理块数
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

typedef pair<int, int> PII;
const int N = 12;
//用于观察belady现象的页面序列
int Acess_Series[] = {3,2,1,0,3,2,4,3,2,1,0,4};

void FIFO(int MAX_FRAME);

int main(){
    cout << "页面序列：";
    for(int i = 0; i < N; i++){
       cout <<  Acess_Series[i] <<" ";
    }
    puts("");

    pid_t pid;
    bool FLAG = false;
    for(int i = 1; i <= 2; i++){
        pid = fork();
        if(pid == -1){
            fprintf(stderr, "创建进程失败！" );
            return -1;
        }
        else if(pid > 0){
            FLAG = true;
            wait(NULL);
        }
        else{
            if(FLAG) { 
                FIFO(4);
            }
            else {
                FIFO(3);
            }
            exit(0);
        }
    }
    return 0;
}

/**
 * @brief: 有k个内存页面的FIFO算法
 *
 **/
void FIFO(int MAX_FRAME){
    cout <<"有"<<MAX_FRAME<<"块物理块！"<<endl;
    PII mem[MAX_FRAME]; // 内存中的页面编号 和对应算法记录的值  
    //初始化  first为编号  second 为记录的值。
    for(int i = 0; i < MAX_FRAME; i++){
        mem[i].first = -1;
        mem[i].second = 0;
    }
    cout<<"FIFO!:\t"<<"访问页面\t"<<"\t当前内存"<<endl;
    int diseffect = 0;//缺页数
    for(int i = 0; i < N; i++){
        int x = Acess_Series[i]; //要使用的页面
        bool flag = true;
        for(int i = 0; i < MAX_FRAME; i++){
            if(x == mem[i].first ){ 
                flag = false;
                cout<<"\t";
                break; //命中
            }
            else if(mem[i].first == -1){
                mem[i].first = x;
                mem[i].second = 0;
                flag = false;
                diseffect++;
                cout <<"缺页!\t";
                break; //有空余 也是缺页
            }
        }
        if(flag){
            diseffect++;
            int t = 0, ma=0;
            // 找出时间最久的
            for(int i = 0; i < MAX_FRAME; i++){
                if(mem[i].second > ma) t = i, ma = mem[i].second;
            }
            //置换掉
            cout <<"缺页!\t";
            mem[t].first = x;
            mem[t].second = 0;
        }
        // 进入的时间加一
        for(int i = 0; i < MAX_FRAME; i++){
            if(mem[i].first != -1)  mem[i].second ++;
        }
        //输出状态
        cout << x <<"\t\t";
        for(int i = 0; i < MAX_FRAME; i++){
            cout << mem[i].first << "\t";
        }
        puts("");
    }
    cout <<"缺页共" << diseffect <<"个，总页面" << N <<"个，缺页率：" << (double) diseffect/N<<endl;
    return ;
}
