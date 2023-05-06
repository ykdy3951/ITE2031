        lw      0       1       zero    load zero in reg 1 (result register)
        lw      0       2       mcand   
        lw      0       3       mplier
        lw      0       4       max
        lw      0       5       one     index register
loop    nor     3       3       3       1's complement to mplier
        nor     5       5       5       1's complement to index reg
        nor     5       3       6
        nor     3       3       3       restore mplier
        nor     5       5       5       restore reg 5 (index register)
        beq     0       6       if      if bit is zero, goto if label
        add     2       1       1       if bit is not zero add mcand to return register
if      add     2       2       2       left shift mcand
        add     5       5       5       left shift reg 5 (index reg)
        beq     5       4       done
        beq     0       0       loop
done    halt                            end of program
mcand   .fill   32766
mplier  .fill   12328
one     .fill   1
zero    .fill   0
max     .fill   32768
