/*
 *  protocol.c
 *  HackerPacket
 *
 *  Created by ddm on 1/20/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "protocol.h"

const BASCommandSpec kMyCommandSet[] = {
    {
        kInitInterfaceCommand,     // commandName
        kPromiscousModeRightName,  // rightName
        "default",                 // rightDefaultRule -- allow anyone
        NULL,                     // rightDescriptionKey -- no custom prompt
        NULL                      // userData
    },

    {
        kRecvDataFromInterfaceCommand,          // commandName
        kPromiscousModeRightName, // rightName
        "default",                  // rightDefaultRule -- allow anyone
        NULL,                     // rightDescriptionKey -- no custom prompt
        NULL                      // userData
    },

    {
        kStopInterfaceCommand,          // commandName
        kPromiscousModeRightName, // rightName
        "default",                  // rightDefaultRule -- allow anyone
        NULL,                     // rightDescriptionKey -- no custom prompt
        NULL                      // userData
    },
	
    {   
        NULL,                     // the array is null terminated
        NULL, 
        NULL, 
        NULL, 
        NULL
    }
};

