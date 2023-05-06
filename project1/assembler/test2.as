        lw      0       2       ten     loop end value
        lw      0       3       one     increment value
loop    beq     1       2       exit    if value of reg 1 is same as value of reg 2 then exit        
        add     4       1       4       add
        add     1       3       1       increment reg 1          
        beq     0       0       loop    go to loop
exit    sw      0       4       accum   save the return value in accum label 
        halt
zero    .fill   0
one     .fill   1
ten     .fill   10
accum   .fill   0
