CC=m68k-palmos-gcc
CFLAGS = -O2 -g #-Wall
PILRC = pilrc
OBJRES = m68k-palmos-obj-res
BUILDPRC = build-prc

TARGET = BaseConv
TYPE = appl
ID = BScv

OSVERSION = 5r3

PALMUSBPORT=/dev/tty.usbserial-FTCURT8N

all: ${TARGET}.prc

${TARGET}: ${TARGET}.o 
	$(CC) -palmos${OSVERSION} -o ${TARGET} ${TARGET}.c

${TARGET}.prc: ${TARGET} ${TARGET}.rcp
	-/bin/rm -f *.bin
	$(OBJRES) ${TARGET}
	$(PILRC) ${TARGET}.rcp
	mv Tbmp03e9.bin tAIB03e9.bin
	$(BUILDPRC) -t ${TYPE} ${TARGET}.prc "$(TARGET)" $(ID) ${TARGET} *.bin

clean:
	/bin/rm *.o *.bin ${TARGET} *.grc ${TARGET}.prc

install: ${TARGET}.prc
	pilot-xfer -p ${PALMUSBPORT} -i ${TARGET}.prc

rel:
	cp ${TARGET}.prc release/
	cp *.bin *.grc release/
