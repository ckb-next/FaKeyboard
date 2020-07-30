/* ########################################################################

   USBIP hardware emulation

   ########################################################################

   Copyright (c) : 2016  Luis Claudio Gambôa Lopes

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   For e-mail suggestions :  lcgamboa@yahoo.com
   ######################################################################## */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "usbip.h"

/* Device Descriptor */
const USB_DEVICE_DESCRIPTOR dev_dsc =
{
    0x12,                   // Size of this descriptor in bytes
    0x01,                   // DEVICE descriptor type
    0x0200,                 // USB Spec Release Number in BCD format
    0x00,                   // Class Code
    0x00,                   // Subclass code
    0x00,                   // Protocol code
    0x40,                   // Max packet size for EP0, see usb_config.h
    0x1b1c,                 // Vendor ID
    0x1b4c,                 // Product ID: Mouse in a circle fw demo
    0x0011,                 // Device release number in BCD format
    0x01,                   // Manufacturer string index
    0x02,                   // Product string index
    0x03,                   // Device serial number string index
    0x01                    // Number of possible configurations
};

/* Configuration 1 Descriptor */
const CONFIG_HID_BRAGI  configuration_hid = {{
        /* Configuration Descriptor */
        0x09,//sizeof(USB_CFG_DSC),    // Size of this descriptor in bytes
        USB_DESCRIPTOR_CONFIGURATION,                // CONFIGURATION descriptor type
        0x0074,                 // Total length of data for this cfg
        4,                      // Number of interfaces in this cfg
        1,                      // Index value of this configuration
        0,                      // Configuration string index
        0xe0,
        250,                    // Max power consumption (2X mA)
    }, {
        /* Interface Descriptor */
        0x09,//sizeof(USB_INTF_DSC),   // Size of this descriptor in bytes
        USB_DESCRIPTOR_INTERFACE,               // INTERFACE descriptor type
        0,                      // Interface Number
        0,                      // Alternate Setting Number
        1,                      // Number of endpoints in this intf
        0x03,                   // Class code
        0x00,                   // Subclass code
        0x00,                   // Protocol code
        0,                      // Interface string index
    }, {
        /* HID Class-Specific Descriptor */
        0x09,               // Size of this descriptor in bytes RRoj hack
        0x21,                // HID descriptor type
        0x0111,                 // HID Spec Release Number in BCD format (1.11)
        0x00,                   // Country Code (0x00 for Not supported)
        0x01,        // Number of class descriptors, see usbcfg.h
        0x22,                // Report descriptor type
        0x003c,           // Size of the report descriptor
    }, {
        /* Endpoint Descriptor */
        0x07,/*sizeof(USB_EP_DSC)*/
        USB_DESCRIPTOR_ENDPOINT,    //Endpoint Descriptor
        0x82,            //EndpointAddress
        0x03,                       //Attributes
        0x0040,                  //size
        0x01                        //Interval
    },
    // IF 2
    {
        /* Interface Descriptor */
        0x09,//sizeof(USB_INTF_DSC),   // Size of this descriptor in bytes
        USB_DESCRIPTOR_INTERFACE,               // INTERFACE descriptor type
        1,                      // Interface Number
        0,                      // Alternate Setting Number
        2,                      // Number of endpoints in this intf
        0x03,                   // Class code
        0x00,                   // Subclass code
        0x00,                   // Protocol code
        0,                      // Interface string index
    }, {
        /* HID Class-Specific Descriptor */
        0x09,               // Size of this descriptor in bytes RRoj hack
        0x21,                // HID descriptor type
        0x0111,                 // HID Spec Release Number in BCD format (1.11)
        0x00,                   // Country Code (0x00 for Not supported)
        0x01,         // Number of class descriptors, see usbcfg.h
        0x22,                // Report descriptor type
        0x001b,           // Size of the report descriptor
    }, {
        /* Endpoint Descriptor */
        0x07,/*sizeof(USB_EP_DSC)*/
        USB_DESCRIPTOR_ENDPOINT,    //Endpoint Descriptor
        0x84,            //EndpointAddress
        0x03,                       //Attributes
        0x0040,                  //size
        0x01                        //Interval
    }, {
        /* Endpoint Descriptor */
        0x07,/*sizeof(USB_EP_DSC)*/
        USB_DESCRIPTOR_ENDPOINT,    //Endpoint Descriptor
        0x04,            //EndpointAddress
        0x03,                       //Attributes
        0x0040,                  //size
        0x01                        //Interval
    },
    // IF3
    {
        /* Interface Descriptor */
        0x09,//sizeof(USB_INTF_DSC),   // Size of this descriptor in bytes
        USB_DESCRIPTOR_INTERFACE,               // INTERFACE descriptor type
        2,                      // Interface Number
        0,                      // Alternate Setting Number
        1,                      // Number of endpoints in this intf
        0x03,                   // Class code
        0x00,                   // Subclass code
        0x00,                   // Protocol code
        0,                      // Interface string index
    }, {
        /* HID Class-Specific Descriptor */
        0x09,               // Size of this descriptor in bytes RRoj hack
        0x21,                // HID descriptor type
        0x0111,                 // HID Spec Release Number in BCD format (1.11)
        0x00,                   // Country Code (0x00 for Not supported)
        0x01,         // Number of class descriptors, see usbcfg.h
        0x22,                // Report descriptor type
        0x0015,           // Size of the report descriptor
    }, {
        /* Endpoint Descriptor */
        0x07,/*sizeof(USB_EP_DSC)*/
        USB_DESCRIPTOR_ENDPOINT,    //Endpoint Descriptor
        0x83,            //EndpointAddress
        0x03,                       //Attributes
        0x0040,                  //size
        0x01                        //Interval
    }, 
    // last one
    {
        /* Interface Descriptor */
        0x09,//sizeof(USB_INTF_DSC),   // Size of this descriptor in bytes
        USB_DESCRIPTOR_INTERFACE,               // INTERFACE descriptor type
        3,                      // Interface Number
        0,                      // Alternate Setting Number
        1,                      // Number of endpoints in this intf
        0x03,                   // Class code
        0x00,                   // Subclass code
        0x00,                   // Protocol code
        0,                      // Interface string index
    }, {
        /* HID Class-Specific Descriptor */
        0x09,               // Size of this descriptor in bytes RRoj hack
        0x21,                // HID descriptor type
        0x0111,                 // HID Spec Release Number in BCD format (1.11)
        0x00,                   // Country Code (0x00 for Not supported)
        0x01,         // Number of class descriptors, see usbcfg.h
        0x22,                // Report descriptor type
        0x0015,           // Size of the report descriptor
    }, {
        /* Endpoint Descriptor */
        0x07,/*sizeof(USB_EP_DSC)*/
        USB_DESCRIPTOR_ENDPOINT,    //Endpoint Descriptor
        0x81,            //EndpointAddress
        0x03,                       //Attributes
        0x0040,                  //size
        0x01                        //Interval
    }, 

};


const char* configuration = (const char*)& configuration_hid;
const USB_INTERFACE_DESCRIPTOR* interfaces[] = { &configuration_hid.dev_int0, &configuration_hid.dev_int1, &configuration_hid.dev_int2 };

const unsigned char string_0[] =
{
    0x04,
    USB_DESCRIPTOR_STRING,
    0x09,
    0x04
};

const unsigned char string_1[] =  // Manufacturer
{
    0x10,
    USB_DESCRIPTOR_STRING,
    'G', 0x00,
    'e', 0x00,
    'n', 0x00,
    'e', 0x00,
    'r', 0x00,
    'i', 0x00,
    'c', 0x00,
};

const unsigned char string_2[] =  // Product
{
    84,
    USB_DESCRIPTOR_STRING, // bLength, bDscType
    'C', 0x00,
    'O', 0x00,
    'R', 0x00,
    'S', 0x00,
    'A', 0x00,
    'I', 0x00,
    'R', 0x00,
    ' ', 0x00,
    'H', 0x00,
    'A', 0x00,
    'R', 0x00,
    'P', 0x00,
    'O', 0x00,
    'O', 0x00,
    'N', 0x00,
    ' ', 0x00,
    'R', 0x00,
    'G', 0x00,
    'B', 0x00,
    ' ', 0x00,
    'W', 0x00,
    'I', 0x00,
    'R', 0x00,
    'E', 0x00,
    'L', 0x00,
    'E', 0x00,
    'S', 0x00,
    'S', 0x00,
    ' ', 0x00,
    'G', 0x00,
    'a', 0x00,
    'm', 0x00,
    'i', 0x00,
    'n', 0x00,
    'g', 0x00,
    ' ', 0x00,
    'M', 0x00,
    'o', 0x00,
    'u', 0x00,
    's', 0x00,
    'e', 0x00,
};

const unsigned char string_3[] =  // Serial
{
    0x5a,
    USB_DESCRIPTOR_STRING,
    '7', 0x00,
    'A', 0x00,
    'A', 0x00,
    'B', 0x00,
    '4', 0x00,
    'F', 0x00,
    '4', 0x00,
    'F', 0x00,
    'A', 0x00,
    'F', 0x00,
    '2', 0x00,
    '6', 0x00,
    '3', 0x00,
    '2', 0x00,
    '7', 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
};

const unsigned char string_4[] =
{
    0x36, 0x03, 0x42, 0x00, 0x6f, 0x00, 0x6f, 0x00, 0x74, 0x00, 0x6c, 0x00, 0x6f, 0x00, 0x61, 0x00, 0x64, 0x00, 0x65, 0x00, 0x72, 0x00, 0x20, 0x00, 0x76, 0x00, 0x65, 0x00, 0x72, 0x00, 0x73, 0x00, 0x69, 0x00, 0x6f, 0x00, 0x6e, 0x00, 0x3a, 0x00, 0x20, 0x00, 0x30, 0x00, 0x2e, 0x00, 0x35, 0x00, 0x2e, 0x00, 0x33, 0x00, 0x36, 0x00
};
const unsigned char string_5[] =
{
    0x0A,
    USB_DESCRIPTOR_STRING,
    'B', 0x00,
    'P', 0x00,
    '0', 0x00,
    '0', 0x00,
};

const unsigned char* strings[] = {string_0, string_1, string_2, string_3, string_4, string_5};
const USB_DEVICE_QUALIFIER_DESCRIPTOR  dev_qua = {};


//Class specific descriptor - HID keyboard
const byte keyboard_report[60] =
{
    0x05, 0x01, 0x09, 0x02, 0xA1, 0x01, 0x09, 0x01, 0xA1, 0x00, 0x05, 0x09, 0x19, 0x01, 0x29, 0x20,
    0x15, 0x00, 0x25, 0x01, 0x95, 0x20, 0x75, 0x01, 0x81, 0x02, 0x75, 0x10, 0x95, 0x02, 0x05, 0x01,
    0x09, 0x30, 0x09, 0x31, 0x16, 0x01, 0x80, 0x26, 0xFF, 0x7F, 0x81, 0x06, 0x75, 0x08, 0x95, 0x01,
    0x05, 0x01, 0x09, 0x38, 0x15, 0x81, 0x25, 0x7F, 0x81, 0x06, 0xC0, 0xC0
};

const byte corsair_report[27] =
{
    0x06, 0x42, 0xFF, 0x09, 0x01, 0xA1, 0x01, 0x15, 0x00, 0x26, 0xFF, 0x00, 0x75, 0x08, 0x95, 0x40,
    0x09, 0x01, 0x81, 0x02, 0x95, 0x40, 0x09, 0x01, 0x91, 0x02, 0xC0
};

const byte corsair_report2[21] =
{
    0x06, 0x42, 0xFF, 0x09, 0x02, 0xA1, 0x01, 0x15, 0x00, 0x26, 0xFF, 0x00, 0x75, 0x08, 0x95, 0x40,
    0x09, 0x02, 0x81, 0x02, 0xC0
};

#define BSIZE 64
char buffer[BSIZE + 1];
int  bsize = 0;
int  seqnum84 = 0;
int  seqnum83 = 0;
int  seqnum82 = 0;
int opmode = 1;
char response[BSIZE + 1] = {0};
void handle_data(int sockfd, USBIP_RET_SUBMIT* usb_req, int bl)
{
    if(usb_req->ep == 0x04)
    {
        if(usb_req->direction == 0) // Host Out
        {
            bsize = recv(sockfd, (char*) buffer , bl, 0);
            for(int i = 0; i < bl; i++)
                printf("%02x ", (unsigned char)buffer[i]);
            puts("");
            send_corsair_bragi_req(sockfd, usb_req, "", 0x40, 0);
            // Wipe the previous buffer
            memset(response, 0, 0x40);
            if(buffer[0] == 0x08)
            {
                printf("SSEQNUM %d\n", seqnum84);
                usb_req->seqnum = seqnum84;
                response[0] = 0x00;
                response[1] = buffer[1];
                response[2] = 0x00;
                if(buffer[1] == 0x02) // GET
                {
                    if(buffer[2] == 0x03) // "Operating mode"
                    {
                        response[3] = opmode;
                    }
                    else if(buffer[2] == 0x5f) // "MultipointConnectionSupport" according to CUE logs
                    {
                        //response[3] = 0x02; // When only this is set, CUE doesn't show the device at all. Possibly thinks it's disconnected?
                        response[2] = 0x05; // No idea what this means
                        response[3] = 0x40; // No idea what this means
                    }
                    else if(buffer[2] == 0x01) // "Pollrate"
                    {
                        response[3] = 0x04; // Pollrate
                    }
                    else if(buffer[2] == 0x13) // "FW ver"
                    {
                        response[3] = 0x01;
                        response[4] = 0x10;
                        response[5] = 0x6b;
                    }
                    else if(buffer[2] == 0x14) // "BLD ver"
                    {
                        response[3] = 0x00;
                        response[4] = 0x07;
                        response[5] = 0x1c;
                    }
                    else if(buffer[2] == 0x11) // "VID"
                    {
                        response[4] = 0x1b; // VID
                        response[3] = 0x1c; // VID
                    }
                    else if(buffer[2] == 0x12) // "PID"
                    {
                        response[3] = 0x4c; // PID
                        response[4] = 0x1b; // PID
                    }
                    else
                    {
                        printf("!!!!Unhandled GET!!!!\n");
                        response[2] = buffer[2];
                        response[3] = buffer[3];
                    }
                }
                else if(buffer[1] == 0x0d) // Download file to computer
                {
                    if(buffer[2] == 0x00 && buffer[3] == 0x05) // Pairing ID
                    {
                        // Wireshark capture has all zeroes here. Odd
                        //memset(response + 2, 0, 40); // Random size that's large enough to wipe junk
                    }
                    else
                        printf("!!!!Unhandled file download!!!!\n");
                }
                else if(buffer[1] == 0x09) // No idea
                {
                    response[3] = 0x08;
                    response[4] = 0x0e;
                    response[5] = 0x08;
                }
                else if(buffer[1] == 0x05) // No idea
                {
                    //memset(response + 2, 0, 40); // Random size that's large enough to wipe junk
                }
                else if (buffer[1] == 0x01) // SET
                {
                    if(buffer[2] == 0x03) // op mode
                    {
                        opmode = buffer[4];
                    }
                }
                else if(buffer[1] == 0x08) // Unknown. Returns some data. "read chunk"?
                {
                    if(buffer[2] == 0x00)
                    {
                        response[3] = 0x5a;
                        response[4] = 0x36;
                        response[5] = 0xbc;
                        response[6] = 0xd1;
                        response[7] = 0xe8;
                        response[8] = 0x5f;
                        response[9] = 0x4b;
                        response[10] = 0x76;
                    }
                    else
                        printf("Unhandled 0x08 command\n");
                }
                else
                {
                    printf("!!!!UNHANDLED!!!!!\n");
                }
                send_corsair_response(sockfd, usb_req, response, 64, 0, 0x84);
            }
        }
        else if(usb_req->direction == 1)
        {
            seqnum84 = usb_req->seqnum;
        }
    }
    else if(usb_req->ep == 0x03)
    {
        seqnum83 = usb_req->seqnum;
    }
    else if(usb_req->ep == 0x02)
    {
        seqnum82 = usb_req->seqnum;
    }
};

void handle_unknown_control(int sockfd, StandardDeviceRequest* control_req, USBIP_RET_SUBMIT* usb_req)
{
    if(control_req->bmRequestType == 0x81)
    {
        if(control_req->bRequest == 0x6)  //# Get Descriptor
        {
            if(control_req->wValue1 == 0x22)  // Send HID Reports
            {
                printf("Sending HID Report for IF%01x\n", control_req->wIndex0);
                if(control_req->wIndex0 == 0x00)
                    send_usb_req(sockfd, usb_req, (char*) keyboard_report, 60, 0);
                else if(control_req->wIndex0 == 0x01)
                    send_usb_req(sockfd, usb_req, (char*) corsair_report, 27, 0);
                else if(control_req->wIndex0 == 0x02)
                    send_usb_req(sockfd, usb_req, (char*) corsair_report2, 21, 0);
                else if(control_req->wIndex0 == 0x03)
                    send_usb_req(sockfd, usb_req, (char*) corsair_report2, 21, 0);
                    //send_usb_req(sockfd, usb_req, (char*) keyboard_report, 106, 0);
                //else if(control_req->wIndex0 == 0x02)
                //    send_usb_req(sockfd, usb_req, (char*) corsair_report_if2, 25, 0);
                else
                    puts("Unknown interface");
            }
        }
    }
    if(control_req->bmRequestType == 0x21)  // Host Request
    {
        if(control_req->bRequest == 0x0a)  // set idle
        {
            printf("Idle\n");
            // Idle
            send_usb_req(sockfd, usb_req, "", 0, 0);
        }
        if(control_req->bRequest == 0x09)  // set report
        {
            printf("set report\n");
            char data[64];
            if ((recv (sockfd, data , control_req->wLength, 0)) != control_req->wLength)
            {
                printf ("receive error : %s \n", strerror (errno));
                exit(-1);
            };
            char buffer2[BSIZE] = { 0 };
            send_ctrl_response(sockfd, usb_req, buffer2, control_req->wLength, 0);
        }
    }
};

int main(int argc, char* argv[])
{
    printf("hid mouse started....\n");
    usbip_run(&dev_dsc);
}

