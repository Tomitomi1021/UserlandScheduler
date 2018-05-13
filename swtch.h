typedef unsigned long	ulong;
typedef unsigned short	ushort;
typedef unsigned int	uint;
typedef unsigned char	uchar;

typedef uchar	BYTE;
typedef ushort	WORD;
typedef uint	DWORD;
typedef ulong	QWORD;

struct context{
	QWORD rflags;
	QWORD r15;
	QWORD r14;
	QWORD r13;
	QWORD r12;
	QWORD r11;
	QWORD r10;
	QWORD r9;
	QWORD r8;
	QWORD rdi;
	QWORD rsi;
	QWORD rbp;
	QWORD rbx;
	QWORD rdx;
	QWORD rcx;
	QWORD rax;
	QWORD rip;
};
void swtch(struct context** old,struct context* new);
struct context* mkcontext(void(*func)(),void* stack,int stacksize);
