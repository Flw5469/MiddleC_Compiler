	.file	"test.cpp"
 # GNU C++17 (Rev2, Built by MSYS2 project) version 12.1.0 (x86_64-w64-mingw32)
 #	compiled by GNU C version 12.1.0, GMP version 6.2.1, MPFR version 4.1.0-p13, MPC version 1.2.1, isl version isl-0.24-GMP

 # GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
 # options passed: -mtune=generic -march=x86-64
	.text
	.def	__main;	.scl	2;	.type	32;	.endef
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
.LFB0:
	pushq	%rbp	 #
	.seh_pushreg	%rbp
	movq	%rsp, %rbp	 #,
	.seh_setframe	%rbp, 0
	subq	$48, %rsp	 #,
	.seh_stackalloc	48
	.seh_endprologue
 # test.cpp:23: int main(){
	call	__main	 #
 # test.cpp:29:   int d = 6+2/2+(3+1);
	movl	$11, -4(%rbp)	 #, d
 # test.cpp:30: }
	movl	$0, %eax	 #, _2
	addq	$48, %rsp	 #,
	popq	%rbp	 #
	ret	
	.seh_endproc
	.ident	"GCC: (Rev2, Built by MSYS2 project) 12.1.0"
