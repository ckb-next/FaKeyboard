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

#ifndef VERBOSE_H
#define VERBOSE_H

#include <string>

namespace Verbose {
    void ERROR(const char* format, ...);
    void INFO(const char* format, ...);
    void INFO_VECTOR(const char* name, unsigned char* vector, int size);
};

#endif // VERBOSE_H