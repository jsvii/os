;************************************************
;* perfmon.inc                                  *
;* Copyright (c) 2009-2012 ��־                 *
;* All rights reserved.                         *
;*                                              *
;* �˴�������ѧϰ���о�������������ҵ��;!      *
;* ��������Ȩ��! 2009-2012                      *
;* ����ѧϰ�򱨸� bug: mik@mouseo.com           *
;************************************************


;
; ���� performance monitior ��Ԫͷ�ļ� 
;

%ifndef PERFMON_INC
%define PERFMON_INC



;*
;* ʹ��Ԥ�������¼����������ָ��ִ������
;* IA32_PERFEVTSELx �Ĵ���������Ϊ��EN=1, INT=1, USR=OS=1, umask=0, event select = c0
;*
%define UNHALTED_CORE_CYCLE_EVENT               53003Ch
%define INST_COUNT_EVENT                        5300C0h
%define UNHALTED_REFERENCE_CYCLE_EVENT          53013Ch
%define LLC_REFERENCE_EVENT                     534F2Eh
%define LLC_MISS_EVENT                          53412Eh
%define BR_INST_COUNT_EVENT                     5300C4h
%define BR_MISS_COUNT_EVENT                     5300C5h


;*
;* ���� Westmere �ܹ��ϵ� PEBS �¼�
;*
%define PEBS_INST_COUNT_EVENT                   5300C0h
%define PEBS_X87_OPS_COUNT_EVENT                53FEC1h
%define PEBS_BR_MISS_COUNT_EVENT                5300C5h
%define PEBS_SIMD_INST_COUNT_EVENT              531FC7h
%define PEBS_MEM_LOAD_L1D_MISS_EVENT            5301CBh
%define EPBS_MEM_LOAD_L1D_LINE_MISS_EVENT       5302CBh
%define PEBS_MEM_LOAD_L2_MISS_EVENT             5304CBh
%define PEBS_MEM_LOAD_L2_LINE_MISS_EVENT        5308CBh
%define PEBS_MEM_LOAD_DTLB_MISS_EVENT           5310CBh


;* ���� load latency �¼�
%define PEBS_MEM_INST_COUNT_EVENT               53100Bh


;*
;* ���涨�� IA32_PERF_GLOBAL_CTRL �Ĵ��� enable λ
;*
%define IA32_PMC0_EN                            01h
%define IA32_PMC1_EN                            02h
%define IA32_PMC2_EN                            04h
%define IA32_PMC3_EN                            08h
%define IA32_FIXED_CTR0_EN                      01h
%define IA32_FIXED_CTR1_EN                      02h
%define IA32_FIXED_CTR2_EN                      04h


;*
;* ���涨�� IA32_PEBS_ENABLE �Ĵ��� enable λ
;*
%define PEBS_PMC0_EN                            01h
%define PEBS_PMC1_EN                            02h
%define PEBS_PMC2_EN                            04h
%define PEBS_PMC3_EN                            08h
%define LL_PMC0_EN                              01h
%define LL_PMC1_EN                              02h
%define LL_PMC2_EN                              04h
%define LL_PMC3_EN                              08h



;*
;* ������ܿ���
;*

;-------------------------------------------------------
; �� ENABLE_COUNTER������ counter enable
; input:
;       %1 - ����1����Ҫ������ IA32_PMCx ������ 
;       %2 - ����2: ��Ҫ������ IA32_FIXED_CTRx ������
;
; ʾ����
;       ENABLE_COUNTER (IA32_PMC0_EN | IA32_PMC1_EN), 0
;-------------------------------------------------------
%macro ENABLE_COUNTER 2
        mov ecx, IA32_PERF_GLOBAL_CTRL
        rdmsr
        or eax, %1                      ; IA32_PMCx ������
        or edx, %2                      ; IA32_FIXED_CTRx ������
        wrmsr
%endmacro


;-------------------------------------------------------
; �� DISABLE_COUNTER���ر� counter enable
; input:
;       %1 - ����1����Ҫ�رյ� IA32_PMCx ������ 
;       %2 - ����2: ��Ҫ�رյ� IA32_FIXED_CTRx ������
;
; ʾ����
;       DISABLE_COUNTER (IA32_PMC0_EN | IA32_PMC1_EN), 0
;-------------------------------------------------------
%macro DISABLE_COUNTER 2
        mov ecx, IA32_PERF_GLOBAL_CTRL
        rdmsr
        and eax, (-1) ^ (%1)    ; IA32_PMCx ��������
        and edx, (-1) ^ (%2)    ; IA32_FIXED_CTRx ������
        wrmsr
%endmacro


;-----------------------------------------------------
; �� ENABLE_PEBS_COUNTER: ���� counter PEBS �ж�����
; input:
;       %1 - ����1����Ҫ������ IA32_PMCx ������
;       %2 - ����2: ��Ҫ������ load latency ����������
; ʾ����
;       ENABLE_PEBS_COUNTER (PEBS_PMC0_EN), 0
;-------------------------------------------------------
%macro ENABLE_PEBS_COUNTER 2
        mov ecx, IA32_PEBS_ENABLE
        rdmsr
        or eax, %1                      ; PEBS_PMCx_EN
        or edx, %2                      ; LL_PMCx_EN
        wrmsr
%endmacro


;-----------------------------------------------------
; �� DISABLE_PEBS_COUNTER: �ر� counter PEBS �ж�����
; input:
;       %1 - ����1����Ҫ�رյ� IA32_PMCx ������
;       %2 - ����2: ��Ҫ�رյ� load latency ����������
; ʾ����
;       DISABLE_PEBS_COUNTER (PEBS_PMC0_EN), 0
;-------------------------------------------------------
%macro DISABLE_PEBS_COUNTER 2
        mov ecx, IA32_PEBS_ENABLE
        rdmsr
        and eax, (-1) ^ (%1)
        and edx, (-1) ^ (%2)
        wrmsr
%endmacro



;-----------------------------------------
; ��ȫ�ֿ������￪�����еļ�����
;-----------------------------------------
%macro ENABLE_GLOBAL_COUNTER 0
        ENABLE_COUNTER 0Fh, 07h
%endmacro


;------------------------------------------
; �ر�����ȫ�ֿ������ļ�����
;------------------------------------------
%macro DISABLE_GLOBAL_COUNTER 0
        DISABLE_COUNTER 0Fh, 07h
%endmacro


%macro DISABLE_PEBS 0
        DISABLE_PEBS_COUNTER 0Fh, 0Fh
%endmacro


;*
;* ���涨�忪�� IA32_PMCx ��������
;*
%macro ENABLE_IA32_PMC0 0
        ENABLE_COUNTER (IA32_PMC0_EN), 0
%endmacro

%macro ENABLE_IA32_PMC1 0
        ENABLE_COUNTER (IA32_PMC1_EN), 0
%endmacro

%macro ENABLE_IA32_PMC2 0
        ENABLE_COUNTER (IA32_PMC2_EN), 0
%endmacro

%macro ENABLE_IA32_PMC3 0
        ENABLE_COUNTER (IA32_PMC3_EN), 0
%endmacro


;*
;*
;* ���涨��ر� IA32_PMCx ������
;*
%macro DISABLE_IA32_PMC0 0
        DISABLE_COUNTER (IA32_PMC0_EN), 0
%endmacro

%macro DISABLE_IA32_PMC1 0
        DISABLE_COUNTER (IA32_PMC1_EN), 0
%endmacro

%macro DISABLE_IA32_PMC2 0
        DISABLE_COUNTER (IA32_PMC2_EN), 0
%endmacro

%macro DISABLE_IA32_PMC3 0
        DISABLE_COUNTER (IA32_PMC3_EN), 0
%endmacro


;*
;* ���涨�忪�� IA32_FIXED_CTRx ������
;*
%macro ENABLE_IA32_FIXED_CTR0 0
        ;*
        ;* ������IA32_FIXED_CTR_CTRL�Ĵ���
        ;*
        mov ecx, IA32_FIXED_CTR_CTRL
        rdmsr
        and eax, 0FFFFFFF0h             ; ���� IA32_FIXED_CTR0
        or eax, 0Bh                     ; En=3(all ring levels), ANY=0, PMI=1
        wrmsr
        ENABLE_COUNTER  0, (IA32_FIXED_CTR0_EN)
%endmacro

%macro ENABLE_IA32_FIXED_CTR1 0
        mov ecx, IA32_FIXED_CTR_CTRL
        rdmsr
        and eax, 0FFFFFF0Fh             ; ���� IA32_FIXED_CTR1
        or eax, 0B0h                    ; En=3(all ring levels), ANY=0, PMI=1
        wrmsr
        ENABLE_COUNTER 0, (IA32_FIXED_CTR1_EN)
%endmacro

%macro ENABLE_IA32_FIXED_CTR2 0
        mov ecx, IA32_FIXED_CTR_CTRL
        rdmsr
        and eax, 0FFFFF0FFh             ; ���� IA32_FIXED_CTR2
        or eax, 0B00h                   ; En=3(all ring levels), ANY=0, PMI=1
        wrmsr
        ENABLE_COUNTER 0, (IA32_FIXED_CTR2_EN)
%endmacro


;* 
;* ���涨��ر� IA32_FIXED_CTRx ������
;*
%macro DISABLE_IA32_FIXED_CTR0 0
        DISABLE_COUNTER 0, (IA32_FIXED_CTR0_EN)
%endmacro

%macro DISABLE_IA32_FIXED_CTR1 0
        DISABLE_COUNTER 0, (IA32_FIXED_CTR1_EN)
%endmacro

%macro DISABLE_IA32_FIXED_CTR2 0
        DISABLE_COUNTER 0, (IA32_FIXED_CTR2_EN)
%endmacro


;*
;* ���涨�忪�� PEBS_ENABLE ��
;*
%macro ENABLE_PEBS_PMC0 0
        ENABLE_PEBS_COUNTER (PEBS_PMC0_EN), 0
%endmacro

%macro ENABLE_PEBS_PMC1 0
        ENABLE_PEBS_COUNTER (PEBS_PMC1_EN), 0
%endmacro

%macro ENABLE_PEBS_PMC2 0
        ENABLE_PEBS_COUNTER (PEBS_PMC2_EN), 0
%endmacro

%macro ENABLE_PEBS_PMC3 0
        ENABLE_PEBS_COUNTER (PEBS_PMC3_EN), 0
%endmacro


;*
;* ���� PEBS ��ʹ�� load latency ����
;*
%macro ENABLE_PEBS_WITH_LL_PMC0 0
        ENABLE_PEBS_COUNTER (PEBS_PMC0_EN), (LL_PMC0_EN)
%endmacro

%macro ENABLE_PEBS_WITH_LL_PMC1 0
        ENABLE_PEBS_COUNTER (PEBS_PMC1_EN), (LL_PMC1_EN)
%endmacro

%macro ENABLE_PEBS_WITH_LL_PMC2 0
        ENABLE_PEBS_COUNTER (PEBS_PMC2_EN), (LL_PMC2_EN)
%endmacro

%macro ENABLE_PEBS_WITH_LL_PMC3 0
        ENABLE_PEBS_COUNTER (PEBS_PMC3_EN), (LL_PMC3_EN)
%endmacro


;*
;* �ر� PEBS
;*
%macro DISABLE_PEBS_PMC0 0
        DISABLE_PEBS_COUNTER (PEBS_PMC0_EN), 0
%endmacro

%macro DISABLE_PEBS_PMC1 0
        DISABLE_PEBS_COUNTER (PEBS_PMC1_EN), 0
%endmacro

%macro DISABLE_PEBS_PMC2 0
        DISABLE_PEBS_COUNTER (PEBS_PMC2_EN), 0
%endmacro

%macro DISABLE_PEBS_PMC3 0
        DISABLE_PEBS_COUNTER (PEBS_PMC3_EN), 0
%endmacro



;*
;* �ر� PEBS �� load latency ����
;*
%macro DISABLE_PEBS_WITH_LL_PMC0 0
        DISABLE_PEBS_COUNTER (PEBS_PMC0_EN), (LL_PMC0_EN)
%endmacro

%macro DISABLE_PEBS_WITH_LL_PMC1 0
        DISABLE_PEBS_COUNTER (PEBS_PMC1_EN), (LL_PMC1_EN)
%endmacro

%macro DISABLE_PEBS_WITH_LL_PMC2 0
        DISABLE_PEBS_COUNTER (PEBS_PMC2_EN), (LL_PMC2_EN)
%endmacro

%macro DISABLE_PEBS_WITH_LL_PMC3 0
        DISABLE_PEBS_COUNTER (PEBS_PMC3_EN), (LL_PMC3_EN)
%endmacro


;-----------------------------------------------
; �� RESET_FIXED_PMC()
; ������
;       ������ IA32_FIXED_CTR��������ֵΪ 0
;-----------------------------------------------
%macro RESET_FIXED_PMC 0
        xor eax, eax
        xor edx, edx
        mov ecx, IA32_FIXED_CTR0
        wrmsr
        mov ecx, IA32_FIXED_CTR1
        wrmsr
        mov ecx, IA32_FIXED_CTR2
        wrmsr
%endmacro


;---------------------------------------------
; �� RESET_PMC()
; ����:
;       ������ IA32_PMC ��������ֵΪ0
;---------------------------------------------
%macro RESET_PMC 0
        xor eax, eax
        xor edx, edx
        mov ecx, IA32_PMC0
        wrmsr
        mov ecx, IA32_PMC1
        wrmsr
        mov ecx, IA32_PMC2
        wrmsr
        mov ecx, IA32_PMC3
        wrmsr
%endmacro


;########## ��������� PEBS �ж� handler �����ĺ궨�� ##############

;--------------------------------------------------------------
; �� RESET_COUNTER_OVERFLOW()
; ������
;       �� IA32_PERF_GLOBAL_STATUS �Ĵ������� overflow ��־λ
;-------------------------------------------------------------
%macro RESET_COUNTER_OVERFLOW 0
	mov ecx, IA32_PERF_GLOBAL_OVF_CTRL
	mov eax, 0Fh                    ; IA32_PMCx overflow
	mov edx, 07h                    ; IA32_FIXED_CTRx overflow
	wrmsr
%endmacro

;-------------------------------------------------------------
; �� RESET_PMI_COUNTER_OVERFLOW()
; ������
;       �崥�� PMI �жϵ� counter overflow
;       ȥ������ PEBS �жϵ� counter overflow
;-------------------------------------------------------------
%macro RESET_PMI_COUNTER_OVERFLOW 0
        mov ecx, IA32_PERF_GLOBAL_STATUS
        rdmsr
        mov esi, eax                    ; ���� overflow status
        mov ecx, IA32_PEBS_ENABLE
        rdmsr
        and eax, esi                    ; ȡ PEBS overflow status
        not eax
        and eax, esi                    ; ȡ PMI overflow status
        mov ecx, IA32_PERF_GLOBAL_OVF_CTRL
        mov edx, 0
        wrmsr
%endmacro


;--------------------------------------------------------------
; �� RESET_PEBS_BUFFER_OVERFLOW() 
; ������
;       �� IA32_PERF_GLOBAL_STATUS �Ĵ����� OvfBuffer λ
;--------------------------------------------------------------
%macro RESET_PEBS_BUFFER_OVERFLOW 0
        mov ecx, IA32_PERF_GLOBAL_OVF_CTRL
        mov eax, 0
        mov edx, 40000000h              ; OvfBuffer λ
        wrmsr
%endmacro




%endif
