        lw      0       2       five    load #loop in register 2
        lw      0       3       one     load increment value in reg 3
start   beq     1       2       done    if reg 1 equals to reg 2 then halt
        sw      1       1       arr0    save reg 1 in arr[reg1] 
        add     1       3       1       increment reg 1
        beq     0       0       start   goto start
done    halt
one     .fill   1
five    .fill   5
arr0    .fill   0
arr1    .fill   0
arr2    .fill   0
arr3    .fill   0
arr4    .fill   0
