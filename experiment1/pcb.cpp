#include "pcb.h"
using namespace std;

//定义内存为500MB
#define MEM 500
int now_mem=0;
set<pcb> Ready,Running,Block,New;
set<int> PID;


int main(int argc,char* argv[]){
    Init();


    int p;
    while(1) {
        printf("\n请选择想要进行的操作:\n1.Dispatch&&Timeout\n2.Event Wait\n3.Event Occur\n4.Create\n5.Release\n6.结束程序\n");
        cin>>p;
        switch(p){
        case 1:system("clear");Dispatch_Timeout();break;
        case 2:system("clear");EventWait();break;
        case 3:system("clear");EventOccur();break;
        case 4:system("clear");Create();break;
        case 5:system("clear");Release();break;
        case 6:return 0;
        default:cout<<"输入错误！\n";
        }

    }
    return 0;
}


void Init(){

    int nums;
    int pid,memory,priority;

    printf("初始化：\n请输入准备队列中的进程数:\n");
    scanf("%d",&nums);
    printf("请依次输入进程的pid、优先级、内存大小:\n");
    while(nums--){
        cin>>pid>>priority>>memory;
        if(PID.count(pid)==1) {cout<<"该pid已存在！"<<endl;continue;}
        else if(memory>MEM) {cout<<"该进程内存超出最大内存！！！"<<endl;continue;}
        else {PID.insert(pid);}
        New.insert(pcb{pid,priority,memory});
    }
    printf("请输入就绪队列中的进程数:\n");
    scanf("%d",&nums);
    printf("请依次输入进程的pid、优先级、内存大小:\n");
    while(nums--){
        cin>>pid>>priority>>memory;
        if(PID.count(pid)==1) {cout<<"该pid已存在！"<<endl;continue;}
        else if(memory>MEM) {cout<<"该进程内存超出最大内存！！！"<<endl;continue;}
        else {PID.insert(pid);}
        now_mem+=memory;
        Ready.insert(pcb{pid,priority,memory});
    }
    printf("运行队列中是否已经存在进程：(1 | 0)\n");
    scanf("%d",&nums);
    if(nums==1){
        printf("请输入该运行进程的pid、优先级、内存大小:\n");
        cin>>pid>>priority>>memory;
        if(PID.count(pid)==1) {cout<<"该pid已存在！"<<endl;}
        else if(memory>MEM) {cout<<"该进程内存超出最大内存！！！"<<endl;}
        else {
        PID.insert(pid);
        now_mem+=memory;
        Running.insert(pcb{pid,priority,memory});
        }
    }
    printf("输入堵塞队列中的进程数:\n");
    scanf("%d",&nums);
    printf("请依次输入进程的pid、优先级、内存大小:\n");
    while(nums--){
        cin>>pid>>priority>>memory;
        if(PID.count(pid)==1) {cout<<"该pid已存在！"<<endl;continue;}
        else if(memory>MEM) {cout<<"该进程内存超出最大内存！！！"<<endl;continue;}
        else {PID.insert(pid);}
        now_mem+=memory;
        Block.insert(pcb{pid,priority,memory});
    }

    admit();
    Show();
}

void Show(){
    cout<<"内存占用："<<now_mem<<"MB / "<<MEM<<"MB"<<endl;
    cout<<"准备队列：\n";
    for(const auto&i:New){
        cout<<i;
    }

    cout<<"就绪队列：\n";
    for(const auto&i:Ready){
        cout<<i;
    }

    cout<<"运行队列：\n";
    for(const auto&i:Running){
        cout<<i;
    }

    cout<<"阻塞队列：\n";
    for(const auto&i:Block){
        cout<<i;
    }
}


void admit(){
    set<pcb>::iterator it=New.begin();
    while(!New.empty()&&(it->memory+now_mem)<=MEM){
        now_mem+=(it->memory);
        Ready.insert(*it);
        New.erase(it++);
    }
    if(Running.empty()&&!Ready.empty()){
        Running.insert(*Ready.begin());
        Ready.erase(Ready.begin());
    }
}

void Dispatch_Timeout(){
    if(Ready.empty()){
        cout<<"就绪队列为空！"<<endl;
        Show();
        return;
    }
    set<pcb>::iterator it=Running.begin();
    Running.insert(*Ready.begin());
    Ready.erase(Ready.begin());
    Ready.insert(*it);
    Running.erase(it);
    Show();
}

void EventWait(){
    if(Running.empty()) {
        cout<<"当前运行队列为空！！！"<<endl;
        Show();
        return;
    }
    set<pcb>::iterator it=Running.begin();
    if(!Ready.empty()){
    Running.insert(*Ready.begin());
    Ready.erase(Ready.begin());
    }
    Block.insert(*it);
    Running.erase(it);
    Show();
}

void EventOccur(){
    if(Block.empty()){
        cout<<"阻塞队列为空！！！"<<endl;
        Show();
        return;
    }
    int pid;
    Show();
    cout<<"请输入要唤醒的进程的pid："<<endl;
    scanf("%d",&pid);
    set<pcb>::iterator it=find_if(Block.begin(),Block.end(),[pid](const pcb& p){return p.pid==pid;});
    if(it==Block.end()) {
        cout<<"阻塞队列中没有该pid的进程！"<<endl;
        system("clear");
        Show();
        return;
    }
    if(Running.empty()&&Ready.empty()){
        Running.insert(*it);
        Block.erase(it);
    }
    else {
        Ready.insert(*it);
        Block.erase(it);
    }
    system("clear");
    Show();
}

void Create(){
    cout<<"请输入进程的pid、优先级、内存"<<endl;
    int pid,priority,memory;
    cin>>pid>>priority>>memory;
    if(PID.count(pid)==1) {cout<<"该pid已存在！"<<endl;Show();return;}
    else if(memory>MEM) {cout<<"该进程内存超出最大内存！！！"<<endl;Show();return;}
    PID.insert(pid);
    New.insert(pcb{pid,priority,memory});
    admit();
    cout<<"创建完毕！\n";
    Show();
}

void Release(){
    if(Running.empty()) {
        cout<<"当前运行队列为空！！！"<<endl;
        Show();
        return;
    }
    set<pcb>::iterator it=Running.begin();
    if(!Ready.empty()){
    Running.insert(*Ready.begin());
    Ready.erase(Ready.begin());
    }
    now_mem-=it->memory;
    PID.erase(it->pid);
    Running.erase(it);
    admit();
    Show();
}