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
(Sys.init)
@4000
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
@THIS
M = D
@5000
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
@THAT
M = D
@Sys.init$ret.2
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
@Sys.main
0;JMP
(Sys.init$ret.2)
@SP
M = M - 1
@SP
A = M
D = M
@6
M = D
(Sys.Sys.init$LOOP)
@Sys.Sys.init$LOOP
0;JMP
(Sys.main)
@0
D = A
@SP
A = M
M = D
@SP
M = M + 1
@0
D = A
@SP
A = M
M = D
@SP
M = M + 1
@0
D = A
@SP
A = M
M = D
@SP
M = M + 1
@0
D = A
@SP
A = M
M = D
@SP
M = M + 1
@0
D = A
@SP
A = M
M = D
@SP
M = M + 1
@4001
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
@THIS
M = D
@5001
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
@THAT
M = D
@200
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
@1
D = D + A
@R15
M = D
@R13
D = M
@R15
A = M
M = D
@40
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
@2
D = D + A
@R15
M = D
@R13
D = M
@R15
A = M
M = D
@6
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
@3
D = D + A
@R15
M = D
@R13
D = M
@R15
A = M
M = D
@123
D = A
@SP
A = M
M = D
@SP
M = M + 1
@Sys.main$ret.3
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
@Sys.add12
0;JMP
(Sys.main$ret.3)
@SP
M = M - 1
@SP
A = M
D = M
@5
M = D
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
@LCL
D = M
@1
A = D + A
D = M
@SP
A = M
M = D
@SP
M = M + 1
@LCL
D = M
@2
A = D + A
D = M
@SP
A = M
M = D
@SP
M = M + 1
@LCL
D = M
@3
A = D + A
D = M
@SP
A = M
M = D
@SP
M = M + 1
@LCL
D = M
@4
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
(Sys.add12)
@4002
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
@THIS
M = D
@5002
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
@THAT
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
@12
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
