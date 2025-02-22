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
*   1      2023/08/29  BGP
* Created.

               nam       go
               ttl       Start sub-block processes

               use       defs.d

tylg           set       Prgrm+Objct
atrv           set       ReEnt+rev
rev            set       $01
edition        set       1

               mod       eom,name,tylg,atrv,start,size

               org       0
pids           rmb       3			   
stack          rmb       200
size           equ       .

name           fcs       /go/
               fcb       edition

rlenc          fcs       /rlenc/
rldec          fcs       /rldec/
aclamp         fcs       /aclamp/

forkit     	lda       #Prgrm+Objct	; A = type/language byte
* B needs to be at least 4 for C-based testbench programs (1 for assembly testbench programs)
			ldb		  #4   	    	; B = size of optional 256-byte data pages
			ldy       #0			; Y = size of the parameter area (0 is default)
			os9       F$Fork		; fork the process
			rts
start                    
			leax      rlenc,pcr     ; point to module to fork
			bsr       forkit
			bcs       exit
			sta       pids+0,u      ; save process ID
			
			leax      rldec,pcr		; X = address of the module name
			bsr       forkit
			bcs       exit
			sta       pids+1,u      ; save process ID
	
			leax      aclamp,pcr	; X = address of the module name
			bsr       forkit
			bcs       exit
			sta       pids+2,u      ; save process ID
	

loop       
			ldx     #60
			os9		F$Sleep
            lda     pids+0,u
			ldb		#5
			os9		F$Send
			ldx     #60
			os9		F$Sleep
            lda     pids+1,u
			ldb		#5
			os9		F$Send
			ldx     #60
			os9		F$Sleep
            lda     pids+2,u
			ldb		#5
			os9		F$Send
			bra		loop
						
exit		os9		F$Exit

               emod      
eom            equ       *
               end       

