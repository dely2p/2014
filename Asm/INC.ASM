.radix 16
main    segment
        mov     ah,0	; 키보드 입력받기
        int     16
        inc     al	; 키보드 값 증가시키기
        mov     dl,al	; 그 문자를 화면에 출력하기
        mov     ah,2
        int     21
        mov     ah,4c	; 종료하기
        int     21
main    ends
        end