* Run-length encoder in hand-optimized Turbo9 assembler
*
* Entry:
*    X = source buffer
*    Y = destination buffer
*    D = source buffer size
*
* Exit:
*    Destination buffer ends in $00
rlenc:	pshs  	d       save src buffer size (use as counter)
		bra 	lstart	branch to start

* Entry:
*    0,s = return value
*    2,s = return address
*    4,s = 16-bit buffer size left to evaluate
* Exit:
*    4,s = 16-bit value decremented by 1
*   CC.Z = Set if decrement results in 0
decsize	pshs 	d       save D on stack
    	ldd     4,s     load buffer size
		subd    #$0001  decrement
		std     4,s     save back
		puls    d,pc  	pull D and return
	
ltop	bsr     decsize decrement source counter  
		beq     done    branch if result was 0 (done)
lstart	lda     #1  	set initial "same byte" counter to 1 
* Entry:
* 0,s = return address
* 2,s = source buffer size
    	ldb     ,x+     get source byte, increment pointer
loop	cmpb    ,x      compare with next byte... same?
		bne     saveoff no, different byte
		leax    1,x     else increment pointer
		cmpa    #255    reached max value for counter?
		beq     saveoff if so, save off
		inca      		else increment "same byte" counter
		bsr     decsize decrement source buffer size
		beq     done    branch if zero (done)
		bra     loop 	continue
	
saveoff std     ,y++	save off "same byte" counter and byte
		bra     ltop	get next byte
	 	
done  	clra			terminate with 0 count byte
	 	sta		,y		save it
     	puls   	d,pc 	pull and return
