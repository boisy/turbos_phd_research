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
               ttl       Very simple initial program

               use       defs.d

tylg           set       Prgrm+Objct
atrv           set       ReEnt+rev
rev            set       $01
edition        set       1

               mod       eom,name,tylg,atrv,start,size

               org       0
stack          rmb       200
size           equ       .

name           fcs       /go/
               fcb       edition

start          equ       *

               clra
               clrb
               leax      rlenc,pcr
               ldy       #0
               os9       F$Fork

               clra
               clrb
               leax      rldec,pcr
               ldy       #0
               os9       F$Fork
               
               clra
               clrb
               leax      aclamp,pcr
               ldy       #0
               os9       F$Fork

sleep@         ldx       #0
               os9       F$Sleep
               bra       sleep@               

rlenc          fcs       /rlenc/
rldec          fcs       /rldec/
aclamp         fcs       /aclamp/

               emod
eom            equ       *
               end

