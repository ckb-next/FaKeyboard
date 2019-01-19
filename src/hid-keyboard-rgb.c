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

int windows_client = 0;

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
    0x1b20,                 // Product ID: Mouse in a circle fw demo
    0x0205,                 // Device release number in BCD format
    0x01,                   // Manufacturer string index
    0x02,                   // Product string index
    0x03,                   // Device serial number string index
    0x01                    // Number of possible configurations
};

/* Configuration 1 Descriptor */
const CONFIG_HID  configuration_hid = {{
        /* Configuration Descriptor */
        0x09,//sizeof(USB_CFG_DSC),    // Size of this descriptor in bytes
        USB_DESCRIPTOR_CONFIGURATION,                // CONFIGURATION descriptor type
        0x0054,                 // Total length of data for this cfg
        3,                      // Number of interfaces in this cfg
        1,                      // Index value of this configuration
        0,                      // Configuration string index
        0xa0,
        250,                    // Max power consumption (2X mA)
    }, {
        /* Interface Descriptor */
        0x09,//sizeof(USB_INTF_DSC),   // Size of this descriptor in bytes
        USB_DESCRIPTOR_INTERFACE,               // INTERFACE descriptor type
        0,                      // Interface Number
        0,                      // Alternate Setting Number
        1,                      // Number of endpoints in this intf
        0x03,                   // Class code
        0x01,                   // Subclass code
        0x01,                   // Protocol code
        0,                      // Interface string index
    }, {
        /* HID Class-Specific Descriptor */
        0x09,               // Size of this descriptor in bytes RRoj hack
        0x21,                // HID descriptor type
        0x0111,                 // HID Spec Release Number in BCD format (1.11)
        0x00,                   // Country Code (0x00 for Not supported)
        0x01,        // Number of class descriptors, see usbcfg.h
        0x22,                // Report descriptor type
        0x0043,           // Size of the report descriptor
    }, {
        /* Endpoint Descriptor */
        0x07,/*sizeof(USB_EP_DSC)*/
        USB_DESCRIPTOR_ENDPOINT,    //Endpoint Descriptor
        0x81,            //EndpointAddress
        0x03,                       //Attributes
        0x0008,                  //size
        0x08                        //Interval
    },
    // IF 2
    {
        /* Interface Descriptor */
        0x09,//sizeof(USB_INTF_DSC),   // Size of this descriptor in bytes
        USB_DESCRIPTOR_INTERFACE,               // INTERFACE descriptor type
        1,                      // Interface Number
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
        0x0070,           // Size of the report descriptor
    }, {
        /* Endpoint Descriptor */
        0x07,/*sizeof(USB_EP_DSC)*/
        USB_DESCRIPTOR_ENDPOINT,    //Endpoint Descriptor
        0x82,            //EndpointAddress
        0x03,                       //Attributes
        0x0040,                  //size
        0x01                        //Interval
    },
    // IF 3
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
        0x0019,           // Size of the report descriptor
    }, {
        /* Endpoint Descriptor */
        0x07,/*sizeof(USB_EP_DSC)*/
        USB_DESCRIPTOR_ENDPOINT,    //Endpoint Descriptor
        0x03,            //EndpointAddress
        0x03,                       //Attributes
        0x0040,                  //size
        0x01                        //Interval
    }
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
    0x42,
    USB_DESCRIPTOR_STRING, // bLength, bDscType
    'G', 0x00,
    'e', 0x00,
    'n', 0x00,
    'e', 0x00,
    'r', 0x00,
    'i', 0x00,
    'c', 0x00,
    ' ', 0x00,
    'F', 0x00,
    'a', 0x00,
    'k', 0x00,
    'e', 0x00,
    ' ', 0x00,
    ' ', 0x00,
    ' ', 0x00,
    'R', 0x00,
    'G', 0x00,
    'B', 0x00,
    ' ', 0x00,
    ' ', 0x00,
    ' ', 0x00,
    ' ', 0x00,
    'K', 0x00,
    'e', 0x00,
    'y', 0x00,
    'b', 0x00,
    'o', 0x00,
    'a', 0x00,
    'r', 0x00,
    'd', 0x00,
    ' ', 0x00,
    ' ', 0x00,
};

const unsigned char string_3[] =  // Serial
{
    0x5a,
    USB_DESCRIPTOR_STRING,
    '0', 0x00,
    '9', 0x00,
    '0', 0x00,
    '6', 0x00,
    '3', 0x00,
    '5', 0x00,
    '6', 0x00,
    '7', 0x00,
    '3', 0x00,
    '9', 0x00,
    '0', 0x00,
    '6', 0x00,
    '2', 0x00,
    '3', 0x00,
    '4', 0x00,
    '5', 0x00,
    '6', 0x00,
    '7', 0x00,
    '8', 0x00,
    '9', 0x00,
    '0', 0x00,
    '1', 0x00,
    '2', 0x00,
    '3', 0x00,
    '4', 0x00,
    '5', 0x00,
    '6', 0x00,
    '7', 0x00,
    '8', 0x00,
    '9', 0x00,
    '0', 0x00,
    '1', 0x00,
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

// No idea what this is
const unsigned char string_4[] =
{
    0x08,
    USB_DESCRIPTOR_STRING,
    'P', 0x00,
    '0', 0x00,
    '4', 0x00,
};

const unsigned char* strings[] = {string_0, string_1, string_2, string_3, string_4};
const USB_DEVICE_QUALIFIER_DESCRIPTOR  dev_qua = {};


//Class specific descriptor - HID keyboard
const byte keyboard_report[0x43] =
{
    0x05, 0x01, 0x09, 0x06, 0xA1, 0x01, 0x05, 0x07, 0x19, 0xE0, 0x29, 0xE7, 0x15, 0x00, 0x25, 0x01,
    0x75, 0x01, 0x95, 0x08, 0x81, 0x02, 0x75, 0x08, 0x95, 0x01, 0x81, 0x01, 0x05, 0x07, 0x19, 0x00,
    0x2A, 0xFF, 0x00, 0x15, 0x00, 0x26, 0xFF, 0x00, 0x75, 0x08, 0x95, 0x06, 0x81, 0x00, 0x05, 0x08,
    0x19, 0x01, 0x29, 0x03, 0x25, 0x01, 0x75, 0x01, 0x95, 0x03, 0x91, 0x02, 0x75, 0x05, 0x95, 0x01,
    0x91, 0x01, 0xC0
};

const byte corsair_report[112] =
{
    0x05, 0x01, 0x09, 0x06, 0xA1, 0x01, 0x85, 0x01, 0x05, 0x07, 0x19, 0xE0, 0x29, 0xE7, 0x15, 0x00,
    0x25, 0x01, 0x75, 0x01, 0x95, 0x08, 0x81, 0x02, 0x19, 0x00, 0x29, 0x97, 0x15, 0x00, 0x25, 0x01,
    0x75, 0x01, 0x95, 0x98, 0x81, 0x02, 0xC0, 0x05, 0x0C, 0x09, 0x01, 0xA1, 0x01, 0x85, 0x02, 0x05,
    0x0C, 0x19, 0x00, 0x2A, 0xFF, 0x0F, 0x15, 0x00, 0x26, 0xFF, 0x0F, 0x75, 0x10, 0x95, 0x02, 0x81,
    0x00, 0xC0, 0x06, 0xC0, 0xFF, 0x09, 0x02, 0xA1, 0x01, 0x85, 0x03, 0x15, 0x00, 0x26, 0xFF, 0x00,
    0x95, 0x3F, 0x75, 0x08, 0x09, 0x02, 0x81, 0x02, 0xC0, 0x06, 0xC2, 0xFF, 0x09, 0x03, 0xA1, 0x01,
    0x85, 0x0E, 0x15, 0x00, 0x26, 0xFF, 0x00, 0x95, 0x3F, 0x75, 0x08, 0x09, 0x03, 0x81, 0x02, 0xC0
};

const byte corsair_report_if2[25] =
{
    0x06, 0xc2, 0xff, 0x09, 0x04, 0xa1, 0x01, 0x15, 0x00, 0x26, 0xff, 0x00, 0x95, 0x40, 0x75, 0x08,
    0x09, 0x02, 0xb1, 0x02, 0x09, 0x04, 0x91, 0x02, 0xc0
};


#define BSIZE 64
char buffer[BSIZE + 1];
int  bsize = 0;
int  seqnum81 = 0;
int  seqnum82 = 0;
int  seqnum03 = 0;

void handle_data(int sockfd, USBIP_RET_SUBMIT* usb_req, int bl)
{
    if(usb_req->ep == 0x03)
    {
        if(usb_req->direction == 0) //input
        {
            printf("direction=input\n");
            bsize = recv(sockfd, (char*) buffer , bl, 0);
            for(int i = 0; i < bl; i++)
                printf("%02x ", (unsigned char)buffer[i]);
            puts("");
            char blankbuffer[64] = {0};
            send_corsair_req(sockfd, usb_req, blankbuffer, 0x40, 0);
            if(buffer[0] == 0x0e)
            {
                usb_req->seqnum = seqnum82;
                if(buffer[1] == 01)
                {
                    char response[BSIZE + 1] = { 0x0e, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x05, 0x02, 0x14, 0x00, 0x1c, 0x1b, 0x20, 0x1b, 0x01, 0x01, 0x01, 0x07, 0xc0, 0xff, 0x40, 0x01, 0x03, 0x00, 0x00, 0x00, 0x02, 0x04, 0x01, 0x00 };
                    send_corsair_response(sockfd, usb_req, response, 64, 0, 0x82);
                }
                else
                {
                    char response[BSIZE + 1] = { buffer[0], buffer[1], 0 };
                    send_corsair_response(sockfd, usb_req, response, 64, 0, 0x82);
                }
            }
        }
        else//not supported
            send_usb_req(sockfd, usb_req, "", 64, 0);
        usleep(500);
    }
    else if(usb_req->ep == 0x01)
    {
        seqnum81 = usb_req->seqnum;
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
                    send_usb_req(sockfd, usb_req, (char*) keyboard_report, 0x43, 0);
                else if(control_req->wIndex0 == 0x01)
                    send_usb_req(sockfd, usb_req, (char*) corsair_report, 112, 0);
                else if(control_req->wIndex0 == 0x02)
                    send_usb_req(sockfd, usb_req, (char*) corsair_report_if2, 25, 0);
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
            char data[20];
            if ((recv (sockfd, data , control_req->wLength, 0)) != control_req->wLength)
            {
                printf ("receive error : %s \n", strerror (errno));
                exit(-1);
            };
            send_ctrl_response(sockfd, usb_req, "", control_req->wLength, 0);
        }
    }
};

int main(int argc, char* argv[])
{
    printf("hid keyboard started....\n");
    usbip_run(&dev_dsc);
}

