//
// Created by Robert Smallshire on 2019-05-23.
//

#include "display.h"

#include <IOKit/graphics/IOGraphicsLib.h>
#include <CoreVideo/CVBase.h>
#include <CoreVideo/CVDisplayLink.h>
#include <ApplicationServices/ApplicationServices.h>



// Returns the io_service_t corresponding to a CG display ID, or 0 on failure.
// The io_service_t should be released with IOObjectRelease when not needed.
//
io_service_t IOServicePortFromCGDisplayID(CGDirectDisplayID displayID)
{
    const int MAX_DISPLAYS = 16;
    CGDisplayCount    displayCount;
    CGDisplayCount    maxDisplays = MAX_DISPLAYS;
    CGDirectDisplayID onlineDisplays[MAX_DISPLAYS];
    CGDisplayErr dErr = CGGetOnlineDisplayList(maxDisplays, onlineDisplays, &displayCount);
    if (dErr != kCGErrorSuccess) {
        fprintf(stderr, "CGGetOnlineDisplayList: error %d.\n", dErr);
        exit(1);
    }

    io_iterator_t iter;
    io_service_t serv, servicePort = 0;

    CFMutableDictionaryRef matching = IOServiceMatching("IODisplayConnect");

    // releases matching for us
    kern_return_t err = IOServiceGetMatchingServices(kIOMasterPortDefault,
                                                     matching,
                                                     &iter);
    if (err)
    {
        return 0;
    }

    while ((serv = IOIteratorNext(iter)) != 0)
    {
        CFDictionaryRef info;
        CFIndex vendorID, productID, serialID;
        CFNumberRef vendorIDRef, productIDRef, serialIDRef;
        Boolean success;

        info = IODisplayCreateInfoDictionary(serv,
                                             kIODisplayOnlyPreferredName);

        vendorIDRef = CFDictionaryGetValue(info,
                                           CFSTR(kDisplayVendorID));
        productIDRef = CFDictionaryGetValue(info,
                                            CFSTR(kDisplayProductID));
        serialIDRef = CFDictionaryGetValue(info,
                                           CFSTR(kDisplaySerialNumber));

        success = CFNumberGetValue(vendorIDRef, kCFNumberCFIndexType,
                                   &vendorID);
        success &= CFNumberGetValue(productIDRef, kCFNumberCFIndexType,
                                    &productID);
        if (serialIDRef != 0) {
            success &= CFNumberGetValue(serialIDRef, kCFNumberCFIndexType,
                                    &serialID);
        }

        if (!success)
        {
            CFRelease(info);
            continue;
        }

        if (CGDisplayVendorNumber(displayID) != vendorID ||
            CGDisplayModelNumber(displayID) != productID)
        {
            CFRelease(info);
            continue;
        }

        // we're a match
        servicePort = serv;
        CFRelease(info);
        break;
    }

    IOObjectRelease(iter);
    return servicePort;
}

// Get the name of the specified display
//
char* getDisplayName(CGDirectDisplayID displayID)
{
    char* name;
    CFDictionaryRef info, names;
    CFStringRef value;
    CFIndex size;

    io_service_t serv = IOServicePortFromCGDisplayID(displayID);
    if (!serv)
    {
        return strdup("Unknown");
    }

    info = IODisplayCreateInfoDictionary(serv,
                                         kIODisplayOnlyPreferredName);

    IOObjectRelease(serv);

    names = CFDictionaryGetValue(info, CFSTR(kDisplayProductName));

    if (!names || !CFDictionaryGetValueIfPresent(names, CFSTR("en_US"),
                                                 (const void**) &value))
    {
        CFRelease(info);
        return strdup("Unknown");
    }

    size = CFStringGetMaximumSizeForEncoding(CFStringGetLength(value),
                                             kCFStringEncodingUTF8);
    name = calloc((size_t) (size + 1), 1);
    CFStringGetCString(value, name, size, kCFStringEncodingUTF8);

    CFRelease(info);

    return name;
}
