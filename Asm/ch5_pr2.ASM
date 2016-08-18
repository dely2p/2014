main    segment
        assume cs:main
abc:    mov     ax,0b800h
        mov     ds,ax
        mov     di,(24*80+79)*2
        mov     ah,42h
        mov     al,'U'
        mov     [di],ax
        mov     ah,4ch
        int     21h
main    ends
        end     abc
