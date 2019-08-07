#include<stdio.h>
#include"swtch.h"
#include<signal.h>
#include<sys/time.h>
#include<math.h>

long  lock=0;
struct context *Cmain,*Ctest1,*Ctest2;
struct context** ActiveContext;
int IsMain=1;

void test1(){
	int i;
	double A=0.12345678;
	for(i=0;1;i++){
		printf("\e[1;1H\e[2K test1:%d %lf\n",i,A);
		A=A+0.001;
	}
}

void test2(){
	int i;
	for(i=0;1;i++){
		printf("\e[2;1H\e[2K test2:%d %lf\n",i,sin(i*M_PI/180.0/100.0));
	}
}

void signalhandler(int no){
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
	timer.it_interval.tv_usec=100*1000;
	timer.it_value.tv_sec=0;
	timer.it_value.tv_usec=100*1000;
	setitimer(ITIMER_REAL,&timer,NULL);

	signal(SIGALRM,signalhandler);

	Ctest1  = mkcontext(test1,stack1,0x2000);
	Ctest2  = mkcontext(test2,stack2,0x2000);
	printf("\e[2J");

	while(1){
		ActiveContext=&Ctest1;
		IsMain=0;
		swtch(&Cmain,Ctest1,&lock);
		unblocksignal(SIGALRM);
		ActiveContext=&Ctest2;
		IsMain=0;
		swtch(&Cmain,Ctest2,&lock);
		unblocksignal(SIGALRM);
	}
	return 0;
}
