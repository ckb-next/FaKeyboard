CC=gcc
CFLAGS= -Wall -DLINUX -g
#PROGS= hid-mouse hid-keyboard cdc-acm
PROGS= hid-keyboard hid-mouse hid-mousepad hid-mouse-wireless hid-mouse-wireless-wired

all:	${PROGS}

hid-mouse:	usbip.c hid-mouse.c
		${CC} ${CFLAGS} usbip.c -c
		${CC} ${CFLAGS} usbip.o hid-mouse.c -o hid-mouse

hid-mouse-wireless:	usbip.c hid-mouse-wireless.c
		${CC} ${CFLAGS} usbip.c -c
		${CC} ${CFLAGS} usbip.o hid-mouse-wireless.c -o hid-mouse-wireless

hid-mouse-wireless-wired:	usbip.c hid-mouse-wireless-wired.c
		${CC} ${CFLAGS} usbip.c -c
		${CC} ${CFLAGS} usbip.o hid-mouse-wireless-wired.c -o hid-mouse-wireless-wired

hid-keyboard:	usbip.c hid-keyboard.c 
		${CC} ${CFLAGS} usbip.c -c 
		${CC} ${CFLAGS} usbip.o hid-keyboard.c -o hid-keyboard

hid-mousepad:	usbip.c hid-mousepad.c 
		${CC} ${CFLAGS} usbip.c -c 
		${CC} ${CFLAGS} usbip.o hid-mousepad.c -o hid-mousepad

#cdc-acm:	usbip.c cdc-acm.c 
#		${CC} ${CFLAGS} usbip.c -c 
#		${CC} ${CFLAGS} usbip.o cdc-acm.c -o cdc-acm
clean:
		rm -f ${PROGS} core core.* *.o temp.* *.out typescript*
