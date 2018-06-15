CC=gcc
CFLAGS= -Wall -Wextra -Wcast-align -Winit-self -Wno-unused-parameter -DLINUX -g -fsanitize=address
PROGS= hid-keyboard hid-keyboard-wireless hid-keyboard-wireless-wired hid-mouse hid-mousepad hid-mouse-wireless hid-mouse-wireless-wired hid-keyboard-special hid-headset hid-stand hid-node

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

hid-keyboard-wireless:	usbip.c hid-keyboard-wireless.c
		${CC} ${CFLAGS} usbip.c -c
		${CC} ${CFLAGS} usbip.o hid-keyboard-wireless.c -o hid-keyboard-wireless

hid-keyboard-wireless-wired:	usbip.c hid-keyboard-wireless-wired.c
		${CC} ${CFLAGS} usbip.c -c
		${CC} ${CFLAGS} usbip.o hid-keyboard-wireless-wired.c -o hid-keyboard-wireless-wired

hid-keyboard-special:	usbip.c hid-keyboard-special.c
		${CC} ${CFLAGS} usbip.c -c
		${CC} ${CFLAGS} usbip.o hid-keyboard-special.c -o hid-keyboard-special

hid-mousepad:	usbip.c hid-mousepad.c
		${CC} ${CFLAGS} usbip.c -c
		${CC} ${CFLAGS} usbip.o hid-mousepad.c -o hid-mousepad

hid-headset:	usbip.c hid-headset.c
		${CC} ${CFLAGS} usbip.c -c
		${CC} ${CFLAGS} usbip.o hid-headset.c -o hid-headset

hid-stand:	usbip.c hid-stand.c
		${CC} ${CFLAGS} usbip.c -c
		${CC} ${CFLAGS} usbip.o hid-stand.c -o hid-stand

hid-node:	usbip.c hid-node.c
		${CC} ${CFLAGS} usbip.c -c
		${CC} ${CFLAGS} usbip.o hid-node.c -o hid-node

clean:
		rm -f ${PROGS} core core.* *.o temp.* *.out typescript*
