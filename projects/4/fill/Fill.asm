// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.


@256
D = A
@rows		// number of rows in screen
M = D

@32
D = A
@cols		// number of registers in each row
M = D

@kbdOutput	// stores the keyboard output
M = 0

(MAIN)
	@i
	M = 0

	@SCREEN
	D = A
	@currScreenAddress	// variable storing the address of the screen register to be written into
	M = D

	@KBD
	D = M
	@kbdOutput
	M = D

	(LOOPROW)
		@i
		D = M
		@rows
		D = D - M
		@MAIN
		D;JGE		// jump to MAIN if all rows painted

		@i
		M = M + 1

		@j
		M = 0

		(LOOPCOL)
			@j
			D = M
			@cols
			D = D - M
			@LOOPROW
			D;JGE		// jump to LOOPROW if row j painted

			@kbdOutput
			D = M

			@BLACK
			D;JGT		// goto BLACK is any key pressed
			
			@WHITE
			0;JMP

			(BLACK)
				@currScreenAddress
				A = M
				M = -1			// set to black
				@CONTINUE
				0;JMP
		
			(WHITE)
				@currScreenAddress
				A = M
				M = 0			// set to white

			(CONTINUE)
			@j
			M = M + 1			// update j and the screenAddress
			@currScreenAddress
			M = M + 1

			@LOOPCOL
			0;JMP

	@MAIN
	0;JMP