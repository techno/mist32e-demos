 	.globl	_start
	.align	4
	.section .init
_start:
	lih	r0, hi(0x40000)
	wl16	r0, lo(0x40000)
	srspw	r0
	movepc	rret, 8
	br	main, #al
	br	0, #al
