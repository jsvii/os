; float.inc
; Copyright (c) 2009-2012 ��־
; All rights reserved.


%ifndef FLOAT_INC
%define FLOAT_INC

;;
;; ���� float ����س���



%define FLT_MAX                 0x7F7FFFFF                      ; �����ȵ����ֵ
%define FLT_MIN                 0x00800000                      ; ��������Сֵ
%define DBL_FLT_MAX             0x7FEFFFFFFFFFFFFF              ; ˫���ȵ����ֵ
%define DBL_FLT_MIN             0x0018000000000000              ; ˫������Сֵ


;%define FLT_MAX                 3.402823466e+38                ; ���������ֵ
;%define FLT_MIN                 1.175494351e-38                ; ��������Сֵ
;%define DBL_FLT_MAX             1.7976931348623158e+308         ; ˫�������ֵ
;%define DBL_FLT_MIN             2.2250738585072014e-308         ; ˫������Сֵ

;��չ˫�������ֵ
%macro DBE_FLAT_MAX 0
        0xFFFFFFFF
        dd 0xFFFFFFFF
        dw 0x7FFE
%endmacro
%define DBE_FLAT_MIN            0x1p-16382                      ; ��չ˫������Сֵ

%define Inf                     __Infinity__ 
%define NaN                     __QNaN__ 
%define QNaN                    __QNaN__ 
%define SNaN                    __SNaN__

;%define SNaN                    0x7fb00000
;%define QNaN                    0x7ff00000
;%define NaN                     QNaN
;%define Inf                     0x7f000000

; ����80λ denormal ��
%macro DENORMAL80 0
        1                       ; significand Ϊ 1 ֵ
        dw 0                    ; exponent Ϊ 0
%endmacro

; ���� 80λ unsupported ��
%macro UNSUPPORTED80 0
        0                       ; significand Ϊ 0 ֵ
        dw 0x7FFFF              ; exponent Ϊ���
%endmacro

%endif
