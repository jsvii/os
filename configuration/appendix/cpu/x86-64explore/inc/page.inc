; page.inc
; Copyright (c) 2009-2012 mik 
; All rights reserved.


%ifndef PAGE_INC
%define PAGE_INC


;*** 32-bit paging �� ***
%define PDT32_BASE              100000h

;*** PAE paging �� ***
%define PDPT_BASE               100000h


; IA-32e paging ��
%define PML4T_BASE              200000h
%define INIT_SEG                0FFFFFFF800000000h


;; ���� table entry λ

%define P                       1
%define RW                      2
%define US                      4
%define PWT                     8
%define PCD                     10h
%define A                       20h
%define D                       40h
%define PS                      80h
%define G                       100h
%define PAT                     1000h
%define XD                      80000000h


;; δ������ҳʱ user stack
%define USER_ESP                08FF0h

;; Ϊÿ�� logical processor �����Լ��� stack pointer
PROCESSOR0_ESP                        equ        0x3ff000
PROCESSOR1_ESP                        equ        0x3fe000
PROCESSOR2_ESP                        equ        0x3fd000
PROCESSOR3_ESP                        equ        0x3fc000


;* ÿ��������ʹ�õ� stack ȱʡΪ 2K ��С
%define PROCESSOR_STACK_SIZE            800h


;** ������ҳ�� ***
; 32λ����ģʽ�µ� kernel stack

;--------------------------------------------------------------------------------------------------------
; Ϊÿ�� logical ������������� kernel stack  pointer
; ���䷽�������ݴ����� index ֵ�����䣬ÿ��������ȱʡʹ�� 2K stack
; ���磺
;      processor #0 = PROCESSOR_KERNEL_ESP + PROCESSOR_STACK_SIZE + (PROCESSOR_STACK_SIZE * 0)
;      processor #1 = PROCESSOR_KERNEL_ESP + PROCESSOR_STACK_SIZE + (PROCESSOR_STACK_SIZE * 1)
;      processor #2 = PROCESSOR_KERNEL_ESP + PROCESSOR_STACK_SIZE + (PROCESSOR_STACK_SIZE * 2)
;      processor #3 = PROCESSOR_KERNEL_ESP + PROCESSOR_STACK_SIZE + (PROCESSOR_STACK_SIZE * 3)
;------------------------------------------------------------------------------------------------------------
%define PROCESSOR_KERNEL_ESP            0FFE00800h

; �����ж� handler �� RSP
%define PROCESSOR_IDT_ESP               0FFE04800h

; user stack
%define PROCESSOR_USER_ESP              7FE00800h


%ifdef NON_PAGING
        %define USER_ESP                08FF0h
        %define KERNEL_ESP              3FF000h
%else
        %define USER_ESP                (PROCESSOR_USER_ESP)
        %define KERNEL_ESP              (PROCESSOR_KERNEL_ESP)
%endif



; 64-bit ģʽ�µ�
%define SYSTEM_DATA64_BASE              0FFFFFFF800000000h

; ����ʹ���� compatibility ģʽ��
%define COMPATIBILITY_USER_ESP          018FF0h
%define COMPATIBILITY_KERNEL_ESP        0FFE003F0h
%define LIB32_ESP                       0FFE01FF0h


; 64-bit ģʽ�µ� kernel stack
;
%define PROCESSOR_KERNEL_RSP            0FFFFFFFFFFE00800h
%define PROCESSOR0_KERNEL_RSP           0FFFFFFFFFFE00800h

; �����ж� handler �� RSP
%define PROCESSOR_IDT_RSP               0FFFFFFFFFFE04800h
%define PROCESSOR0_IDT_RSP              0FFFFFFFFFFE04800h

; ���� sysenter ָ�� RSP
%define PROCESSOR_SYSENTER_RSP          0FFFFFFFFFFE08800h


; ���� syscall ָ�� RSP
%define PROCESSOR_SYSCALL_RSP           0FFFFFFFFFFE0C800h


; ���� IST �� RSP ֵ
%define PROCESSOR_IST1_RSP              0FFFFFFFFFFE10800h
%define PROCESSOR0_IST1_RSP             0FFFFFFFFFFE10800h


; 64-bit ģʽ�µ� user stack
%define PROCESSOR_USER_RSP              00007FFFFFE00800h



%define USER_RSP                        (PROCESSOR_USER_RSP)
%define KERNEL_RSP                      (PROCESSOR_KERNEL_RSP)


%endif