// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
// The algorithm is based on repetitive addition.

// Using i variable as counter
//		@i 
//		M = 0		// i = 0
//		@R2
//		M = 0		// ans = 0
//	(LOOP)
//		@i
//		D = M 		// D = i
//		@R0
//		D = D - M 	// D = i - R0
//		@END
//		D;JGE		// if D <= 0; END
//	
//		@i
//		M = M + 1	// i += 1
//	
//		@R1
//		D = M
//		@R2
//		M = M + D	// ans += R1
//	
//		@LOOP		// goto LOOP
//		0;JMP
//	
//	(END)
//		@END
//		0;JMP
//	

// Using R0 itself as counter (Less instructions in loop)
	@R2
	M = 0		// ans = 0
	@R0
	D = M
	@END
	D;JEQ
(LOOP)

	@R1
	D = M
	@R2
	M = M + D	// ans += R1

	@R0
	D = M - 1	// R0--
	M = D
	@END
	D;JLE		// if R0 <= 0; END

	@LOOP		// goto LOOP
	0;JMP

(END)
	@END
	0;JMP
