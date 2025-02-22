CC = CMOC
CFLAGS = -DTURBOS -D__CMOC__ -DMAIN --os9 -I ../clib

all: turbos.o go rlenc rldec aclamp rlenc_opt rldec_opt aclamp_opt
	ls -l rlenc rldec aclamp rlenc_opt rldec_opt aclamp_opt

turbos.o: ../clib/turbos.c
	cmoc $(CFLAGS) -c $^

go: go.asm
	lwasm -f os9 -I ../../source/definitions $^
	
rlenc: rlenc.c turbos.o
	cmoc $(CFLAGS) $^
	
rldec: rldec.c turbos.o
	cmoc $(CFLAGS) $^
	
aclamp: aclamp.c turbos.o
	cmoc $(CFLAGS) $^
	
rlenc_opt: rlenc_opt.asm
	lwasm -f os9 $^ -o$@

rldec_opt: rldec_opt.asm
	lwasm -f os9 $^ -o$@

aclamp_opt: aclamp_opt.asm
	lwasm -f os9 $^ -o$@

clean:
	rm rlenc rldec aclamp rlenc_opt rldec_opt aclamp_opt *.o *.map *.list
