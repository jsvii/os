; hpet.inc
; Copyright (c) 2009-2012 mik 
; All rights reserved.


;*
;* Ϊ High Precision Event Timer ����������
;*

%ifndef HPET_INC
%define HPET_INC


;*
;* ����߾��ȶ�ʱ���Ļ�ַ
;* ����һ��һӳ�䷽ʽ��
;* �����ַ 0FED00000h ӳ�䵽������ַ 0FED00000h
;
%define HPET_BASE                       0FED00000h

;*
;* ���� HPET �ļĴ�����ַ
;*
%define HPET_COUNTER                    0FED000F0h

%define HPET_TIMER0_CONFIG              0FED00100h
%define HPET_TIMER1_CONFIG              0FED00120h
%define HPET_TIMER2_CONFIG              0FED00140h
%define HPET_TIMER3_CONFIG              0FED00160h
%define HPET_TIMER4_CONFIG              0FED00180h
%define HPET_TIMER5_CONFIG              0FED001A0h
%define HPET_TIMER6_CONFIG              0FED001C0h
%define HPET_TIMER7_CONFIG              0FED001E0h

%define HPET_TIMER0_COMPARATOR          0FED00108h
%define HPET_TIMER1_COMPARATOR          0FED00128h
%define HPET_TIMER2_COMPARATOR          0FED00148h
%define HPET_TIMER3_COMPARATOR          0FED00168h
%define HPET_TIMER4_COMPARATOR          0FED00188h
%define HPET_TIMER5_COMPARATOR          0FED001A8h
%define HPET_TIMER6_COMPARATOR          0FED001C8h
%define HPET_TIMER7_COMPARATOR          0FED001E8h



;-----------------------------------------------+
;  Ϊ HPET ���� count ֵ                        |
;                                               |
;  HPET �������Ƶ���м�����                    |
;       1) 1��          = Լ 14318179 ��        |
;       2) 1����(ms)    = Լ 14318 ��           |
;       3) 1΢��(us)    = Լ 14 ��              |
;       4) 1����(ns)    = Լ 0.014 ��           |
;-----------------------------------------------+

%define HPET_COUNT_SECOND               14318179                ; 1 �� count ֵ
%define HPET_COUNT_MILLISECOND          14318                   ; 1 ���� count ֵ
%define HPET_COUNT_MICROSECOND          14                      ; 1 ΢�� count ֵ



;----------------------------------------------------
; �� START_HPET_TIMER()��������ʱ��
; input:
;       %1 - timer
;       %2 - ʱ�䵥λ
;       %3 - ��λֵ
;
; ʾ����
;       START_HPET_TIMER  0, HPET_COUNT_MILLISECOND, 100
;--------------------------------------------------------
%macro START_HPET_TIMER 3
        bts DWORD [HPET_BASE + 100h + (20h * %1)], 2
        mov DWORD [HPET_BASE + 108h + (20h * %1) + 4], 0
        mov DWORD [HPET_BASE + 108h + (20h * %1)], (%2 * %3)
        mov DWORD [HPET_COUNTER + 4], 0
        mov DWORD [HPET_COUNTER], 0
%endmacro



;----------------------------------------------------
; �� STOP_HPET_TIMER()���رն�ʱ��
; input:
;       %1 - timer
;
; ʾ����
;       STOP_HPET_TIMER  0
;--------------------------------------------------------
%macro STOP_HPET_TIMER 1
        btr DWORD [HPET_BASE + 100h + (20h * %1)], 2
%endmacro



;------------------------------------------
; ʹ�� HPET timer 1 ������ʱ
; ������ʹ��΢�뼶��
;------------------------------------------
%macro DELAY_WITH_US 1
        mov esi, [HPET_TIMER1_CONFIG]
        btr DWORD [HPET_TIMER1_CONFIG], 2
        mov DWORD [HPET_TIMER1_COMPARATOR + 4], 0
        mov DWORD [HPET_TIMER1_COMPARATOR], (%1 * HPET_COUNT_MICROSECOND)
        mov DWORD [HPET_COUNTER + 4], 0
        mov DWORD [HPET_COUNTER], 0

%%1:
        mov eax, [HPET_COUNTER]
        mov edi, [HPET_TIMER1_COMPARATOR]
        cmp eax, edi
        jb %%1
        mov [HPET_TIMER1_CONFIG], esi
%endmacro

;------------------------------------------
; ʹ�� HPET timer 1 ������ʱ
; ������ʹ�ú��뼶��
;------------------------------------------
%macro DELAY_WITH_MS 1
        mov esi, [HPET_TIMER1_CONFIG]
        btr DWORD [HPET_TIMER1_CONFIG], 2
        mov DWORD [HPET_TIMER1_COMPARATOR + 4], 0
        mov DWORD [HPET_TIMER1_COMPARATOR], (%1 * HPET_COUNT_MILLISECOND)
        mov DWORD [HPET_COUNTER + 4], 0
        mov DWORD [HPET_COUNTER], 0

%%1:
        mov eax, [HPET_COUNTER]
        mov edi, [HPET_TIMER1_COMPARATOR]
        cmp eax, edi
        jb %%1
        mov [HPET_TIMER1_CONFIG], esi
%endmacro


%endif