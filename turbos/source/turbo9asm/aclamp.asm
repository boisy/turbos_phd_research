* Audio clamp in hand-optimized Turbo9 assembler
*
* Entry:
*    A = upper clamp value
*    B = lower clamp value
*    X = source buffer
*    Y = destination buffer
*    U = source buffer size
* Exit:
*    D = destination size
aclamp:
    pshs       d
l@  lda        ,x+
	cmpa       ,s
	ble        ok@
	lda        ,s
	bra        ok2@
ok@ cmpa       1,s
    bge        ok2@
	lda        1,s
ok2@ sta       ,y+
    leau       -1,u
	cmpu       #0
	bne        l@	
	puls       d,pc
