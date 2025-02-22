#include "turbos.h"

int f_fork(char *modname, char *params, char typelang, char pagemem, int parammem) {
asm {
 ldx modname
 lda typelang
 ldb pagemem
 ldy parammem
 pshs u
 ldu params
 swi2
 fcb F$Fork
 puls u
 bcs error
 tfr a,b
 clra
 bra bye
error ldd #-1
bye 
}
}

int f_sleep(int ticks) {
asm {
 ldx ticks
 swi2
 fcb F$Sleep
 stx ticks
}
}

int f_icpt(interrupt void (*routine)(void)) {
asm {
 ldx routine
 swi2
 fcb F$Icpt
}
}

void *f_srqmem(int size) {
asm {
 ldd size
 pshs u
 swi
 fcb F$SRqMem
 tfr u,d
 puls u
 bcc ok
 ldd #-1
ok 
}
}

void f_srtmem(void *address, int size) {
asm {
 pshs u
 ldd size
 ldu address
 pshs u
 swi
 fcb F$SRtMem
 tfr u,d
 puls u
}
}


void mapVDG(void *addr) {
asm {
 ldd addr
 ldx #$FFC6
 lsra
l0@ lsra
 bcs l1@
 sta ,x
 bra l2@
l1@ sta 1,x
l2@ leax 2,x
 cmpx #$FFD4
 bne l0@
 }
}

