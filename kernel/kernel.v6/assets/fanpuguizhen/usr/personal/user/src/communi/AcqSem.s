#int AcquireSemphore (int sd)
#{
.globl _AcquireSemphore
_AcquireSemphore:
	mov	r5,-(sp)
	mov	sp,r5
	mov	4(r5),r0
	sys	52
	bec	1f
	jmp	cerror
1:
	clr r0
	mov	(sp)+,r5
	rts	pc
#}
