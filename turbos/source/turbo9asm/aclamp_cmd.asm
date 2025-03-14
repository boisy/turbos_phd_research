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

        nam     aclamp
        ttl     Audio clamp in Turbo9 assembly

        use     defs.d

tylg    set     Prgrm+Objct
atrv    set     ReEnt+rev
rev     set     $01
edition set     1

        mod     eom,name,tylg,atrv,start,size

        org     0
dest    rmb     sourcel*2
stack   rmb     16
size    equ     .

name    fcs     /aclamp/
        fcb     edition

* Clamp low/high values
clmplo  equ     $03
clmphi  equ     $0C

source  fcb     1,6,9,18,19,21,2,8
        fcb     9,15,23,12,11,15,16,7
        fcb     1,6,9,18,19,21,2,8
        fcb     9,15,23,12,11,15,16,7
sourcel equ     *-source

start	
loop	ldd     #clmplo*256+clmphi load A/B with clamp values
        leax    source,pcr	point X to the source buffer
        leay    dest,u      point Y to the destination buffer
        pshs    u           save statics pointer
        ldu     #sourcel    load U with the source size
        bsr     aclamp		branch to the subroutine
        puls    u           recover statics pointer
*	    ldx     #2			timed sleep 0 means sleep forever
*	    os9     F$Sleep		perform sleep waiting on a signal
        bra     loop		once we wake up, go back to sleep
        
        use     aclamp.asm    
        
        emod
eom     equ     *
        end	    
