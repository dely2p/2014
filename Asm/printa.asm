.radix 16
main    segment
        mov     ah,0
        int     16
        mov     dl,'A'
        mov     ah,2
        int     21
        mov     ah,4c
        int     21
main    ends
        end
