#ifndef MAC_DISPLAYS_DISPLAYS_H
#define MAC_DISPLAYS_DISPLAYS_H


#include <IOKit/graphics/IOGraphicsLib.h>
#include <CoreVideo/CVBase.h>
#include <CoreVideo/CVDisplayLink.h>
#include <ApplicationServices/ApplicationServices.h>

char* getDisplayName(CGDirectDisplayID displayID);

char* getDisplayMode(CGDirectDisplayID displayID);

//
#endif //MAC_DISPLAYS_DISPLAYS_H
