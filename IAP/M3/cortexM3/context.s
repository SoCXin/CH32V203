;/*
; * File      : context.S
; *
; */


	AREA |.text|, CODE, READONLY, ALIGN = 2
    THUMB
    REQUIRE8
    PRESERVE8

;/*
; * base_t hw_interrupt_disable();
; */
hw_interrupt_disable    PROC
    EXPORT  hw_interrupt_disable
    MRS     r0, PRIMASK
    CPSID   I
    BX      LR
    ENDP

;/*
; * void hw_interrupt_enable(rt_base_t level);
; */
hw_interrupt_enable    PROC
    EXPORT  hw_interrupt_enable
    MSR     PRIMASK, r0
    BX      LR
    ENDP
	
	END