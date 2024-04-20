@256
D = A
@SP
M = D
@1
D = A
@LCL
M = -D
@2
D = A
@ARG
M = -D
@3
D = A
@THIS
M = -D
@4
D = A
@THAT
M = -D
@$ret.1
D = A
@SP
A = M
M = D
@SP
M = M + 1
@LCL
D = M
@SP
A = M
M = D
@SP
M = M + 1
@ARG
D = M
@SP
A = M
M = D
@SP
M = M + 1
@THIS
D = M
@SP
A = M
M = D
@SP
M = M + 1
@THAT
D = M
@SP
A = M
M = D
@SP
M = M + 1
@SP
D = M
@5
D = D - A
@ARG
M = D
@SP
D = M
@LCL
M = D
@Sys.init
0;JMP
($ret.1)
(Main.fibonacci)
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
@2
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
@Main.NOTLT1
D;JLT
D = 0
@Main.LTCONT1
0;JMP
(Main.NOTLT1)
D = -1
(Main.LTCONT1)
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
@Main.Main.fibonacci$N_LT_2
D;JNE
@Main.Main.fibonacci$N_GE_2
0;JMP
(Main.Main.fibonacci$N_LT_2)
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
@R13
M = D
@5
D = D - A
A = D
D = M
@R14
M = D
@SP
M = M - 1
@SP
A = M
D = M
@ARG
A = M
M = D
@ARG
D = M
@SP
M = D + 1
@R13
D = M
@1
D = D - A
A = D
D = M
@THAT
M = D
@R13
D = M
@2
D = D - A
A = D
D = M
@THIS
M = D
@R13
D = M
@3
D = D - A
A = D
D = M
@ARG
M = D
@R13
D = M
@4
D = D - A
A = D
D = M
@LCL
M = D
@R14
A = M
0;JMP
(Main.Main.fibonacci$N_GE_2)
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
@2
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
@Main.fibonacci$ret.2
D = A
@SP
A = M
M = D
@SP
M = M + 1
@LCL
D = M
@SP
A = M
M = D
@SP
M = M + 1
@ARG
D = M
@SP
A = M
M = D
@SP
M = M + 1
@THIS
D = M
@SP
A = M
M = D
@SP
M = M + 1
@THAT
D = M
@SP
A = M
M = D
@SP
M = M + 1
@SP
D = M
@6
D = D - A
@ARG
M = D
@SP
D = M
@LCL
M = D
@Main.fibonacci
0;JMP
(Main.fibonacci$ret.2)
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
@Main.fibonacci$ret.3
D = A
@SP
A = M
M = D
@SP
M = M + 1
@LCL
D = M
@SP
A = M
M = D
@SP
M = M + 1
@ARG
D = M
@SP
A = M
M = D
@SP
M = M + 1
@THIS
D = M
@SP
A = M
M = D
@SP
M = M + 1
@THAT
D = M
@SP
A = M
M = D
@SP
M = M + 1
@SP
D = M
@6
D = D - A
@ARG
M = D
@SP
D = M
@LCL
M = D
@Main.fibonacci
0;JMP
(Main.fibonacci$ret.3)
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
@LCL
D = M
@R13
M = D
@5
D = D - A
A = D
D = M
@R14
M = D
@SP
M = M - 1
@SP
A = M
D = M
@ARG
A = M
M = D
@ARG
D = M
@SP
M = D + 1
@R13
D = M
@1
D = D - A
A = D
D = M
@THAT
M = D
@R13
D = M
@2
D = D - A
A = D
D = M
@THIS
M = D
@R13
D = M
@3
D = D - A
A = D
D = M
@ARG
M = D
@R13
D = M
@4
D = D - A
A = D
D = M
@LCL
M = D
@R14
A = M
0;JMP
(VM_END)
@VM_END
0;JMP
(Sys.init)
@4
D = A
@SP
A = M
M = D
@SP
M = M + 1
@Sys.init$ret.4
D = A
@SP
A = M
M = D
@SP
M = M + 1
@LCL
D = M
@SP
A = M
M = D
@SP
M = M + 1
@ARG
D = M
@SP
A = M
M = D
@SP
M = M + 1
@THIS
D = M
@SP
A = M
M = D
@SP
M = M + 1
@THAT
D = M
@SP
A = M
M = D
@SP
M = M + 1
@SP
D = M
@6
D = D - A
@ARG
M = D
@SP
D = M
@LCL
M = D
@Main.fibonacci
0;JMP
(Sys.init$ret.4)
(Sys.Sys.init$END)
@Sys.Sys.init$END
0;JMP
(VM_END)
@VM_END
0;JMP
