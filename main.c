#include<stdio.h>
#include"swtch.h"
#include<signal.h>
#include<sys/time.h>

struct context *Cmain,*Ctest1,*Ctest2;

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
		swtch(&Ctest1,Cmain);
	}
}

void test2(){
	int i;
	double A=0.12345678;
	for(i=0;1;i++){
		printf("test2:%d %lf\n",i,A);
		A=A*A;
		swtch(&Ctest2,Cmain);
	}
}

int main(){
	int i;
	char stack1[0x1000];
	char stack2[0x1000];

	Ctest1  = mkcontext(test,stack1,0x1000);
	Ctest2  = mkcontext(test2,stack2,0x1000);

	while(1){
		swtch(&Cmain,Ctest1);
		swtch(&Cmain,Ctest2);
	}
	return 0;
}
