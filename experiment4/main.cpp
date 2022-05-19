/**
 * @author:孙
 * @brief:基础点 两个子进程分别完成FIFO和LRU
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


const int MAX_FRAME = 3; //一个进程的最多的实际分配的物理页面数 
const int K = 6; //程序逻辑页面的面数 1~K
const int N = 20; //随机访问序列的长度



int Acess_Series[N]; // 随机访问序列
typedef pair<int, int> PII;
PII mem[MAX_FRAME]; // 内存中的页面编号 和对应算法记录的值

void FIFO(){
    cout<<"FIFO!:\t"<<"访问页面\t"<<"\t当前内存"<<endl;
    int diseffect = 0;//缺页数
    for(int j = 0; j < N; j++){
        int x = Acess_Series[j]; //要使用的页面
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

void LRU(){
    cout<<"LRU!:\t"<<"访问页面\t"<<"\t当前内存"<<endl;
    int diseffect = 0;
    for(int j = 0; j < N; j++){
        int x = Acess_Series[j]; //要使用的页面
        bool flag = true;
        for(int i = 0; i < MAX_FRAME; i++){
            if(x == mem[i].first ){ 
                flag = false;
                mem[i].second = 0;// 重置访问时间  与FIFO的唯一区别在于此罢了
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


int main(){
    srand((unsigned int)(time(NULL)));
    // 随机生成20个 [1,K] 之间的数 放入Acess_Series 访问序列数组。
    for(int i = 0; i < N; i++){
        Acess_Series[i] = rand() % K + 1;
    }
    cout << "页面序列：";
    for(int i = 0; i < N; i++){
       cout <<  Acess_Series[i] <<" ";
    }
    puts("");

    //初始化  first为编号  second 为记录的值。
    for(int i = 0; i < MAX_FRAME; i++){
        mem[i].first = -1;
        mem[i].second = 0;
    }

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
                LRU();
            }
            else {
                FIFO();
            }
            exit(0);
        }
    }
    return 0;
}
