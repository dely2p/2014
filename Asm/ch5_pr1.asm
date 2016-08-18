main    segment
        assume cs:main
abc:    mov     ax,0b800h
        mov     ds,ax
        mov     di,(0*80+79)*2
        mov     ah,4Eh
        mov     al,'C'
        mov     [di],ax
        mov     ah,4ch
        int     21h
main    ends
        end     abc
