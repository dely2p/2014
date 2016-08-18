.radix 16
main    segment
        mov     dl,'?'
        mov     ah,2    
        int     21
        mov     ah,0       //dfda
        int     16
        mov     dl,al
        mov     ah,2
        int     21
        mov     ah,4c
        int     21
main    ends
        end
