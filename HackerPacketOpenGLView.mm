#import "HackerPacketOpenGLView.h"
#include "tcpdump3d.h"
#include "font.h"
#include <stdio.h>
#include <time.h>
#include "BetterAuthorizationSampleLib.h"
#include "protocol.h"

CCamera    g_Camera;
TcpDump3D  tcpdump;
time_t     curr_time;

@implementation HackerPacketOpenGLView

- (void) finalize
{
	int ret = noErr;
	printf("finalize called\n");
	if(gAuth == NULL)
	{
		ret = AuthorizationFree(gAuth, kAuthorizationFlagDestroyRights);
		assert(ret == noErr);
		gAuth = NULL;
	}
	
	[super finalize];
}

- (void) sendStopInterfaceToHelper: (NSString *) interfaceName
{

}

- (void) recvInterfaceDataFromHelper
{

	NSDictionary *request = [NSDictionary dictionaryWithObjectsAndKeys:@kRecvDataFromInterfaceCommand, @kBASCommandKey, nil];
	const NSDictionary *response = [self sendRequestToHelper: request];
}



- (void) sendStartInterfaceToHelper: (NSString *) interfaceName
{
	NSDictionary *request = [NSDictionary dictionaryWithObjectsAndKeys:@kInitInterfaceCommand, @kBASCommandKey, interfaceName, @"interface", nil];
	
	const NSDictionary *response = [self sendRequestToHelper: request];
	if(response)
		[response dealloc];
}

- (const NSDictionary *) sendRequestToHelper: (NSDictionary *)request
{

    OSStatus        err = noErr;
    CFDictionaryRef response = NULL;

	if(gAuth == NULL)
	{
		(void) AuthorizationCreate(
			NULL, 
			NULL, 
			kAuthorizationFlagDefaults, 
			&gAuth
		);

		BASSetDefaultRules(
			gAuth, 
			kMyCommandSet, 
			CFBundleGetIdentifier(CFBundleGetMainBundle()), 
			CFSTR("MyAuthPrompts")
		);
		
		{
		// TODO: I'm pretty sure gAuth is getting cached and I don't know how to make a callback
		// when a window is closed right now.
		BASFailCode failCode;
		 BASFixFailure(
					gAuth, 
					(CFStringRef) [[NSBundle mainBundle] bundleIdentifier],
					CFSTR("HackerPacketInstaller"), 
					CFSTR("HackerPacketHelper"), 
					failCode
				);
				
		}
		
	}

	err = BASExecuteRequestInHelperTool(
			gAuth, 
			kMyCommandSet, 
			(CFStringRef) [[NSBundle mainBundle] bundleIdentifier],
			(CFDictionaryRef) request, 
			&response
		  );
	
	if(err != noErr && err != userCanceledErr)
	{
		int alertResult;
		OSStatus installErr;
		BASFailCode failCode;
		
		failCode = BASDiagnoseFailure(gAuth, (CFStringRef) [[NSBundle mainBundle] bundleIdentifier]);
		
        alertResult = NSRunAlertPanel(@"Needs Install", @"BAS needs to install", @"Install", @"Cancel", NULL);
        
        if ( alertResult == NSAlertDefaultReturn ) {

			installErr = BASFixFailure(
					gAuth, 
					(CFStringRef) [[NSBundle mainBundle] bundleIdentifier],
					CFSTR("HackerPacketInstaller"), 
					CFSTR("HackerPacketHelper"), 
					failCode
				);
    
			if(installErr == noErr)
			{
				err = BASExecuteRequestInHelperTool(
					gAuth, 
					kMyCommandSet, 
					(CFStringRef) [[NSBundle mainBundle] bundleIdentifier],
					(CFDictionaryRef) request, 
					&response
				);
			}
		}
	}
	
    if (err == noErr) 
	{
		// check the helper for an error response
        err = BASGetErrorFromResponse(response);
    }

	return (const NSDictionary *)response;
}

// is this called everytime a new window is created?
- (void) awakeFromNib
{
	gAuth = NULL;
	[self sendStartInterfaceToHelper: @"en1"];

	timer = [[NSTimer scheduledTimerWithTimeInterval: 1.0/30.0 target: self selector: @selector(idleLoop) userInfo: nil repeats: YES] retain];
	[[NSRunLoop currentRunLoop] addTimer: timer forMode: NSEventTrackingRunLoopMode];
	curr_time = time(NULL);

}

- (void) prepareOpenGL
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 4.0/3.0, 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
 
	tcpdump.InitGrid(4, 4);
	LoadFontTexture("font-8bit.tga");

}

- (void) idleLoop
{
	curr_time = time(NULL);
	[self recvInterfaceDataFromHelper];
	[self drawRect:[self bounds]]; // redraw now instead dirty to enable updates during live resize
}

- (void) reshape
{
	printf("new window width: %f height: %f\n", [self bounds].size.width, [self bounds].size.height);
	glViewport(0, 0, (int)[self bounds].size.width, (int)[self bounds].size.height);
 	glLoadIdentity();
	gluPerspective(60.0, 4.0/3.0, 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

-(void) drawRect: (NSRect) bounds
{
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	g_Camera.Load();
	tcpdump.Render(g_Camera);
	g_Camera.Unload();

	glDisable(GL_TEXTURE_2D);
    glFlush();
	
	[[self openGLContext] flushBuffer];
}

@end
