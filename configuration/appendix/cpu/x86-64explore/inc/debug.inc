; debug.inc
; Copyright (c) 2009-2012 ��־
; All rights reserved.

%ifndef DEBUG_INC
%define DEBUG_INC


%define	LBR_BIT					0
%define	BTF_BIT					1
%define TR_BIT					6
%define BTS_BIT					7
%define BTINT_BIT				8


;; ���� IA32_DEBUGCTL �Ĵ����Ĺ���λλ
%define LBR                             01h
%define BTF                             02h
%define TR                              40h
%define BTS                             80h
%define BTINT                           100h
%define BTS_OFF_OS                      200h
%define BTS_OFF_USR                     400h
%define FREEZE_LBRS_ON_PMI              800h
%define FREEZE_PERFMON_ON_PMI           1000h
%define UNCORE_PMI_EN                   2000h
%define FREEZE_WHILE_SMM_EN             4000h


;*
;* ���� DS save �������ַ
;* ȱʡ�������ʹ�� 400000h ����
;*
%define DS_SAVE_BASE			0x400000
%define BTS_BUFFER_BASE			0x400100
%define PEBS_BUFFER_BASE		0x400400

;* 
;* ���� BTS �� PEBS ����¼��(maximum �� thresholdֵ��
;* ȱʡ����£��������� 10 ����¼
;*
%define BTS_RECORD_MAXIMUM			10
%define BTS_RECORD_INT_THRESHOLD		(BTS_RECORD_MAXIMUM)
%define BTS_RECORD_CIRCULAR_THRESHOLD		(BTS_RECORD_MAXIMUM + 1)
%define PEBS_RECORD_MAXIMUM			10
%define PEBS_RECORD_THRESHOLD			(PEBS_RECORD_MAXIMUM)


;* 
;* ���� 32 λ��ʽ�µļ�¼ size
;* 
%define BTS_RECORD_SIZE                 12
%define PEBS_RECORD_SIZE                40

;*
;* ���� 64 λ��ʽ�µļ�¼ size
;*
%define BTS_RECORD64_SIZE               24
%define PEBS_RECORD64_SIZE              144

;*
;* ������ǿ��ʽ�µ� PEBS ��¼ size
;*
%define PEBS_ENHANCEMENT_RECORD64_SIZE  176



;*
;* ���� 32 λ��ʽ�� BTS ��������¼ offset λ��
;*
%define	BTS_BASE                        0
%define BTS_INDEX                       4
%define BTS_MAXIMUM                     8
%define BTS_THRESHOLD                   0CH

;*
;* ���� 64 λ��ʽ BTS ������
;*
%define	BTS64_BASE                      0
%define BTS64_INDEX                     8
%define BTS64_MAXIMUM                   10H
%define BTS64_THRESHOLD                 18H

;*
;* ���� 32 λ��ʽ�� PEBS ��������¼ offset λ��
;*
%define PEBS_BASE                       10H
%define PEBS_INDEX                      14H
%define PEBS_MAXIMUM                    18H
%define PEBS_THRESHOLD                  1CH
%define PEBS_COUNTER0                   20H
%define PEBS_COUNTER1                   24H
%define PEBS_COUNTER2                   28H
%define PEBS_COUNTER3                   2CH

;*
;* ���� 64 λ��ʽ�� PEBS ������
;*
%define PEBS64_BASE                     20H
%define PEBS64_INDEX                    28H
%define PEBS64_MAXIMUM                  30H
%define PEBS64_THRESHOLD                38H
%define PEBS64_COUNTER0                 40H
%define PEBS64_COUNTER1                 48H
%define PEBS64_COUNTER2                 50H
%define PEBS64_COUNTER3                 58H



;-----------------------------------------------------------
; ENABLE_DEBUGCTL �꣺���� IA32_DEBUGCTL �Ĵ����Ĺ���
; input:
;       �����ǹ���λ
; ʾ��:
;       ENABLE_DEBUGCTL (TR | BTS | BTINT)������ BTS �� BTINT ����
;------------------------------------------------------------
%macro ENABLE_DEBUGCTL 1
        mov ecx, IA32_DEBUGCTL
        rdmsr
        or eax, %1              ; ����ֵ
        wrmsr
%endmacro


;-------------------------------------------------------------
; DISABLE_DEBUGCTL �꣺�ر� IA32_DEBUGCTL ����
; input:
;       �����ǹ���λ
; ʾ��:
;       DISABLE_DEBUGCTL (TR | BTINT): �ر� TR �� BTINT ����
;-------------------------------------------------------------
%macro DISABLE_DEBUGCTL 1
        mov ecx, IA32_DEBUGCTL
        rdmsr
        and eax, (0FFFFFFFFh) ^ (%1)
        wrmsr
%endmacro



;-----------------------------------------
; ENABLE_LBR() �꣺���� LBR
;-----------------------------------------
%macro ENABLE_LBR 0
        ENABLE_DEBUGCTL (LBR)
%endmacro


;-----------------------------------
; ENABLE_BTS() �꣺���� bts, ��״��· 
;-----------------------------------
%macro ENABLE_BTS 0
        ENABLE_DEBUGCTL (TR | BTS)	
%endmacro

;-------------------------------------------------
; ENABLE_BTS_FREEZE_PERFMON_ON_PMI ��
; ����:
;       ���� BTS ��ʹ�� freeze_perfmon_on_pmi ����
;--------------------------------------------------
%macro ENABLE_BTS_FREEZE_PERFMON_ON_PMI 0
        ENABLE_DEBUGCTL (TR | BTS | FREEZE_PERFMON_ON_PMI)
%endmacro


;--------------------------------------------
; ENABLE_BTS_BTINT() �꣺���� bts, ����ʹ�� BTINT
;--------------------------------------------
%macro ENABLE_BTS_BTINT 0
        ENABLE_DEBUGCTL (TR | BTS | BTINT)
%endmacro


;--------------------------------------------
; ENABLE_BTS_BTINT_FREEZE_PERFMON_ON_PMI
;--------------------------------------------
%macro ENABLE_BTS_BTINT_FREEZE_PERFMON_ON_PMI 0
        ENABLE_DEBUGCTL (TR | BTS | BTINT | FREEZE_PERFMON_ON_PMI)
%endmacro


;-----------------------------------------
; DISABLE_LBR() �꣺�ر� LBR
;-----------------------------------------
%macro DISABLE_LBR 0
        DISABLE_DEBUGCTL (LBR)
%endmacro


;-------------------------------����������������------------
; DISABLE_TR(): ��� TR �ǿ����͹ر� TR��ͬʱ BTS ����Ҳ��Ч
;----------------------------------------------------------
%macro DISABLE_TR 0
	mov ecx, IA32_DEBUGCTL
	rdmsr
	bt eax, 6			; TR λ
	mov edi, eax,
	mov esi, 0FFBFh			; TR = 0
	cmovc edi, esi
	and eax, edi
	wrmsr
%endmacro



;--------------------------
; DISABLE_BTS() �꣺�ر� bts
;---------------------------
%macro DISABLE_BTS 0
        DISABLE_DEBUGCTL (TR | BTS)
%endmacro


;--------------------------------------------
; BIDSABLE_BTS_FREEZE_PERFMON_ON_PMI
;--------------------------------------------
%macro DISABLE_BTS_FREEZE_PERFMON_ON_PMI 0
        DISABLE_DEBUGCTL (TR | BTS | FREEZE_PERFMON_ON_PMI)
%endmacro

;--------------------------------------------
; DISABLE_BTS_BTINT() �꣺�ر� bts
;--------------------------------------------
%macro DISABLE_BTS_BTINT 0
        DISABLE_DEBUGCTL (TR | BTS | BTINT)
%endmacro


;--------------------------------------------
; DISABLE_BTS_BTINT_FREEZE_PERFMON_ON_PMI
;--------------------------------------------
%macro DISABLE_BTS_BTINT_FREEZE_PERFMON_ON_PMI 0
        DISABLE_DEBUGCTL (TR | BTS | BTINT | FREEZE_PERFMON_ON_PMI)
%endmacro


;------------------------------------------
; �� SET_DS_MANAGEMENT() �������� DS ������
;------------------------------------------
%macro SET_DS_MANAGEMENT 0
	mov esi, BTS_BUFFER_BASE		; BTS buffer ��ַ
	mov edi, PEBS_BUFFER_BASE		; PEBS buffer ��ַ
	call set_ds_management_record
%endmacro


;---------------------------------------------------
; �� SET_DS_AREA() 
; ������
;	�������������� DS ���򣨻��λ�· buffer ���ͣ�
;----------------------------------------------------
%macro SET_DS_AREA 0
	;;; ���� IA32_DS_AERA �Ĵ���
	call set_debug_store_area	

	;;; ���� DS ��������
	SET_DS_MANAGEMENT
%endmacro


;------------------------------------------------
; �� SET_INT_DS_AREA()
; ������
;	�������õ� buffer ��ʱ���� DS buffer �ж�
;------------------------------------------------
%macro SET_INT_DS_AREA 0
	SET_DS_AREA				; ������Ϊ���λ�· buffer ����

	; ���������� threshold ֵ���� maximum ֵ
	; ���ﵽ buffer ��ʱ���� DS �ж�
	mov esi, [bts_maximum_pointer]		; �� BTS maximum ��ַ
	mov esi, [esi]				; �� BTS maximum ֵ
	mov edi, [bts_threshold_pointer]	; �� BTS threshold ��ַ
	mov [edi], esi				; �� BTS threshold ���� BTS maximum ֵ
%endmacro




;---------------------------------------------------
; �� SET_DS_AREA64() 
; ������
;	�������������� DS ���򣨻��λ�· buffer ���ͣ�
;----------------------------------------------------
%macro SET_DS_AREA64 0
	;;; ���� IA32_DS_AERA �Ĵ���
	call set_debug_store_area	

	;;; ���� DS ��������
	mov rsi, BTS_BUFFER_BASE
	mov rdi, PEBS_BUFFER_BASE
	call set_ds_management_record
%endmacro

;------------------------------------------------
; �� SET_INT_DS_AREA64()
; ������
;	�������õ� buffer ��ʱ���� DS buffer �ж�
;------------------------------------------------
%macro SET_INT_DS_AREA64 0
	SET_DS_AREA64				; ������Ϊ���λ�· buffer ����

	; ���������� threshold ֵ���� maximum ֵ
	; ���ﵽ buffer ��ʱ���� DS �ж�
	mov rsi, [bts_maximum_pointer]		; �� BTS maximum ��ַ
	mov rsi, [rsi]				; �� BTS maximum ֵ
	mov rdi, [bts_threshold_pointer]	; �� BTS threshold ��ַ
	mov [rdi], rsi				; �� BTS threshold ���� BTS maximum ֵ
%endmacro


;------------------------------------------------------------
; �� DUMP_DS_MANAGEMENT() ������ӡ BTS ������
; ʹ��������ȹر� BTS �����¼ dump_ds_managment()������֧
;------------------------------------------------------------
%macro DUMP_DS_MANAGEMENT 0
	mov ecx, IA32_DEBUGCTL
	rdmsr
	mov esi, 0FF3Fh			; BTS=0, TR=0
	mov edi, eax
	push eax
	bt eax, 7			; ���� BTS λ
	cmovc edi, esi
	and eax, edi
	wrmsr
	call dump_ds_management
	; �ָ�ԭ����
	pop eax
	mov edx, 0
	mov ecx, IA32_DEBUGCTL
	wrmsr
%endmacro

;------------------------------------------------------------
; �� DUMP_BTS_RECORD() ������ӡ BTS buffer ��¼
; ʹ��������ȹر� BTS �����¼ dump_bts_record() ������֧
;------------------------------------------------------------
%macro DUMP_BTS_RECORD 0
	mov ecx, IA32_DEBUGCTL
	rdmsr
	mov esi, 0FF3Fh			; BTS=0, TR=0
	mov edi, eax
	push eax
	bt eax, 7			; ���� BTS λ
	cmovc edi, esi
	and eax, edi
	wrmsr
	call dump_bts_record
	; �ָ�ԭ����
	pop eax
	mov edx, 0
	mov ecx, IA32_DEBUGCTL
	wrmsr
%endmacro

;------------------------------------------------------------
; �� DUMP_BTS() ������ӡ DS �������� BTS buffer ��¼
; ʹ��������ȹر� BTS �����¼������֧
;------------------------------------------------------------
%macro DUMP_BTS 0
	mov ecx, IA32_DEBUGCTL
	rdmsr
	mov esi, 0FF3Fh			; BTS=0, TR=0
	mov edi, eax
	push eax
	bt eax, 7			; ���� BTS λ, BTS �����Ƿ���
	cmovc edi, esi			; ��������͹ر���
	and eax, edi
	wrmsr
	call dump_ds_management
	call dump_bts_record
	; �ָ�ԭ����
	pop eax
	mov edx, 0
	mov ecx, IA32_DEBUGCTL
	wrmsr
%endmacro

;------------------------------------------------------------
; �� DUMP_BTS64() ������ӡ DS �������� BTS buffer ��¼
;------------------------------------------------------------
%macro DUMP_BTS64 0
	call dump_ds_management
	call dump_bts_record
%endmacro


;------------------------------------------------------------
; �� DUMP_PEBS() ������ӡ PEBS �������� buffer ��¼
; ʹ��������ȹر� BTS �����¼������֧
;------------------------------------------------------------
%macro DUMP_PEBS 0
	mov ecx, IA32_DEBUGCTL
	rdmsr
	mov esi, 0FF3Fh			; BTS=0, TR=0
	mov edi, eax
	push eax
	bt eax, 7			; ���� BTS λ
	cmovc edi, esi
	and eax, edi
	wrmsr
	call dump_ds_management
	call dump_pebs_record
	; �ָ�ԭ����
	pop eax
	mov edx, 0
	mov ecx, IA32_DEBUGCTL
	wrmsr
%endmacro

%endif