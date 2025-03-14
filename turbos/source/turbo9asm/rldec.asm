* Run-length decoder in hand-optimized Turbo9 assembler
*
* Entry:
*    X = source buffer (terminated w/ 0)
*    Y = destination buffer
*    D = destination buffer size
*
* Exit:
*    D = destination size
rldec:	pshs	d		save destination buffer size on stack
		clra
		clrb			D = 0
		pshs   	d		save decode size on stack

* 0,s = decode size (starts at 0, then increments)
* 2,s = destination size
lp@
* is count 0?
		ldb		,x		get count (0 - 255) in B
		beq     done    if 0, we're done
		clra            clear A (D = 0 - 255)
		addd    ,s      add D to decode size on stack
		cmpd    2,s     compare to destination size
		bgt     err     too large... stop decoding
		ldd     ,x++	get the count in A and byte in B
		pshs    a       save count on the stack
cl@		stb     ,y+		store byte in Y and increment
		dec     ,s		decrement the counter on the stack
		bne     cl@     continue if not 0
		leas    1,s     recover old count on stack
		bra     lp@     and go to top of loop for next pair
done	leas    4,s		destroy earlier pushed vars
		ldd     #0		load return with 0 (success)		
		rts				return
err		leas     4,s	destroy earlier pushed vars
		ldd     #-1		load return with -1 (error)
		rts				return
