#int UnmapSharedMemory(int shd)
#{
.globl _UnmapSharedMemory
_UnmapSharedMemory:
	mov	r5,-(sp)
	mov	sp,r5
	mov	4(r5),r0
	sys	72
	bec	1f
	jmp	cerror
1:
	clr r0
	mov	(sp)+,r5
	rts	pc
#}
