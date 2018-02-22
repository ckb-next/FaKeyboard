/*******************************************************
 usbip-server - a platform for USB device prototyping

 Fredrik Andersson
 Copyright 2016, All Rights Reserved.

 This software may be used by anyone for any reason so
 long as the copyright notice in the source files
 remains intact.

 code repository located at:
	http://github.com/freand76/usbip-server
********************************************************/

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include "strafe.h"
#include "UsbUtil.h"
#include "AppBase.h"

using namespace UsbUtil;

/* Debug */
#include "Verbose.h"
using namespace Verbose;

/* HID bRequest fields. */
enum {
    GET_REPORT = 1,
    GET_IDLE = 2,
    GET_PROTOCOL = 3,
    SET_REPORT = 9,
    SET_IDLE = 10,
    SET_PROTOCOL = 11
};

int HIDEndpoint::Data(uint8_t* inData, uint8_t* outBuffer, int length)
{
    (void)outBuffer;

    // Poll packet?
    if (length == 8 && inData[0] == 0x08) {
        return 0;
    }

    printf("Endpoint 0 (HID), length %d:\n", length);
    for (int i = 0; i < length; i++) {
        printf("%02hhx ", inData[i]);
    }
    printf("\n");

    return 0;
}

int CorsairOutEndpoint::Data(uint8_t* inData, uint8_t* outBuffer, int length)
{
    (void)outBuffer;
   
    if (length == 8 && inData[0] == 0x08) {
        return 0;
    }

    printf("Endpoint 1 (Corsair Out), length %d:\n", length);
    for (int i = 0; i < length; i++) {
        printf("%02hhx ", inData[i]);
    }
    printf("\n");

    return 0;
}

int CorsairInEndpoint::Data(uint8_t* inData, uint8_t* outBuffer, int length)
{
    (void)outBuffer;
    
    printf("Endpoint 2 (Corsair In), length %d:\n", length);
    for (int i = 0; i < length; i++) {
        printf("%02hhx ", inData[i]);
    }
    printf("\n");

    return 0;
}

int HIDInterface::GenerateHIDReportDescriptor(uint8_t* buffer, int offset)
{
    static uint8_t reportDescriptorHID[] = {
 0x05, 0x01, 0x09, 0x06, 0xA1, 0x01, 0x05, 0x07, 0x19, 0xE0, 0x29, 0xE7, 0x15, 0x00, 0x25, 0x01,
 0x75, 0x01, 0x95, 0x08, 0x81, 0x02, 0x75, 0x08, 0x95, 0x01, 0x81, 0x01, 0x05, 0x07, 0x19, 0x00,
 0x2A, 0xFF, 0x00, 0x15, 0x00, 0x26, 0xFF, 0x00, 0x75, 0x08, 0x95, 0x06, 0x81, 0x00, 0x05, 0x08,
 0x19, 0x01, 0x29, 0x03, 0x25, 0x01, 0x75, 0x01, 0x95, 0x03, 0x91, 0x02, 0x75, 0x05, 0x95, 0x01,
 0x91, 0x01, 0xC0
    };

    int pos = offset;
    for (unsigned int idx = 0; idx < sizeof(reportDescriptorHID); idx++) {
        pos += SetUint(reportDescriptorHID[idx], buffer, pos, 1);
    }
    return pos-offset;
}

int HIDInterface::GenerateInterfaceDescriptor(uint8_t* buffer, int offset)
{
    int pos = offset;
    pos += UsbInterface::GenerateInterfaceDescriptor(buffer, pos);
    int rSize = GenerateHIDReportDescriptor(NULL, 0);

    pos += SetUint(9,      buffer, pos, 1);
    pos += SetUint(0x21,   buffer, pos, 1);
    pos += SetUint(0x110,  buffer, pos, 2);
    pos += SetUint(0,      buffer, pos, 1);
    pos += SetUint(1,      buffer, pos, 1);
    pos += SetUint(0x22,   buffer, pos, 1);
    pos += SetUint(rSize,  buffer, pos, 2);

    return pos - offset;
}

int HIDInterface::GetDescriptor(uint8_t* setup, uint8_t* data, uint8_t* replyBuffer, int bufLength)
{
    uint8_t bDescriptorType = setup[3];

    if (bDescriptorType == 0x22) {
        return GenerateHIDReportDescriptor(replyBuffer, 0);
    }

    return UsbInterface::GetDescriptor(setup, data, replyBuffer, bufLength);
}

int HIDInterface::InRequest(uint8_t* usbSetup, uint8_t* dataIn, uint8_t* dataOut, int transferLength) {
    (void)dataOut;

    uint8_t bRequest = usbSetup[1];
   
    if (bRequest == SET_REPORT) {
        /* black hole SET_REPORTs as meaningless. */
        return 0;
    }
    if (bRequest == SET_IDLE) {
	/* stall this request */
	return 0;
    }

    ERROR_VECTOR("Unknown InRequest UsbInterface", usbSetup, 8);
    ERROR_VECTOR("Data packet follows", dataIn, transferLength);
    return EP_STALL;
}

int CorsairOutInterface::GenerateHIDReportDescriptor(uint8_t* buffer, int offset)
{
    static uint8_t reportDescriptorHID[] = {
 0x05, 0x01, 0x09, 0x06, 0xA1, 0x01, 0x85, 0x01, 0x05, 0x07, 0x19, 0xE0, 0x29, 0xE7, 0x15, 0x00,
 0x25, 0x01, 0x75, 0x01, 0x95, 0x08, 0x81, 0x02, 0x19, 0x00, 0x29, 0x97, 0x15, 0x00, 0x25, 0x01,
 0x75, 0x01, 0x95, 0x98, 0x81, 0x02, 0xC0, 0x05, 0x0C, 0x09, 0x01, 0xA1, 0x01, 0x85, 0x02, 0x05,
 0x0C, 0x19, 0x00, 0x2A, 0xFF, 0x0F, 0x15, 0x00, 0x26, 0xFF, 0x0F, 0x75, 0x10, 0x95, 0x02, 0x81,
 0x00, 0xC0, 0x06, 0xC0, 0xFF, 0x09, 0x02, 0xA1, 0x01, 0x85, 0x03, 0x15, 0x00, 0x26, 0xFF, 0x00,
 0x95, 0x3F, 0x75, 0x08, 0x09, 0x02, 0x81, 0x02, 0xC0, 0x06, 0xC2, 0xFF, 0x09, 0x03, 0xA1, 0x01,
 0x85, 0x0E, 0x15, 0x00, 0x26, 0xFF, 0x00, 0x95, 0x3F, 0x75, 0x08, 0x09, 0x03, 0x81, 0x02, 0xC0
    };

    int pos = offset;
    for (unsigned int idx = 0; idx < sizeof(reportDescriptorHID); idx++) {
        pos += SetUint(reportDescriptorHID[idx], buffer, pos, 1);
    }
    return pos-offset;
}

int CorsairOutInterface::GenerateInterfaceDescriptor(uint8_t* buffer, int offset)
{
    int pos = offset;
    pos += UsbInterface::GenerateInterfaceDescriptor(buffer, pos);
    int rSize = GenerateHIDReportDescriptor(NULL, 0);

    pos += SetUint(9,      buffer, pos, 1);
    pos += SetUint(0x21,   buffer, pos, 1);
    pos += SetUint(0x110,  buffer, pos, 2);
    pos += SetUint(0,      buffer, pos, 1);
    pos += SetUint(1,      buffer, pos, 1);
    pos += SetUint(0x22,   buffer, pos, 1);
    pos += SetUint(rSize,  buffer, pos, 2);

    return pos - offset;
}

int CorsairOutInterface::GetDescriptor(uint8_t* setup, uint8_t* data, uint8_t* replyBuffer, int bufLength) {
    uint8_t bDescriptorType = setup[3];

    if (bDescriptorType == 0x22) {
        return GenerateHIDReportDescriptor(replyBuffer, 0);
    }

    return UsbInterface::GetDescriptor(setup, data, replyBuffer, bufLength);
}

int main(int argc, char* argv[]) {
    AppBase app;
    if (!app.HandleArguments(argc, argv)) {
	exit(0);
    }
    
    if (!app.Init()) {
	return EXIT_FAILURE;
    }
    
    StrafeDevice strafe;
    app.AddDevice(&strafe, stringArray[1], "1-1", 2, 3, USB_SPEED_FULL);

    if (!app.Start()) {
	return EXIT_FAILURE;
    }
    
    while (true) {
	usleep(500*1000);
        //printf("Tick\n");
    }

    app.Stop();
    
    return EXIT_SUCCESS;
}
