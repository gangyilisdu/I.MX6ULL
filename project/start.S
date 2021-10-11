
.global _start  		/* 全局标号 */


_start: 
	ldr pc, = Reset_Handler
	ldr pc, = Undefined_Handler
	ldr pc, = SVC_Handler
	ldr pc, = PreAbort_Handler
	ldr pc, = DataAbort_Handler
	ldr pc, = NotUsed_Handler
	ldr pc, = IRQ_Handler
	ldr pc, = FIQ_Handler

Reset_Handler:

	cpsid i
	
	mrc p15, 0, r0, c1, c0, 0
	bic r0, r0, #(1<<12)
	bic r0, r0, #(1<<11)
	bic r0, r0, #(1<<2)
	bic r0, r0, #(1<<1)
	bic r0, r0, #(1<<0)
	mcr p15, 0, r0, c1, c0, 0

	ldr r0, =0x87800000
	dsb
	isb
	mrc p15, 0, r0, c12, c0, 0
	dsb
	isb
	
	/*SYS*/
	mrs r0, cpsr
	bic r0, r0, #0x1f
	orr r0, r0, #0x1f
	msr cpsr, r0
	ldr sp, =0x80600000

	/*IRQ*/
	mrs r0, cpsr
	bic r0, r0, #0x1f
	orr r0, r0, #0x12
	msr cpsr, r0
	ldr sp, =0x80400000

	/*SVC*/
	mrs r0, cpsr
	bic r0, r0, #0x1f
	orr r0, r0, #0x13
	msr cpsr, r0
	ldr sp, =0x80200000
	
	cpsie i
	b main


Undefined_Handler:
	ldr r0, = Undefined_Handler
	bx r0

SVC_Handler:
	ldr r0, = SVC_Handler
	bx r0

PreAbort_Handler:
	ldr r0, = PreAbort_Handler
	bx r0

DataAbort_Handler:
	ldr r0, = DataAbort_Handler
	bx r0

NotUsed_Handler:
	ldr r0, = NotUsed_Handler
	bx r0

IRQ_Handler:
	push {lr}
	push {r0-r3, r12}
	
	mrs r0, spsr
	push {r0}

	mrc p15, 4, r1, c15, c0, 0
	add r1, r1, #0x2000
	ldr r0, [r1, #0xc]

	push {r0, r1}

	cps #0x13

	push {lr}
	ldr r2, =system_irqhandler
	blx r2

	pop {lr}
	cps #0x12
	pop {r0, r1}
	str r0, [r1, #0x10]

	pop {r0}
	msr spsr_cxsf, r0
	
	pop {r0-r3, r12}
	pop {lr}
	subs pc, lr, #4

FIQ_Handler:
	ldr r0, = FIQ_Handler
	bx r0 



