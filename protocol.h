/*
 *  protocol.h
 *  HackerPacket
 *
 *  Created by ddm on 1/20/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PROTOCOL_H

#include "BetterAuthorizationSampleLib.h"

#define kInitInterfaceCommand         "InitInterfaceCommand"
#define kRecvDataFromInterfaceCommand "RecvDataFromInterfaceCommand"
#define kStopInterfaceCommand         "StopInterfaceCommand"
#define kPromiscousModeRightName      "com.msynet.HackerPacket.promiscousmode.manage"

extern const BASCommandSpec kMyCommandSet[];

#endif
