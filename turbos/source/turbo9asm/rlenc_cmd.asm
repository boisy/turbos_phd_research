************************************************************
* TurbOS
************************************************************
* See LICENSE.txt for licensing information.
************************************************************
*
* $Id$
*
* Edt/Rev  YYYY/MM/DD  Modified by
* Comment
* ----------------------------------------------------------
*   1      2024/05/31  Boisy Gene Pitre
* Created.

		nam		rlenc
		ttl		Run-length encoder in Turbo9 assembly
		
		use		defs.d

tylg    set       Prgrm+Objct
atrv    set       ReEnt+rev
rev     set       $01
edition set       1

		mod       eom,name,tylg,atrv,start,size

		org       0
dest    rmb       sourcel*2		make dest 2X (worst case)
stack   rmb       16			minimal stack needed
size    equ       .

name    fcs       /rlenc/
		fcb       edition

source  fcb       1,1,2,2,1,4,4,4
        fcb       4,4,4,12,13,13,13,13
		fcb       4,4,4,12,13,13,13,13
		fcb       4,4,4,12,13,13,13,13
		fcb       4,4,4,12,13,13,13,13
		fcb       4,4,4,12,13,13,13,13
		fcb       4,4,4,12,13,13,13,13
		fcb       4,4,4,12,13,13,13,13
sourcel equ       *-source

start:
	
forever	leax      source,pcr	point X to the src buffer
		leay      dest,u        point Y to the dest buffer
		ldd       #sourcel	    load D with the source size
 	   	bsr       rlenc			branch to the subroutine
*		ldx       #2			timed sleep 0 (forever)
*		os9		  F$Sleep		sleep & wait on a signal
		bra       forever		once awoke, go back to sleep
	
    	use       rlenc.asm
		     
        emod
eom     equ       *
        end	    
