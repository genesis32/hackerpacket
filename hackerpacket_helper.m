/*
 *  hackerpacket_helper.m
 *  HackerPacket
 *
 *  Created by ddm on 1/20/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <assert.h> 
#include "protocol.h"

OSStatus InitInterface(AuthorizationRef auth, const void *userData, CFDictionaryRef request,
					   CFMutableDictionaryRef response, aslclient asl, aslmsg aslMsg)
{
	int ret = 0;

	ret = asl_log(asl, aslMsg, ASL_LEVEL_DEBUG, "HackerPacketHelper::InitInterface");
	assert(ret == 0);

	if(!CFDictionaryContainsKey(request, @"interface"))
	{
		ret = asl_log(asl, aslMsg, ASL_LEVEL_ERR, "HackerPacketHelper::InitInterface element does not exist");
		assert(ret == 0);
	}
	else
	{
		CFStringRef interfaceName = NULL;
		char cbuff[8];

		interfaceName = (CFStringRef)CFDictionaryGetValue(request, @"interface");
		if(CFStringGetCString(interfaceName, cbuff, 8, kCFStringEncodingASCII))
		{
			ret = asl_log(asl, aslMsg, ASL_LEVEL_DEBUG, "HackerPacketHelper::InitInterface found %s", cbuff);
			assert(ret == 0);
		}

	}

	return noErr;
}

OSStatus RecvDataFromInterface(AuthorizationRef auth, const void *userData, CFDictionaryRef request,
					   CFMutableDictionaryRef response, aslclient asl, aslmsg aslMsg)
{
	int ret = 0;

	ret = asl_log(asl, aslMsg, ASL_LEVEL_DEBUG, "HackerPacketHelper::RecvDataFromInterface");
	assert(ret == 0);

	return noErr;
}

OSStatus StopInterface(AuthorizationRef auth, const void *userData, CFDictionaryRef request,
					   CFMutableDictionaryRef response, aslclient asl, aslmsg aslMsg)
{

	int ret = 0;

	ret = asl_log(asl, aslMsg, ASL_LEVEL_DEBUG, "HackerPacketHelper::StopInterface");
	assert(ret == 0);

	if(!CFDictionaryContainsKey(request, @"interface"))
	{
		ret = asl_log(asl, aslMsg, ASL_LEVEL_ERR, "HackerPacketHelper::StopInterface element does not exist");
		assert(ret == 0);
	}
	else
	{
		CFStringRef interfaceName = NULL;
		char cbuff[8];

		interfaceName = (CFStringRef)CFDictionaryGetValue(request, @"interface");
		if(CFStringGetCString(interfaceName, cbuff, 8, kCFStringEncodingASCII))
		{
			ret = asl_log(asl, aslMsg, ASL_LEVEL_DEBUG, "HackerPacketHelper::StopInterface interface found %s", cbuff);
			assert(ret == 0);
		}

	}


	return noErr;
}


int main(int argc, char *argv[])
{
	static const BASCommandProc kMyCommandProcs[] = 
	{
    InitInterface,
	RecvDataFromInterface,
	StopInterface,
    NULL
	};	
	
	return BASHelperToolMain(kMyCommandSet, kMyCommandProcs);
}
