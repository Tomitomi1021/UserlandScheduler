#include<stdio.h>
#include"swtch.h"
#include<signal.h>
#include<sys/time.h>

struct context *Cmain,*Ctest;

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
		printf("test:%d %lf\n",i,A);
		A=A*A;
		swtch(&Ctest,Cmain);
	}
}

int main(){
	int i;
	char stack[0x1000];

	Ctest  = mkcontext(test,stack,0x1000);

	for(i=0;1;i++){
		printf("main:%d\n",i);
		swtch(&Cmain,Ctest);
	}
	return 0;
}
