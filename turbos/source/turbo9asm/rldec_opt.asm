*******************************************************************************
* TurbOS
*******************************************************************************
* See LICENSE.txt for licensing information.
********************************************************************
*
* $Id$
*
* Edt/Rev  YYYY/MM/DD  Modified by
* Comment
* ------------------------------------------------------------------
*   1      2024/05/31  BGP
* Created.

               nam       rldec
               ttl       Run-length decoder in Turbo9 assembly

               use       defs.d

tylg           set       Prgrm+Objct
atrv           set       ReEnt+rev
rev            set       $01
edition        set       1

               mod       eom,name,tylg,atrv,start,size

sourcesize     equ       32

               org       0
source         rmb       sourcesize
dest           rmb       sourcesize*2
stack          rmb       200
size           equ       .

name           fcs       /rldec/
               fcb       edition

*
* RLE decoder.
*
* Example 1:
* 12 bytes of input: [1]A[1]B[1]A[3]C[1]A[1]B
*  8 bytes of output:  ABACCCABA
*
* Example 2:
* 12 bytes of input: [5]D[1]A[1]C[1]D[5]B[3]D
* 16 bytes of output:  DDDDDACDBBBBBDDD
*
	
start:
	leax	sighandler,pcr		point to the signal handler
	os9		F$Icpt			install it
	
forever	
	ldx       #0				timed sleep 0 means sleep forever
	os9		F$Sleep			perform sleep waiting on a signal
	bra       forever			once we wake up, go back to sleep
	
sighandler
	leax      source,u			point X to the source buffer
	leay      dest,u              point Y to the destination buffer
	ldd       #sourcesize         load D with the source size
*	bsr      	rldec			branch to the subroutine
	rti						return from the signal handler
	
* Run-length decoder in hand-optimized Turbo9 assembler
*
* Entry:
*    X = source buffer
*    Y = destination buffer
*    D = destination buffer size
*
* Exit:
*    D = destination size
rldec:
	pshs	  d			save destination buffer size on stack
	clra
	clrb				D = 0
	pshs      d			save decode size (starts at 0) on stack

* 0,s = decode size (starts at 0, then increments)
* 2,s = destination size
loop_top
* is count 0?
	ldb       ,x        get count (0 - 255) in B
	beq       done      if 0, we're done
	clra                clear A (D = 0 - 255)
	addd      ,s        add D to decode size on stack
	cmpd      2,s       compare to destination size
	bgt       err       too large... stop decoding
    ldd       ,x++		get the count in A and byte in B
	pshs      a         save count on the stack
copy_loop
	stb       ,y+		store byte in Y and increment
	dec       ,s		decrement the counter on the stack
	bne       copy_loop	continue if not 0
	leas      -1,s      recover old count on stack
	bra       loop_top  and go to top of loop for next pair
done	
	leas      4,s		destroy earlier pushed vars
	ldd       #0		load return with 0 (success)		
	rts
err leas      4,s		destroy earlier pushed vars
    ldd       #-1		load return with -1 (error)
	rts
    
	emod
eom  equ       *
     end	    
