@0
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
@R13
M = D
@LCL
D = M
@0
D = D + A
@R15
M = D
@R13
D = M
@R15
A = M
M = D
(LOOP)
@ARG
D = M
@0
A = D + A
D = M
@SP
A = M
M = D
@SP
M = M + 1
@LCL
D = M
@0
A = D + A
D = M
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
@SP
M = M - 1
@SP
A = M
D = M
@R13
M = D
@LCL
D = M
@0
D = D + A
@R15
M = D
@R13
D = M
@R15
A = M
M = D
@ARG
D = M
@0
A = D + A
D = M
@SP
A = M
M = D
@SP
M = M + 1
@1
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
@R13
M = D
@ARG
D = M
@0
D = D + A
@R15
M = D
@R13
D = M
@R15
A = M
M = D
@ARG
D = M
@0
A = D + A
D = M
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
@LOOP
D;JNE
@LCL
D = M
@0
A = D + A
D = M
@SP
A = M
M = D
@SP
M = M + 1
(VM_END)
@VM_END
0;JMP
