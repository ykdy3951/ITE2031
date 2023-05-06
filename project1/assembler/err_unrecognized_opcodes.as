        hello   0       1       two     ERROR: Unrecognize opcodes
        lw      1       2       neg1    
start   add     1       2       1       
        beq     0       1       2       
        beq     0       0       start
done    halt      
two     .fill   2
neg1    .fill   -1
