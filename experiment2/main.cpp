#include"main.h"
using namespace std;


const int BUFF_SIZE=8;//设置缓冲区大小为8
queue<int> Full_que,Empty_que;
queue<string> Mutex_que;
queue<int> BUFFER;
int Empty,Full,Mutex;

//第n个数据
int i=1;

//cur进程和数据
string cur_process="0";
int cur_data=0;

int main(){
    // 初始化 默认刚开始时缓冲区为空
    Mutex=1;
    Empty=BUFF_SIZE;
    Full=0;

    while(1){
        cout<<"\t生产者消费者模拟：\t"<<endl;
        cout<<"1:输入Pn/pn, 输入第n个生产者进程(例如P1,p12,P123)"<<endl;
        cout<<"2:输入Cn/cn, 输入第n个消费者进程(例如C1,c12,C123)"<<endl;
        cout<<"3:输入E/e, 执行 (ExeEnd)"<<endl;
        cout<<"4:输入Q/q, 退出"<<endl;
        string temp;
        cin>>temp;
        system("clear");
        switch (temp[0]){
            case 'P':
            case 'p':product(atoi(temp.substr(1).c_str()));Show();break;
            case 'C':
            case 'c':consum(atoi(temp.substr(1).c_str()));Show();break;
            case 'E':
            case 'e':exeend();Show();break;
            case 'Q':
            case 'q':return 0;
            default:cout<<"Wrong Input!"<<endl;break;
        }
    }
    return 0;
}


bool P(string a){
    if(a=="Empty"){
        Empty--;
        return Empty>=0;
    }
    else if(a=="Full"){
        Full--;
        return Full>=0;
    }
    else if(a=="Mutex"){
        Mutex--;
        return Mutex>=0;
    }
    return false;
}

void V(string a){
    if(a=="Empty"){
        Empty++;
    }
    else if(a=="Full"){
        Full++;
    }
    else if(a=="Mutex"){
        Mutex++;
    }
    return;
}

void consum(int n){
    if(!P("Full")){
        Full_que.push(n);
    }
    else {
        if(!P("Mutex")){
            Mutex_que.push("C"+to_string(n));
        }
        else{
            cur_process="C"+to_string(n);
            cur_data = BUFFER.front();
        }
    }

}


void product(int n){
    if(!P("Empty")){
        Empty_que.push(n);
    }
    else {
        if(!P("Mutex")){
            Mutex_que.push("P"+to_string(n));
        }
        else{
            cur_process="P"+to_string(n);
            cur_data=(i++);
        }
    }

}

void exeend(){
    if(cur_process=="0"){
        cout<<"现在没有current_process!"<<endl;return;
    }
    else if(cur_process[0]=='P'){
        BUFFER.push(cur_data);
        V("Mutex");
        bool t=false;
        if(!mutex_proc()) {
            cur_data=0;
            cur_process="0";
            t=true;
        }
        V("Full");
        if(!consum_proc()&&t){
            cur_data=0;
            cur_process="0";
        }
    }
    else if(cur_process[0]=='C'){
        BUFFER.pop();
        V("Mutex");
        bool t=false;
        if(!mutex_proc()) {
            cur_data=0;
            cur_process="0";
            t=true;
        }
        V("Empty");
        if(!produce_proc()&&t){
            cur_data=0;
            cur_process="0";
        }
    }
    return;
}

bool produce_proc(){
    if(Empty_que.empty()) return false;
    int n=Empty_que.front();Empty_que.pop();
    
    if(!P("Mutex")){
        Mutex_que.push("P"+to_string(n));
        return false;
    }
    else{
        cur_process="P"+to_string(n);
        cur_data=(i++);
        return true;
    }
}


bool consum_proc(){
    if(Full_que.empty()) return false;
    int n=Full_que.front();Full_que.pop();

     if(!P("Mutex")){
        Mutex_que.push("C"+to_string(n));
        return false;
    }
    else{
        cur_process="C"+to_string(n);
        cur_data = BUFFER.front();
        return true;
    }
}

bool mutex_proc(){
    if(Mutex_que.empty()) return false;
    string temp=Mutex_que.front();Mutex_que.pop();
    cur_process=temp;
    cur_data = temp[0]=='P' ? (i++):BUFFER.front();
    return true;
}

void Show(){
    cout<<"BUFFER:\t\t";
    show_queue(BUFFER);
    cout<<"Empty_queue:\t";
    show_queue(Empty_que);
    cout<<"Full_queue:\t";
    show_queue(Full_que);
    cout<<"Mutex_queue:\t";
    show_queue(Mutex_que);
    cout<<"Mutex:\t\t"<<Mutex<<endl;
    cout<<"Full:\t\t"<<Full<<endl;
    cout<<"Empty:\t\t"<<Empty<<endl;
    cout<<"cur_process:\t"<<cur_process<<endl;
    cout<<"cur_data:\t"<<cur_data<<"\t(正在读取或写入的数据)"<<endl;

}

void show_queue(queue<int> temp){
    while(!temp.empty()){
        cout<<temp.front()<<" ";
        temp.pop();
    }
    cout<<endl;
    return;
}

void show_queue(queue<string> temp){
    while(!temp.empty()){
        cout<<temp.front()<<" ";
        temp.pop();
    }
    cout<<endl;
    return;
}

