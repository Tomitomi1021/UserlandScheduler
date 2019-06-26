#include"swtch.h"

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

