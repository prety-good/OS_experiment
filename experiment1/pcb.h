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

typedef struct pcb{
    int pid=-1;
    int priority=-1;
    int memory=-1;

    bool operator<(const pcb a)const{
        return (this->priority!=a.priority)  ? this->priority > a.priority:this->pid<a.pid;
    }

    friend ostream& operator<<(ostream &out,const pcb &a){
        out<<"pid:"<<a.pid<<"\tpriority:"<<a.priority<<"\tsize of memery:"<<a.memory<<"MB"<<endl;
        return out;
    }
}pcb;




void Init();
void Show();
/**
 * @brief: 将running中的进程放回Ready，并把ready中的优先级最高的进程放入running
 */
void Dispatch_Timeout();
void EventWait();
void EventOccur();
void Create();
void Release();

/**
 * @brief：将New中转为Ready
 */
void admit();