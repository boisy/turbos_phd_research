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
               leax      rlenc,pcr           point to program string
               bsr       launch              launch it
               leax      rldec,pcr           point to program string
               bsr       launch              launch it
               leax      aclamp,pcr          point to program string
               bsr       launch              launch it
     
sleep@         ldx       #0
               os9       F$Sleep
               bra       sleep@               
               
launch         clra                          no specific type/language
               clrb                          no additional memory pages
               pshs      u                   save statics pointer
               leau      params,pcr          point to the parameter area
               ldy       #1                  parameter size
               os9       F$Fork              fork the program
               puls      u,pc

rlenc          fcs       /rlenc/
rldec          fcs       /rldec/
aclamp         fcs       /aclamp/
params         fcb       C$CR

               emod
eom            equ       *
               end

