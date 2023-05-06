        lw      0       2       ten     load #loop in register 2
        lw      0       3       one     load increment value in reg 3
        lw      0       4       one     load one in reg 4
start   beq     1       2       done    if reg 1 equals to reg 2 then halt
        sw      1       4       arr0    save reg 4 in arr[reg1]
        add     4       4       4       left shift register 4
        add     1       3       1       increment reg 1
        beq     0       0       start   goto start
done    halt
one     .fill   1
ten     .fill   10
arr0    .fill   0
arr1    .fill   0
arr2    .fill   0
arr3    .fill   0
arr4    .fill   0
arr5    .fill   0
arr6    .fill   0
arr7    .fill   0
arr8    .fill   0
arr9    .fill   0
