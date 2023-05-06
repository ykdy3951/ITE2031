        lw      0       1       minus
        lw      0       2       target     load target address to reg2
        jalr    2       3                  store pc+1 to reg3 then goto reg2
        halt                               exit
        noop
L2      jalr    3       2                  store pc+1 to reg2 then goto reg3
L1      beq     0       0       L2         goto L2
minus   .fill   -1
target  .fill   L1
