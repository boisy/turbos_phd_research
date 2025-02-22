# Demo for CMOC/Turbo9 port of FreeRTOS
#   Author: Boisy Pitre
#
SRCDIR=../..
RTOS_BASEDIR=$(SRCDIR)/Source
#include ".././.
CPU=turbo9

DEVC_PREFIX=
CC=$(DEVC_PREFIX)cmoc
AS=$(DEVC_PREFIX)as
AR=$(DEVC_PREFIX)ar
\OBJDUMP=$(DEVC_PREFIX)lwobjdump

CPPFLAGS+=-I. -I./asm-$(CPU)/arch-turbo9 -I../Common/include -I../../Source/portable/CMOC/Turbo9 \
  -I$(RTOS_BASEDIR)/include -DCMOC_TURBO9

CFLAGS+=-D$(CPU) -Wno-const -DFREERTOS

CSRCS=main.c startup.c 
#  ../Common/Minimal/PollQ.c

# vectors.c serial.c sci.c ParTest.c gelfunc.c \
#  ../Common/Minimal/flash.c \
#  ../Common/Minimal/dynamic.c \
#  ../Common/Minimal/BlockQ.c \
#  ../Common/Minimal/PollQ.c \
#  ../Common/Minimal/comtest.c \
#  ../Common/Minimal/integer.c \
#  ../Common/Minimal/death.c \

RTOS_OBJS = $(RTOS_BASEDIR)/portable/CMOC/Turbo9/port.c \
  $(RTOS_BASEDIR)/portable/MemMang/heap_2.c \
  $(RTOS_BASEDIR)/list.c \
  $(RTOS_BASEDIR)/tasks.c \
  $(RTOS_BASEDIR)/queue.c

#OBJS=$(CSRCS:.c=.o) $(RTOS_OBJS:.c=.o)
OBJS=$(CSRCS:.c=.o) $(RTOS_OBJS:.c=.o)

all:	main.obj

main.obj:	$(OBJS)
	$(CC) $(LDFLAGS) --raw -o $@ $^ 

clean:
	$(RM) $(OBJS) *.obj *.img *.rom *.list *.map lastpage

freertos.img: main.obj lastpage
	dd if=/dev/zero of=zero_file bs=10240 count=1
	cat zero_file main.obj > $@_tmp
	os9 padrom -c=0 65280 $@_tmp
	cat $@_tmp lastpage > $@

lastpage: lastpage.asm
	lwasm lastpage.asm --raw -olastpage

