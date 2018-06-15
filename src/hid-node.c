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
    0x0c0b,                 // Product ID: Mouse in a circle fw demo
    0x0005,                 // Device release number in BCD format
    0x01,                   // Manufacturer string index
    0x02,                   // Product string index
    0x03,                   // Device serial number string index
    0x01                    // Number of possible configurations
};

/* Configuration 1 Descriptor */
const CONFIG_HID_NODE  configuration_hid = {{
        /* Configuration Descriptor */
        0x09,//sizeof(USB_CFG_DSC),    // Size of this descriptor in bytes
        USB_DESCRIPTOR_CONFIGURATION,                // CONFIGURATION descriptor type
        0x0029,                 // Total length of data for this cfg
        1,                      // Number of interfaces in this cfg
        1,                      // Index value of this configuration
        0,                      // Configuration string index
        0xc0,
        25,                    // Max power consumption (2X mA)
    }, {
        /* Interface Descriptor */
        0x09,//sizeof(USB_INTF_DSC),   // Size of this descriptor in bytes
        USB_DESCRIPTOR_INTERFACE,               // INTERFACE descriptor type
        0,                      // Interface Number
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
        0x01,        // Number of class descriptors, see usbcfg.h
        0x22,                // Report descriptor type
        0x0021,           // Size of the report descriptor
    }, {
        /* Endpoint Descriptor */
        0x07,/*sizeof(USB_EP_DSC)*/
        USB_DESCRIPTOR_ENDPOINT,    //Endpoint Descriptor
        0x81,            //EndpointAddress
        0x03,                       //Attributes
        0x0040,                  //size
        0x01                        //Interval
    }, {
        /* Endpoint Descriptor */
        0x07,/*sizeof(USB_EP_DSC)*/
        USB_DESCRIPTOR_ENDPOINT,    //Endpoint Descriptor
        0x01,            //EndpointAddress
        0x03,                       //Attributes
        0x0040,                  //size
        0x01                        //Interval
    },
};


const char* configuration = (const char*)& configuration_hid;
const USB_INTERFACE_DESCRIPTOR* interfaces[] = { &configuration_hid.dev_int0 };

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
    // Should be Corsair Components, Inc.
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
    'N', 0x00,
    'o', 0x00,
    'd', 0x00,
    'e', 0x00,
    ' ', 0x00,
    'T', 0x00,
    'h', 0x00,
    'i', 0x00,
    'n', 0x00,
    'g', 0x00,
    ' ', 0x00,
    ' ', 0x00,
    ' ', 0x00,
    ' ', 0x00,
    ' ', 0x00,
    ' ', 0x00,
    ' ', 0x00,
};

const unsigned char string_3[] =  // Serial
{
    0x5a,
    USB_DESCRIPTOR_STRING,
    '2', 0x00,
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
    '8', 0x00,
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
    '3', 0x00,
};

const unsigned char* strings[] = {string_0, string_1, string_2, string_3, string_4};
const USB_DEVICE_QUALIFIER_DESCRIPTOR  dev_qua = {};


const byte corsair_report[33] =
{
  0x06, 0xc2, 0xff, 0x09, 0x01, 0xa1, 0x01, 0x15, 0x00, 0x26, 0xff, 0x00, 0x75, 0x08, 0x95, 0x10,
  0x09, 0x01, 0x81, 0x02, 0x95, 0x40, 0x09, 0x01, 0x91, 0x02, 0x95, 0x00, 0x09, 0x01, 0xb1, 0x02,
  0xc0
};

#define BSIZE 64
char buffer[BSIZE + 1];
int  bsize = 0;
int  seqnum81 = 0;
int  seqnum82 = 0;
int  seqnum03 = 0;

void handle_data(int sockfd, USBIP_RET_SUBMIT* usb_req, int bl)
{
    if(usb_req->ep == 0x01)
    {
        if(usb_req->direction == 0) // Host Out
        {
            printf("direction=input\n");
            bsize = recv(sockfd, (char*) buffer , bl, 0);
            for(int i = 0; i < bl; i++)
                printf("%02x ", (unsigned char)buffer[i]);
            puts("");
            char dummybuffer[64] = {0};
            send_corsair_req(sockfd, usb_req, dummybuffer, 16, 0);
            if((unsigned char)buffer[0] == 0x03)
            {
                usb_req->seqnum = seqnum81;
                char response[BSIZE + 1] = { 0x00, 0xdb, 0x94, 0x7c, 0xea, 0x00 };
                send_corsair_response(sockfd, usb_req, response, 16, 0, 0x81);
            }
            else if((unsigned char)buffer[0] == 0x02)
            {
                usb_req->seqnum = seqnum81;
                char response[BSIZE + 1] = { 0x00, 0x00, 0x05, 0x68, 0xea, 0x00 };
                send_corsair_response(sockfd, usb_req, response, 16, 0, 0x81);
            }
            else if((unsigned char)buffer[0] == 0x06)
            {
                usb_req->seqnum = seqnum81;
                char response[BSIZE + 1] = { 0x00, 0x02, 0x00 };
                send_corsair_response(sockfd, usb_req, response, 16, 0, 0x81);
            }
            /*else if((unsigned char)buffer[0] == 0x37)
            {
                usb_req->seqnum = seqnum81;
                char response[BSIZE + 1] = { 0x00, 0x02, 0x00 };
                send_corsair_response(sockfd, usb_req, response, 16, 0, 0x81);
            }*/
            else // 0x37 
            {
                usb_req->seqnum = seqnum81;
                send_corsair_response(sockfd, usb_req, dummybuffer, 16, 0, 0x81);
            }
        }
        else
            seqnum81 = usb_req->seqnum;
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
    else if(usb_req->ep == 0x02)
    {
        seqnum82 = usb_req->seqnum;
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
                if(control_req->wIndex0 == 0x00)
                    send_usb_req(sockfd, usb_req, (char*) corsair_report, 33, 0);
                //else if(control_req->wIndex0 == 0x01)
                //    send_usb_req(sockfd, usb_req, (char*) corsair_report2, 24, 0);
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
            char data[20];
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
    printf("hid mousepad started....\n");
    usbip_run(&dev_dsc);
}

