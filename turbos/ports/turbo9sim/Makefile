%: %.c turbos.o
	cmoc --os9 -I ../../source/clib -DMAIN -DTURBOS -o $@ $^

%: %.asm
	lwasm $^

ifndef TURBOSDIR
	TURBOSDIR=$(HOME)/Projects/turbos
endif

PORTNAME	= turbo9sim
vpath %.c ../../source/clib
vpath %.asm ../../source/turbo9asm
vpath %.c ../../source/c

include $(TURBOSDIR)/ports/rules.mak

VPATH=$(TURBOSDIR)/source/modules $(TURBOSDIR)/source/commands $(TURBOSDIR)/source/kernel_min $(TURBOSDIR)/tests

AFLAGS += -DPORTNAME="$(PORTNAME)" -D$(PORTNAME) -I$(TURBOSDIR)/source/include

TURBOSMODS = $(TURBOSDIR)/ports/turbo9sim
COMMANDS_ASM = go rlenc_cmd rldec_cmd aclamp_cmd
COMMANDS_C = go rlenc rldec aclamp
MODULES	= $(TURBOSMODS)/kernel_min $(TURBOSMODS)/init $(TURBOSMODS)/tkt9sim_min
OBJS_BASELINE = $(MODULES) go_baseline
OBJS_C_EXEMPLARS	= $(MODULES) $(COMMANDS_C)
OBJS_ASM_EXEMPLARS = $(MODULES) $(COMMANDS_ASM)

TARGETS =  turbos turbos_baseline.img turbos_exemplars_asm.img turbos_exemplars_c.img

all:	$(TARGETS)
	@ls -l turbos_baseline.img.rom
	@ls -l turbos_exemplars_asm.img.rom
	@ls -l turbos_exemplars_c.img.rom

turbos:
	cd $(TURBOSDIR)/ports/turbo9sim; make
	
cmds: $(COMMANDS)

turbos.o: turbos.c
	cmoc -c $<
		
lint: *.asm $(KERNEL_SOURCES) $(MODULE_SOURCES)
	$(foreach file,$^,mamou -ls -p $(file) > /tmp/lint.out; mv /tmp/lint.out $(file) ;)

turbos_baseline.img: $(OBJS_BASELINE)
	cat $? > $@.rom
	cat $@.rom > $@.tmp
	os9 padrom -b -c=0 65280 $@.tmp
	dd if=/dev/zero of=lastpage.tmp bs=1 count=240
	printf "\x00\x00\x01\x00\x01\x03\x01\x0F\x01\x0C\x01\x06\x01\x09" > vectors.tmp
	stat -f "obase=16; 65536-256-%z+20" $@.rom | bc | xxd -r -p >> vectors.tmp
	cat $@.tmp lastpage.tmp vectors.tmp > $@
	
turbos_exemplars_asm.img: $(OBJS_ASM_EXEMPLARS)
	cat $? > $@.rom
	cat $@.rom > $@.tmp
	os9 padrom -b -c=0 65280 $@.tmp
	dd if=/dev/zero of=lastpage.tmp bs=1 count=240
	printf "\x00\x00\x01\x00\x01\x03\x01\x0F\x01\x0C\x01\x06\x01\x09" > vectors.tmp
	stat -f "obase=16; 65536-256-%z+20" $@.rom | bc | xxd -r -p >> vectors.tmp
	cat $@.tmp lastpage.tmp vectors.tmp > $@
	
turbos_exemplars_c.img: $(OBJS_C_EXEMPLARS)
	cat $? > $@.rom
	cat $@.rom > $@.tmp
	os9 padrom -b -c=0 65280 $@.tmp
	dd if=/dev/zero of=lastpage.tmp bs=1 count=240
	printf "\x00\x00\x01\x00\x01\x03\x01\x0F\x01\x0C\x01\x06\x01\x09" > vectors.tmp
	stat -f "obase=16; 65536-256-%z+20" $@.rom | bc | xxd -r -p >> vectors.tmp
	cat $@.tmp lastpage.tmp vectors.tmp > $@
	
clean:
	-$(RM) *.o $(TARGETS) $(COMMANDS_C) $(COMMANDS_ASM) $(OBJS_C_EXEMPLARS) $(OBJS_ASM_EXEMPLARS) $(OBJS_BASELINE) *.dump *.list *.rom *.tmp *.map

gh:
	open "http://www.github.com/boisy/turbos"

xroar_mac_c: turbos_c.dsk
	/Applications/Xroar.app/Contents/MacOS/xroar --load-fd0 $^

xroar_mac_asm: turbos_asm.dsk
	/Applications/Xroar.app/Contents/MacOS/xroar --load-fd0 $^

emuc: turbos_c.dsk
	mame -inipath ~/mame -cfg_directory ~/mame/cfg coco2b -flop1 $^

emuasm: turbos_asm.dsk
	mame -inipath ~/mame -cfg_directory ~/mame/cfg coco2b -flop1 $^

demuc: turbos_c.dsk
	mame -inipath ~/mame -cfg_directory ~/mame/cfg coco2b -d -flop1 $^

demuasm: turbos_asm.dsk
	mame -inipath ~/mame -cfg_directory ~/mame/cfg coco2b -d -flop1 $^
