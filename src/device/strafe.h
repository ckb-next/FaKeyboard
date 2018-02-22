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

#include "UsbDevice.h"
#include "UsbConfiguration.h"
#include "UsbInterface.h"
#include "UsbEndpoint.h"

const char* stringArray[5] = {
    "Corsair",
    "Corsair Strafe Gaming Keyboard",
    "0D04302AAEC784405680168EF5001941",
    "Mouse Configuration",
    "Mouse Interface",
};

// So, I need a UsbEndpoint, a UsbInterface and a UsbDevice.
// What could possibly go wrong?

// For the sake of testing, we don't send anything from this.
class HIDEndpoint : public UsbEndpoint {
public:
    HIDEndpoint() : UsbEndpoint(0x81, 3, 8, 8) {};
    int Data(uint8_t* inData, uint8_t* outBuffer, int length);
};

class CorsairOutEndpoint : public UsbEndpoint {
public:
    CorsairOutEndpoint() : UsbEndpoint(0x82, 3, 64, 1) {};
    int Data(uint8_t* inData, uint8_t* outBuffer, int length);
};

class CorsairInEndpoint : public UsbEndpoint {
public:
    CorsairInEndpoint() : UsbEndpoint(0x03, 3, 64, 1) {}
    int Data(uint8_t* inData, uint8_t* outBuffer, int length);
};

class HIDInterface : public UsbInterface {
public:
    HIDInterface() :
    UsbInterface(0, 3, 1, 1, 1, endpointArray) {};

    int GenerateInterfaceDescriptor(uint8_t* buffer, int offset);
    int GetDescriptor(uint8_t* setup, uint8_t* data, uint8_t* replyBuffer, int bufLength);
    int InRequest(uint8_t* usbSetup, uint8_t* data, uint8_t* replyBuffer, int transferLength);
private:
    int GenerateHIDReportDescriptor(uint8_t* buffer, int offset);
    HIDEndpoint endpoint;
    UsbEndpoint* endpointArray[1] = { &endpoint };
};

class CorsairOutInterface : public UsbInterface {
public:
    CorsairOutInterface() :
    UsbInterface(1, 3, 0, 0, 1, endpointArray) {};

    int GenerateInterfaceDescriptor(uint8_t* buffer, int offset);
    int GetDescriptor(uint8_t* setup, uint8_t* data, uint8_t* replyBuffer, int bufLength);
private:
    int GenerateHIDReportDescriptor(uint8_t* buffer, int offset);
    CorsairOutEndpoint endpoint;
    UsbEndpoint* endpointArray[1] = { &endpoint };
};

class CorsairInInterface : public UsbInterface {
public:
    CorsairInInterface() :
    UsbInterface(2, 3, 0, 0, 1, endpointArray) {};

private:
    CorsairInEndpoint endpoint;
    UsbEndpoint* endpointArray[1] = { &endpoint };
};

class StrafeDevice : public UsbDevice {
public:
    StrafeDevice() :
    UsbDevice(0x1b1c, 0x1b15, 0x0204, 0, 0, 0, 1, configurationArray, &usbStringArray, 1, 2, 3) {};
private:
    HIDInterface hid;
    CorsairOutInterface cs_out;
    CorsairInInterface cs_in;
    UsbInterface* interfaceArray[3] = { &hid, &cs_out, &cs_in };
    UsbConfiguration config = { 1, 3, interfaceArray, 0, 0xA0, 250 };
    UsbConfiguration* configurationArray[1] = { &config };
    UsbString usbStringArray = { 5, stringArray };
};
