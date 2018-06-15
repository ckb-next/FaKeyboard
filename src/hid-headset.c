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
    0x1b29,                 // Product ID: Mouse in a circle fw demo
    0x0007,                 // Device release number in BCD format
    0x01,                   // Manufacturer string index
    0x02,                   // Product string index
    0x03,                   // Device serial number string index
    0x01                    // Number of possible configurations
};

/* Configuration 1 Descriptor */
const CONFIG_HID_HEADSET  configuration_hid = {{
        /* Configuration Descriptor */
        0x09,//sizeof(USB_CFG_DSC),    // Size of this descriptor in bytes
        USB_DESCRIPTOR_CONFIGURATION,                // CONFIGURATION descriptor type
        0x00af, // with alt: 0x013e,                 // Total length of data for this cfg
        4,                      // Number of interfaces in this cfg
        1,                      // Index value of this configuration
        0,                      // Configuration string index
        0xa0,
        50,                    // Max power consumption (2X mA)
    }, {
        /* Interface Descriptor */
        0x09,//sizeof(USB_INTF_DSC),   // Size of this descriptor in bytes
        USB_DESCRIPTOR_INTERFACE,               // INTERFACE descriptor type
        0,                      // Interface Number
        0,                      // Alternate Setting Number
        0,                      // Number of endpoints in this intf
        0x01,                   // Class code
        0x01,                   // Subclass code
        0x00,                   // Protocol code
        0,                      // Interface string index
    },
    { 10, 36, 1, 0x0100, 114, 2, 1, 2 },            // header
    { 12, 36, 2, 10, 0x0201, 0, 2, 0x0003, 0, 6 },  // input terminal mic
    { 10, 36, 6, 11, 1, 1, 3, 0, 0, 0 },            // feature unit
    { 12, 36, 2, 1, 0x0402, 6, 2, 0x0003, 0, 0 },   // input terminal headset
    { 10, 36, 6, 2, 1, 1, 0x43, 0, 0, 0 },          // feature unit
    {  7, 36, 5, 15, 1, 2, 0 },                     // selector unit
    {  9, 36, 3, 3, 0x0101, 0, 15, 0 },             // output terminal streaming
    { 12, 36, 2, 4, 0x0101, 0, 2, 0x0003, 0, 0 },   // input terminal streaming
    { 10, 36, 6, 5, 12, 1, 1, 2, 2, 0 },            // feature unit
    {  9, 36, 3, 6, 0x0402, 1, 5, 0 },              // output terminal headset
    { 13, 36, 4, 12, 2, 4, 11, 2, 0x0003, 0, 0, 0}, // mixer unit
    // IF 2
    {
        /* Interface Descriptor */
        0x09,//sizeof(USB_INTF_DSC),   // Size of this descriptor in bytes
        USB_DESCRIPTOR_INTERFACE,               // INTERFACE descriptor type
        1,                      // Interface Number
        0,                      // Alternate Setting Number
        0,                      // Number of endpoints in this intf
        0x01,                   // Class code
        0x02,                   // Subclass code
        0x00,                   // Protocol code
        0,                      // Interface string index
    }, 
    //IF 2 Alternate
    /*{
        0x09,//sizeof(USB_INTF_DSC),   // Size of this descriptor in bytes
        USB_DESCRIPTOR_INTERFACE,               // INTERFACE descriptor type
        1,                      // Interface Number
        1,                      // Alternate Setting Number
        1,                      // Number of endpoints in this intf
        0x01,                   // Class code
        0x02,                   // Subclass code
        0x00,                   // Protocol code
        0,                      // Interface string index
    },*/
    //{
        /* AudioStreaming Descriptor goes here */
    //}
    // IF 3
    {
        /* Interface Descriptor */
        0x09,//sizeof(USB_INTF_DSC),   // Size of this descriptor in bytes
        USB_DESCRIPTOR_INTERFACE,               // INTERFACE descriptor type
        2,                      // Interface Number
        0,                      // Alternate Setting Number
        0,                      // Number of endpoints in this intf
        0x01,                   // Class code
        0x02,                   // Subclass code
        0x00,                   // Protocol code
        0,                      // Interface string index
    },
    // IF 4
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
    }, 
    {
        9,               // Size of this descriptor in bytes RRoj hack
        33,                // HID descriptor type
        0x0111,                 // HID Spec Release Number in BCD format (1.11)
        0x00,                   // Country Code (0x00 for Not supported)
        0x01,         // Number of class descriptors, see usbcfg.h
        34,                // Report descriptor type
        0x0134,           // Size of the report descriptor
    },
    {
        /* Endpoint Descriptor */
        0x07,/*sizeof(USB_EP_DSC)*/
        USB_DESCRIPTOR_ENDPOINT,    //Endpoint Descriptor
        0x84,            //EndpointAddress
        0x03,                       //Attributes
        0x0025,                  //size
        0x01                        //Interval
    }
};


const char* configuration = (const char*)& configuration_hid;
const USB_INTERFACE_DESCRIPTOR* interfaces[] = { &configuration_hid.dev_int0, &configuration_hid.dev_int1, &configuration_hid.dev_int2, &configuration_hid.dev_int3 };

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
    'G', 0x00,
    'a', 0x00,
    'm', 0x00,
    'i', 0x00,
    'n', 0x00,
    'g', 0x00,
    ' ', 0x00,
    'H', 0x00,
    'e', 0x00,
    'a', 0x00,
    'd', 0x00,
    's', 0x00,
    'e', 0x00,
    't', 0x00,
    ' ', 0x00,
    ' ', 0x00,
    'N', 0x00,
    'u', 0x00,
    'l', 0x00,
    'l', 0x00,
    ' ', 0x00,
    ' ', 0x00,
    ' ', 0x00,
    ' ', 0x00,
};

const unsigned char string_3[] =  // Serial
{
    0x5a,
    USB_DESCRIPTOR_STRING,
    '1', 0x00,
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

const unsigned char* strings[] = {string_0, string_1, string_2, string_3, string_4, string_4, string_4};
const USB_DEVICE_QUALIFIER_DESCRIPTOR  dev_qua = {};

const byte corsair_report[308] =
{
    0x05, 0x0C, 0x09, 0x01, 0xA1, 0x01, 0x85, 0x01, 0x15, 0x00, 0x25, 0x01, 0x75, 0x01, 0x09, 0xE9,
    0x09, 0xEA, 0x95, 0x02, 0x81, 0x06, 0x95, 0x06, 0x81, 0x01, 0x85, 0x02, 0x05, 0x0C, 0x09, 0x00,
    0x95, 0x10, 0x81, 0x02, 0x85, 0x04, 0x09, 0x00, 0x75, 0x08, 0x95, 0x24, 0x91, 0x02, 0x85, 0x05,
    0x09, 0x00, 0x95, 0x20, 0x81, 0x02, 0x85, 0x06, 0x09, 0x00, 0x95, 0x24, 0x91, 0x02, 0x85, 0x07,
    0x09, 0x00, 0x95, 0x20, 0x81, 0x02, 0xC0, 0x06, 0xC5, 0xFF, 0x09, 0x01, 0xA1, 0x01, 0x09, 0x64,
    0xA1, 0x02, 0x85, 0x64, 0x09, 0x69, 0x09, 0x6A, 0x09, 0x6B, 0x09, 0x6C, 0x09, 0x6D, 0x09, 0x6E,
    0x09, 0x6F, 0x75, 0x01, 0x95, 0x07, 0x81, 0x02, 0x95, 0x01, 0x81, 0x03, 0x09, 0x70, 0x75, 0x07,
    0x25, 0x64, 0x81, 0x22, 0x75, 0x01, 0x81, 0x03, 0x09, 0x71, 0x75, 0x04, 0x25, 0x0D, 0x81, 0x22,
    0x09, 0x72, 0x09, 0x73, 0x09, 0x74, 0x75, 0x01, 0x95, 0x03, 0x25, 0x01, 0x81, 0x02, 0x95, 0x01,
    0x81, 0x03, 0x09, 0x75, 0x75, 0x03, 0x25, 0x06, 0x81, 0x22, 0x75, 0x01, 0x95, 0x05, 0x81, 0x03,
    0xC0, 0x09, 0x65, 0xA1, 0x02, 0x85, 0x65, 0x09, 0xD5, 0x09, 0xD6, 0x75, 0x08, 0x95, 0x02, 0x26,
    0xFF, 0x00, 0x81, 0x22, 0xC0, 0x09, 0x66, 0xA1, 0x02, 0x85, 0x66, 0x09, 0x78, 0x09, 0x79, 0x09,
    0x7A, 0x09, 0x7B, 0x75, 0x08, 0x95, 0x04, 0x26, 0xFF, 0x00, 0x81, 0x22, 0xC0, 0x09, 0x67, 0xA1,
    0x02, 0x85, 0x67, 0x09, 0x90, 0x75, 0x08, 0x95, 0x09, 0x26, 0xFF, 0x00, 0x81, 0x22, 0xC0, 0x09,
    0xC8, 0xA1, 0x02, 0x85, 0xC8, 0x09, 0xDB, 0x75, 0x08, 0x95, 0x02, 0x26, 0xFF, 0x00, 0x91, 0x22,
    0xC0, 0x09, 0xC9, 0xA1, 0x02, 0x85, 0xC9, 0x09, 0xD4, 0x75, 0x08, 0x95, 0x01, 0x26, 0xFF, 0x00,
    0x91, 0x22, 0xC0, 0x09, 0xCA, 0xA1, 0x02, 0x85, 0xCA, 0x09, 0xDA, 0x75, 0x08, 0x95, 0x04, 0x26,
    0xFF, 0x00, 0x91, 0x22, 0xC0, 0x09, 0xCB, 0xA1, 0x02, 0x85, 0xCB, 0x09, 0xD2, 0x75, 0x08, 0x95,
    0x01, 0x91, 0x22, 0x09, 0xD3, 0xA1, 0x02, 0x05, 0x0A, 0x19, 0x01, 0x29, 0x12, 0x95, 0x12, 0x91,
    0x02, 0xC0, 0xC0, 0xC0
};

#define BSIZE 64
char buffer[BSIZE + 1];
int  bsize = 0;
int  seqnum81 = 0;
int  seqnum82 = 0;
int  seqnum03 = 0;

void handle_data(int sockfd, USBIP_RET_SUBMIT* usb_req, int bl)
{
    if(usb_req->ep == 0x02)
    {
        if(usb_req->direction == 0) // Host Out
        {
            printf("direction=input\n");
            bsize = recv(sockfd, (char*) buffer , bl, 0);
            for(int i = 0; i < bl; i++)
                printf("%02x ", (unsigned char)buffer[i]);
            puts("");
            char emptybuf[64] = { 0 };
            send_corsair_req(sockfd, usb_req, emptybuf, 0x40, 0);
        }
        // Host In
        //else
        //{
            // Gets stuck here
            /*for(int i = 0; i < bl; i++)
                printf("%02x ", buffer[i]);
            puts("");
            char buffer2[BSIZE] = { 0 };*/
            //send_corsair_req_in(sockfd, usb_req, buffer2, 0, 0);
        //}
        usleep(500);
    }
    else if(usb_req->ep == 0x01)
    {
        seqnum81 = usb_req->seqnum;
    }
    //else if(usb_req->ep == 0x02)
    //{
    //    seqnum82 = usb_req->seqnum;
    //}
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
                //if(control_req->wIndex0 == 0x00)
                    //send_usb_req(sockfd, usb_req, (char*) keyboard_report, 106, 0);
                //else 
                if(control_req->wIndex0 == 0x03)
                    send_usb_req(sockfd, usb_req, (char*) corsair_report, 308, 0);
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
          /*  if(control_req->wValue1 == 0x02 && control_req->wValue0 == 0xc8)
            {
                char buffer[3] = { 0xc8, 0x01, 0x00 };
                send_ctrl_response(sockfd, usb_req, (char*)buffer, 3, 1);
                return;
            }
            else
            {*/
        char data[64];
        printf("set report\n");
        printf("wValue0: %x, wValue1: %x\n", control_req->wValue0, control_req->wValue1);
        if ((recv (sockfd, &data , control_req->wLength, 0)) != control_req->wLength)
        {
            printf ("receive error : %s \n", strerror (errno));
            exit(-1);
        };
        printf("Packet from CUE len %i: ", control_req->wLength);
        for(int i = 0; i < control_req->wLength; i++)
        {
            printf("%02x ", (unsigned char)data[i]);
        }
        puts("");
        
        if((unsigned char)data[0] == 0xcb && (unsigned char)data[1] == 0x01 && (unsigned char)data[2] == 0x01)
        {
            data[3] = 0x15;
            send_ctrl_response(sockfd, usb_req, data, control_req->wLength, 0);
        }
        else if((unsigned char)data[0] == 0xc9 && (unsigned char)data[1] == 0x66)
        {
            data[0] = 0x15;
            data[1] = 0x15;
            send_ctrl_response(sockfd, usb_req, data, control_req->wLength, 0);
        }
        else
        {
            char buffer2[BSIZE] = { 0 };
            send_ctrl_response(sockfd, usb_req, buffer2, control_req->wLength, 0);
        }
           // }
    }
        /*if(control_req->bRequest == 0x01)
        {
            if(control_req->wValue1 == 0x02)
            {
                puts("\n\n\n\n02 ctrl response\n\n\n\n");
                char buffer2[2] = { 0x0b, 0x02 };
                send_ctrl_response(sockfd, usb_req, buffer2, 2, 0);
            }
        }*/
        // Unknown packets
        
    }
};

int main(int argc, char* argv[])
{
    printf("hid mouse started....\n");
    usbip_run(&dev_dsc);
}

