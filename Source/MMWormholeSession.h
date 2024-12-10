#import "MMWormhole.h"

#import <WatchConnectivity/WatchConnectivity.h>

NS_ASSUME_NONNULL_BEGIN

/**
 The MMWormholeSession class provides support for WatchConnectivity-based message passing.
 */
@interface MMWormholeSession : MMWormhole <WCSessionDelegate>

/**
 This method returns a specific instance of MMWormholeSession that should be used for listening. You
 may create your own instances of MMWormholeSession for sending messages, but this is the only object
 that will be able to receive messages.
 
 The reason for this is that MMWormholeSession is based on the WCSession class that is part of the
 WatchConnectivity framework provided by Apple, and WCSession is itself a singleton with a single
 delegate. Therefore, to receive callbacks, only one MMWormholeSession object may register itself
 as a listener.
 */
+ (instancetype)sharedListeningSession;

/**
 This method should be called after all of your initial listeners have been set and you are ready to
 begin listening for messages. There are likely some listeners that your application requires to be
 active so that it won't miss critical messages. You should set up these listeners before calling
 this method so that any already queued messages will be delivered immediately when you activate the
 session. Any listeners you set up after calling this method may miss messages that were already
 queued and waiting to be delivered.
 */
- (void)activateSessionListening;

@end

NS_ASSUME_NONNULL_END
