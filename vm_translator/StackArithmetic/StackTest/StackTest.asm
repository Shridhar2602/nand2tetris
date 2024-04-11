@17
D = A
@SP
A = M
M = D
@SP
M = M + 1
@17
D = A
@SP
A = M
M = D
@SP
M = M + 1
@SP
M = M - 1
@SP
A = M
D = M
@SP
M = M - 1
A = M
D = M - D
@StackTest.NOTEQ1
D;JEQ
D = 0
@StackTest.EQCONT1
0;JMP
(StackTest.NOTEQ1)
D = -1
(StackTest.EQCONT1)
@SP
A = M
M = D
@SP
M = M + 1
@17
D = A
@SP
A = M
M = D
@SP
M = M + 1
@16
D = A
@SP
A = M
M = D
@SP
M = M + 1
@SP
M = M - 1
@SP
A = M
D = M
@SP
M = M - 1
A = M
D = M - D
@StackTest.NOTEQ2
D;JEQ
D = 0
@StackTest.EQCONT2
0;JMP
(StackTest.NOTEQ2)
D = -1
(StackTest.EQCONT2)
@SP
A = M
M = D
@SP
M = M + 1
@16
D = A
@SP
A = M
M = D
@SP
M = M + 1
@17
D = A
@SP
A = M
M = D
@SP
M = M + 1
@SP
M = M - 1
@SP
A = M
D = M
@SP
M = M - 1
A = M
D = M - D
@StackTest.NOTEQ3
D;JEQ
D = 0
@StackTest.EQCONT3
0;JMP
(StackTest.NOTEQ3)
D = -1
(StackTest.EQCONT3)
@SP
A = M
M = D
@SP
M = M + 1
@892
D = A
@SP
A = M
M = D
@SP
M = M + 1
@891
D = A
@SP
A = M
M = D
@SP
M = M + 1
@SP
M = M - 1
@SP
A = M
D = M
@SP
M = M - 1
A = M
D = M - D
@StackTest.NOTLT4
D;JLT
D = 0
@StackTest.LTCONT4
0;JMP
(StackTest.NOTLT4)
D = -1
(StackTest.LTCONT4)
@SP
A = M
M = D
@SP
M = M + 1
@891
D = A
@SP
A = M
M = D
@SP
M = M + 1
@892
D = A
@SP
A = M
M = D
@SP
M = M + 1
@SP
M = M - 1
@SP
A = M
D = M
@SP
M = M - 1
A = M
D = M - D
@StackTest.NOTLT5
D;JLT
D = 0
@StackTest.LTCONT5
0;JMP
(StackTest.NOTLT5)
D = -1
(StackTest.LTCONT5)
@SP
A = M
M = D
@SP
M = M + 1
@891
D = A
@SP
A = M
M = D
@SP
M = M + 1
@891
D = A
@SP
A = M
M = D
@SP
M = M + 1
@SP
M = M - 1
@SP
A = M
D = M
@SP
M = M - 1
A = M
D = M - D
@StackTest.NOTLT6
D;JLT
D = 0
@StackTest.LTCONT6
0;JMP
(StackTest.NOTLT6)
D = -1
(StackTest.LTCONT6)
@SP
A = M
M = D
@SP
M = M + 1
@32767
D = A
@SP
A = M
M = D
@SP
M = M + 1
@32766
D = A
@SP
A = M
M = D
@SP
M = M + 1
@SP
M = M - 1
@SP
A = M
D = M
@SP
M = M - 1
A = M
D = M - D
@StackTest.NOTGT7
D;JGT
D = 0
@StackTest.GTCONT7
0;JMP
(StackTest.NOTGT7)
D = -1
(StackTest.GTCONT7)
@SP
A = M
M = D
@SP
M = M + 1
@32766
D = A
@SP
A = M
M = D
@SP
M = M + 1
@32767
D = A
@SP
A = M
M = D
@SP
M = M + 1
@SP
M = M - 1
@SP
A = M
D = M
@SP
M = M - 1
A = M
D = M - D
@StackTest.NOTGT8
D;JGT
D = 0
@StackTest.GTCONT8
0;JMP
(StackTest.NOTGT8)
D = -1
(StackTest.GTCONT8)
@SP
A = M
M = D
@SP
M = M + 1
@32766
D = A
@SP
A = M
M = D
@SP
M = M + 1
@32766
D = A
@SP
A = M
M = D
@SP
M = M + 1
@SP
M = M - 1
@SP
A = M
D = M
@SP
M = M - 1
A = M
D = M - D
@StackTest.NOTGT9
D;JGT
D = 0
@StackTest.GTCONT9
0;JMP
(StackTest.NOTGT9)
D = -1
(StackTest.GTCONT9)
@SP
A = M
M = D
@SP
M = M + 1
@57
D = A
@SP
A = M
M = D
@SP
M = M + 1
@31
D = A
@SP
A = M
M = D
@SP
M = M + 1
@53
D = A
@SP
A = M
M = D
@SP
M = M + 1
@SP
M = M - 1
@SP
A = M
D = M
@SP
M = M - 1
A = M
D = D + M
@SP
A = M
M = D
@SP
M = M + 1
@112
D = A
@SP
A = M
M = D
@SP
M = M + 1
@SP
M = M - 1
@SP
A = M
D = M
@SP
M = M - 1
A = M
D = M - D
@SP
A = M
M = D
@SP
M = M + 1
@SP
M = M - 1
@SP
A = M
D = M
D = -D
@SP
A = M
M = D
@SP
M = M + 1
@SP
M = M - 1
@SP
A = M
D = M
@SP
M = M - 1
A = M
D = D & M
@SP
A = M
M = D
@SP
M = M + 1
@82
D = A
@SP
A = M
M = D
@SP
M = M + 1
@SP
M = M - 1
@SP
A = M
D = M
@SP
M = M - 1
A = M
D = D | M
@SP
A = M
M = D
@SP
M = M + 1
@SP
M = M - 1
@SP
A = M
D = M
D = !D
@SP
A = M
M = D
@SP
M = M + 1
(VM_END)
@VM_END
0;JMP
