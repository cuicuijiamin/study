#include<bits/stdc++.h>
using namespace std;
#define _PROCESS_NUM 5
enum PROCESS_STATUS{//进程状态枚举类型
    RUN,//运行
    WAIT,//就绪
    FINISH//完成
};
struct process{
    //记录PCB信息
    //需要描述1.进程标识 2.优先数/轮转时间片数 3.占用cpu时间片数
    //4 进程需要的时间片数 5.进程状态
    int id;
    int priority;
    int cputime;
    int alltime;
    PROCESS_STATUS state;
    int next;
}prochain[_PROCESS_NUM+1];

void timesche();
void init();
void print();
void insert2();

void insert(int id);

void prisch();

int pronum;
int algo;
int run,head,tail;
int main(){
    srand(time(NULL));//播下随机种子
    cout<<"enter method id(1.round robin 2.priority):";
    cin>>algo;
    if(algo==1){//简单轮转
        init();
        timesche();
    }
    else if(algo==2){//优先数法
        init();
        prisch();
    }
    else{
        cout<<"enter failed"<<endl;
    }
}
void init() {//所有进程初始全部进入等待队列
    if(algo==1){//简单轮转法
        for(int i=1;i<=_PROCESS_NUM;++i){
            prochain[i].id=i;
            prochain[i].priority=(rand()+11)%41+1;
            prochain[i].cputime=0;
            prochain[i].alltime=(rand()+1)%7+1;
            prochain[i].state=WAIT;
            prochain[i].next=i+1;
            if(i==_PROCESS_NUM)prochain[i].next=0;//没有后面的pcb了
        }
        head=1;
        tail=_PROCESS_NUM;
    }else{//优先数法
        for(int i=1;i<=_PROCESS_NUM;++i){
            prochain[i].id=i;
            prochain[i].priority=(rand()+11)%41+1;
            prochain[i].cputime=0;
            prochain[i].alltime=(rand()+1)%7+1;
            prochain[i].state=WAIT;
            prochain[i].next=0;
            if(prochain[i].priority<prochain[head].priority&&(head!=0)){
                insert(prochain[i].id);
            }else{
                prochain[i].next=head;
                head= prochain[i].id;
            }
        }
        head=1;
        tail=_PROCESS_NUM;
    }
    //将head指针指向的进程设置为运行状态
    run=head;
    prochain[run].state=RUN;
    head=prochain[head].next;
    prochain[run].next=0;
    print();
}

void print() {
    int k,p;
    for (k=1;k<=40;k++)
        printf("=");
    printf("\nrunning proc.         ");
    printf("waiting queue.");
    printf("\n    %d           ",prochain[run].id);
    p=head;
    while(p!=0){
        printf("%5d",p);
        p=prochain[p].next;
    }
    printf("\n");
    for (k=1;k<=40;k++)
        printf("=");
    printf("\n");
    printf("   id          ");
    for (k=1;k<=_PROCESS_NUM;k++)
        printf("%5d",prochain[k].id);
    printf("\n");
    printf("priority       ");
    for (k=1;k<=_PROCESS_NUM;k++)
        printf("%5d",prochain[k].priority);
    printf("\n");
    printf("cputime        ");
    for (k=1;k<=_PROCESS_NUM;k++)
        printf("%5d",prochain[k].cputime);
    printf("\n");
    printf("alltime        ");
    for (k=1;k<=_PROCESS_NUM;k++)
        printf("%5d",prochain[k].alltime);
    printf("\n");
    printf("state          ");
    for (k=1;k<=_PROCESS_NUM;k++){
        char c;
        if(prochain[k].state==RUN)c='R';
        else if(prochain[k].state==WAIT)c='W';
        else c='F';
        printf("%5c",c);
    }
    printf("\n");
}

void timesche() {//简单轮转法核心代码
    while(run!=0)
    {
        prochain[run].alltime-=1;
        prochain[run].cputime+=1;
        if(prochain[run].alltime==0){//此进程结束 将其放入结束队列，运行队列指针移动
            prochain[run].state=FINISH;
            prochain[run].next=0;
            if(head!=0){
                run=head;//运行指针，指向就绪指针指向的进程
                prochain[run].state=RUN;
                head=prochain[head].next;
            }
            else{
                prochain[0].id=prochain[run].id;
                run=0;
            }
        }
        else{
            if((prochain[run].cputime==prochain[run].
                    priority)&&(head!=0))//此进程的轮转时间片用尽，需要把它放去等待队列
            {
                prochain[run].state=WAIT;
                prochain[run].cputime=0;
                insert2();
                run=head;//更新运行指针
                prochain[run].state=RUN;
                head=prochain[head].next;
            }
        }
        print();
    }
}
void prisch() {//优先数法核心代码
    while(run!=0){
        prochain[run].cputime+=1;
        prochain[run].priority-=3;
        prochain[run].alltime-=1;
        if(prochain[run].alltime==0){//此进程执行完毕
            prochain[run].state=FINISH;
            prochain[run].next=0;
            if(head!=0){//将等待队列的进程进入到运行状态
                run=head;
                prochain[run].state=RUN;
                head=prochain[head].next;
            }
            else{
                prochain[0].id=prochain[run].id;
                run=0;
            }
        }else{
            if((prochain[run].priority< prochain[head].priority)&&(head!=0)){//需要判断此时要运行的进程
                prochain[run].state=WAIT;
                insert(run);
                run=head;
                prochain[run].state=RUN;
                head= prochain[head].next;
            }
        }
        print();
    }
}
void insert(int q) {//优先数法链表的插入：根据优先数的大小插入到合理的位置
    int p,s;//p和s是用于移动的临时指针
    p=head;
    s=prochain[head].next;
    while((prochain[q].priority<prochain[s].priority)&&(s!=0)){
        p=s;
        s=prochain[s].next;
    }
    //找到q应该在的位置，在p和s之间
    prochain[p].next=q;
    prochain[q].next=s;
}
void insert2() {//简单轮转法链表插入:尾插
    prochain[tail].next=run;//往等待队列的队尾插入 当前释放资源的进程
    tail=run;
    prochain[run].next=0;
}
