; happy path, tests comments, labels and instructions
        lw x1,x2,4
1:
666: ; 2nd label at this address
        sw x2,0,x3
        bne x3,x4,1b
loop:
        lw x1,x2,0
        addi x1,x2,3
        beq x1,x2,loop
        beq x1,x2,128
1:      beq x1,x2,8 ; whole set: label instruction comment ;;; The quick brown fox jumps over the lazy dog;
1:      beq x1,x2,1b ; expected offset 0
1:      beq x1,x2,1f ; expected offset 4
1: ; next line
        beq x1,x2,loop
        ; other comment
        beq x5,x6,666b
        lw x1,x2,4
