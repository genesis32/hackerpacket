#import <Cocoa/Cocoa.h>


@interface HackerPacketOpenGLView : NSOpenGLView {
	NSTimer *timer;
	AuthorizationRef gAuth;
}
- (void) drawRect: (NSRect) bounds ;
- (void) awakeFromNib;
- (void) prepareOpenGL;
- (void) reshape;
- (void) idleLoop;
- (void) finalize;
- (const NSDictionary *) sendRequestToHelper: (NSDictionary *)request;
- (void) sendStartInterfaceToHelper: (NSString *) interfaceName;
- (void) sendStopInterfaceToHelper: (NSString *) interfaceName;
- (void) recvInterfaceInfoFromHelper;


@end
