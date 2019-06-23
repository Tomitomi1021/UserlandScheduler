#include<stdio.h>
#include"swtch.h"
#include<signal.h>
#include<sys/time.h>

long  lock=0;
struct context *Cmain,*Ctest1,*Ctest2;
struct context** ActiveContext;
int IsMain=1;

struct context* mkcontext(void (*func)(),void* stack,int stacksize){
	struct context* c;
	void* sp = stack;
	sp += stacksize;
	sp = (void*)((QWORD)sp - (QWORD)sp % 0x10);
	sp -= 0x8;
	sp -= sizeof(struct context);
	c = (struct context*)sp;
	c->rip = (QWORD)func;
	c->rbp = (QWORD)stack + stacksize;
	c->rbp -= c->rbp % 0x10;
	return c;
}

void test(){
	int i;
	double A=0.12345678;
	for(i=0;1;i++){
		printf("test1:%d %lf\n",i,A);
		A=A*A;
	}
}

void test2(){
	int i;
	double A=0.12345678;
	for(i=0;1;i++){
		printf("test2:%d %lf\n",i,A);
		A=A*A;
	}
}

void signalhandler(int no){
/*
	printf("SIGNAL!!:%d\n",no);
	if(ActiveContext==&Ctest1){
		printf("ActiveContext:Ctest1\n");
	}else if(ActiveContext==&Ctest2){
		printf("ActiveContext:Ctest2\n");
	}else{
		printf("ActiveContext:Undefined\n");
	}
	printf("lock:%d\n",lock);
*/
	if(lock)return;

	switch(no){
	case SIGALRM:
		if(!IsMain){
			IsMain=1;
			swtch(ActiveContext,Cmain,&lock);
		}
		break;
	}
}

void unblocksignal(int no){
	sigset_t st;
	sigemptyset(&st);
	sigaddset(&st,no);
	sigprocmask(SIG_UNBLOCK,&st,NULL);
}

int main(){
	int i;
	struct itimerval timer;
	char stack1[0x2000];
	char stack2[0x2000];

	timer.it_interval.tv_sec=0;
	timer.it_interval.tv_usec=100;
	timer.it_value.tv_sec=0;
	timer.it_value.tv_usec=100;
	setitimer(ITIMER_REAL,&timer,NULL);

	signal(SIGALRM,signalhandler);

	Ctest1  = mkcontext(test,stack1,0x2000);
	Ctest2  = mkcontext(test2,stack2,0x2000);

	while(1){
		ActiveContext=&Ctest1;
		IsMain=0;
		swtch(&Cmain,Ctest1,&lock);
		//printf("main:%d\n",__LINE__);
		unblocksignal(SIGALRM);
		ActiveContext=&Ctest2;
		IsMain=0;
		swtch(&Cmain,Ctest2,&lock);
		//printf("main:%d\n",__LINE__);
		unblocksignal(SIGALRM);
	}
	return 0;
}
