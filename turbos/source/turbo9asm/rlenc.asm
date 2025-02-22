	* Run-length encoder in hand-optimized Turbo9 assembler
*
* Entry:
*    X = source buffer
*    Y = destination buffer
*    D = source buffer size
*
* Exit:
*    D = destination size
rlenc:
	pshs	  d			save destination buffer size on stack
	lda       #1        set "same byte" counter to 1
    bra       loop_start

* Entry:
*    0,s = return value
*    2,s = buffer size left to evaluate
decsize
    pshs      d
    ldd       4,s
	subd      #$0001
	std       4,s
	puls      d,pc
	
loop_top
    bsr       decsize
	beq       done      branch if result of dec was 0
	
loop_start
    ldb       ,x+       get next byte
loop_compare	
	cmpb      ,x        same as next
	bne       saveoff   no, different byte
	leax      1,x       increment X
	cmpa      #255      max value for counter?
	beq       saveoff   if so, save off
	bsr       decsize
	beq       done
	bra       loop_compare
	
saveoff
    std       ,y++
	bra       loop_top
	 	
done  std     ,y++
     puls     d,pc
