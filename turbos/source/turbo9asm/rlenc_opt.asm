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

               nam       rlenc
               ttl       Run-length encoder in Turbo9 assembly

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

name           fcs       /rlenc/
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
	swi2
	fcb		F$Icpt			install it
	
forever	
	ldx       #0				timed sleep 0 means sleep forever
	swi2
	fcb		F$Sleep			perform sleep waiting on a signal
	bra       forever			once we wake up, go back to sleep
	
sighandler
	leax      source,u			point X to the source buffer
	leay      dest,u              point Y to the destination buffer
	ldd       #sourcesize         load D with the source size
*	bsr      	rlenc			branch to the subroutine
	rti						return from the signal handler
	
* Run-length encoder in hand-optimized Turbo9 assembler
*
* Entry:
*    X = source buffer
*    Y = destination buffer
*    D = source buffer size
*
* Exit:
*    D = destination size
rlenc:
	pshs	  d			save destination buffer size on stack
	lda       #1        set "same byte" counter to 1
    bra       loop_start

* Entry:
*    0,s = return value
*    2,s = buffer size left to evaluate
decsize
    pshs      d
    ldd       4,s
	subd      #$0001
	std       4,s
	puls      d,pc
	
loop_top
    bsr       decsize
	beq       done      branch if result of dec was 0
	
loop_start
    ldb       ,x+       get next byte
loop_compare	
	cmpb      ,x        same as next
	bne       saveoff   no, different byte
	leax      1,x       increment X
	cmpa      #255      max value for counter?
	beq       saveoff   if so, save off
	bsr       decsize
	beq       done
	bra       loop_compare
	
saveoff
    std       ,y++
	bra       loop_top
	 	
done  std     ,y++
     puls     d,pc
	     
	emod
eom  equ       *
     end	    
