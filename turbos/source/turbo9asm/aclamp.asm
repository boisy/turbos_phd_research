* Audio clamp in hand-optimized Turbo9 assembler
*
* Entry:
*    A = low clamp value
*    B = high clamp value
*    X = source buffer
*    Y = destination buffer
*    U = source buffer size
* Exit:
*    D = destination size
aclamp:
		pshs	d		save low/high clamp values
loop@ 	lda     ,x+		get byte in source buffer
		cmpa    ,s      compare to low clamp
		bge     ok@		branch if >= (check high)
		lda     ,s      load A with low clamp value
		bra     ok2@    and continue to next byte
ok@ 	cmpa    1,s     compare to high clamp
    	ble     ok2@    branch if <= (byte in range)
		lda     1,s     load A with high clamp value
ok2@ 	sta     ,y+     save off to destination
    	leau    -1,u    subtract source buffer size
		cmpu    #0      at end?
		bne     loop@   branch if not
		puls    d,pc    pull D and return
