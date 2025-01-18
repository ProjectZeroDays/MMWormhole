# MMWormhole

MMWormhole creates a bridge between an iOS or OS X extension and its containing application. The wormhole is meant to be used to pass data or commands back and forth between the two locations. Messages are archived to files which are written to the application's shared App Group. The effect closely resembles interprocess communication between the app and the extension, though true interprocess communication does not exist between extensions and containing apps. 

The wormhole also supports CFNotificationCenter Darwin Notifications in an effort to support realtime change notifications. When a message is passed to the wormhole, interested parties can listen and be notified of these changes on either side of the wormhole. The effect is nearly instant updates on either side when a message is sent through the wormhole.

## Table of Contents

- [Introduction](#introduction)
- [Example](#example)
- [Getting Started](#getting-started)
- [Prerequisites](#prerequisites)
- [Installation Instructions](#installation-instructions)
- [Detailed Usage Examples](#detailed-usage-examples)
- [Troubleshooting Common Issues](#troubleshooting-common-issues)
- [Contributing](#contributing)
- [Code of Conduct](#code-of-conduct)
- [License](#license)

<p align="center">
<img src="MMWormhole.gif") alt="Example App"/>
</p>

## Introduction

MMWormhole is designed to make it easy to share very basic information and commands between an extension and its containing application. The wormhole should remain stable whether the containing app is running or not, but notifications will only be triggered in the containing app if the app is awake in the background. This makes MMWormhole ideal for cases where the containing app is already running via some form of background modes. 

A good way to think of the wormhole is a collection of shared mailboxes. An identifier is essentially a unique mailbox you can send messages to. You know where a message will be delivered to because of the identifier you associate with it, but not necessarily when the message will be picked up by the recipient. If the app or extension are in the background, they may not receive the message immediately. By convention, sending messages should be done from one side to another, not necessarily from yourself to yourself. It's also a good practice to check the contents of your mailbox when your app or extension wakes up, in case any messages have been left there while you were away.

MMWormhole uses NSKeyedArchiver as a serialization medium, so any object that is NSCoding compliant can work as a message. For many apps, sharing simple strings, numbers, or JSON objects is sufficient to drive the UI of a Widget or Apple Watch app. Messages can be sent and persisted easily as archive files and read later when the app or extension is woken up later.

Using MMWormhole is extremely straightforward. The only real catch is that your app and its extensions must support shared app groups. The group will be used for writing the archive files that represent each message. While larger files and structures, including a whole Core Data database, can be shared using App Groups, MMWormhole is designed to use its own directory simply to pass messages. Because of that, a best practice is to initialize MMWormhole with a directory name that it will use within your app's shared App Group.

## Example

```objective-c
[self.wormhole passMessageObject:@{@"buttonNumber" : @(1)} identifier:@"button"];

[self.wormhole listenForMessageWithIdentifier:@"button" 
  listener:^(id messageObject) {
    self.numberLabel.text = [messageObject[@"buttonNumber"] stringValue];
}];
```

## Getting Started

- Install MMWormhole via CocoaPods or by downloading the Source files
- [Configure your App and Extension to support App Groups](https://developer.apple.com/library/ios/documentation/General/Conceptual/ExtensibilityPG/ExtensionScenarios.html)
- Begin using MMWormhole to pass messages between your App and Extension

### Prerequisites

- Xcode 7.0 or later
- iOS 9.0 or later
- An Apple Developer account with access to App Groups

### Note

The MMWormhole Example app will only work with your shared App Group identifiers and Entitlements and is meant purely for reference

---
## Installing MMWormhole
<img src="https://cocoapod-badges.herokuapp.com/v/MMWormhole/badge.png"/><br/>
You can install Wormhole in your project by using [CocoaPods](https://github.com/cocoapods/cocoapods):

```Ruby
pod 'MMWormhole', '~> 2.0.0'
```

[![Carthage compatible](https://img.shields.io/badge/Carthage-compatible-4BC51D.svg?style=flat)](https://github.com/Carthage/Carthage)<br/>
MMWormhole also supports Carthage.

## Overview

MMWormhole is designed to make it easy to share very basic information and commands between an extension and it's containing application. The wormhole should remain stable whether the containing app is running or not, but notifications will only be triggered in the containing app if the app is awake in the background. This makes MMWormhole ideal for cases where the containing app is already running via some form of background modes. 

A good way to think of the wormhole is a collection of shared mailboxes. An identifier is essentially a unique mailbox you can send messages to. You know where a message will be delivered to because of the identifier you associate with it, but not necessarily when the message will be picked up by the recipient. If the app or extension are in the background, they may not receive the message immediately. By convention, sending messages should be done from one side to another, not necessarily from yourself to yourself. It's also a good practice to check the contents of your mailbox when your app or extension wakes up, in case any messages have been left there while you were away.

MMWormhole uses NSKeyedArchiver as a serialization medium, so any object that is NSCoding compliant can work as a message. For many apps, sharing simple strings, numbers, or JSON objects is sufficient to drive the UI of a Widget or Apple Watch app. Messages can be sent and persisted easily as archive files and read later when the app or extension is woken up later.

Using MMWormhole is extremely straightforward. The only real catch is that your app and it's extensions must support shared app groups. The group will be used for writing the archive files that represent each message. While larger files and structures, including a whole Core Data database, can be shared using App Groups, MMWormhole is designed to use it's own directory simply to pass messages. Because of that, a best practice is to initialize MMWormhole with a directory name that it will use within your app's shared App Group.

### Initialization

Initialize MMWormhole with your App Group identifier and an optional directory name

Objective-C:
```objective-c
self.wormhole = [[MMWormhole alloc] initWithApplicationGroupIdentifier:@"group.com.mutualmobile.wormhole"
                                                     optionalDirectory:@"wormhole"];
```
Swift:
```swift
let wormhole = MMWormhole(applicationGroupIdentifier: "group.com.mutualmobile.wormhole", optionalDirectory: "wormhole")
```

### Passing a Message

Pass a message with an identifier for the message and a NSCoding compliant object as the message itself

Objective-C:
```objective-c
[self.wormhole passMessageObject:@{@"titleString" : title} 
                      identifier:@"messageIdentifier"];
```

Swift:
```swift
wormhole.passMessageObject("titleString", identifier: "messageIdentifier")
```

### Reading a Message

You have two options for reading a message. You can obtain the message for an identifier at any time by asking the wormhole for the message. 

Objective-C:
```objective-c
id messageObject = [self.wormhole messageWithIdentifier:@"messageIdentifier"];
```

You can also listen for changes to that message and be notified when that message is updated.

Objective-C:
```objective-c
[self.wormhole listenForMessageWithIdentifier:@"messageIdentifier" 
 listener:^(id messageObject) {
    // Do Something
}];

```
Swift:
```swift
wormhole.listenForMessageWithIdentifier("messageIdentifier", listener: { (messageObject) -> Void in
    if let message: AnyObject = messageObject {
        // Do something
    }
})
```

### Designing Your Communication Scheme

You can think of message passing between apps and extensions sort of like a web service. The web service has endpoints that you can read and write. The message identifiers for your MMWormhole messages can be thought of in much the same way. A great practice is to design very clear message identifiers so that you immediately know when reading your code who sent the message and why, and what the possible contents of the message might be. Just like you would design a web service with clear semantics, you should do the same with your wormhole messaging scheme.

### Communication with WatchConnectivity

The design of your communication scheme is even more important when you need to support watchOS 2. MMWormhole supports the [WatchConnectivity](https://developer.apple.com/library/prerelease/watchos/documentation/WatchConnectivity/Reference/WatchConnectivity_framework/index.html#//apple_ref/doc/uid/TP40015269) framework provided by Apple as an easy way to get up and running quickly with a basic implementation of WatchConnectivity. This support is not intended to replace WatchConnectivity entirely, and it's important to carefully consider your watch app's communication system to see where MMWormhole will fit best. 

Here are two things you need to know if you want to use WatchConnectivity support in your app:

- [MMWormholeSession](http://cocoadocs.org/docsets/MMWormhole/2.0.0/Classes/MMWormholeSession.html) is a singleton subclass of MMWormhole that supports listening for WatchConnectivity messages. It should be used as the listener for all MMWormhole messages you expect to receive from the WatchConnectivity framework. Be sure to activate the session once your listeners are set so that you can begin receiving message notifications.

- Use the MMWormholeSessionTransiting types described below when creating your wormholes, but be careful not to send too many messages at once. You can easily overload the pipeline by sending too many messages at once.

### Message Transiting Options

The mechanism by which data flows through MMWormhole is defined by the [MMWormholeTransiting](http://cocoadocs.org/docsets/MMWormhole/2.0.0/Classes/MMWormholeTransiting.html) protocol. The default implementation of the protocol is called [MMWormholeFileTransiting](http://cocoadocs.org/docsets/MMWormhole/2.0.0/Classes/MMWormholeFileTransiting.html), which reads and writes messages as archived data files in the app groups shared container. Users of MMWormhole can implement their own version of this protocol to change the message passing behavior.

There are three new implementations of the MMWormholeTransiting protocol that support the WCSession application context, message, and file transfer systems. You may only use one form of transiting with a wormhole at a time, so you need to consider which type of messaging system best fits a given part of your application.

Most apps will find the application context system to be a good balance between real time messaging and simple persistence, so we recommend [MMWormholeSessionContextTransiting](http://cocoadocs.org/docsets/MMWormhole/2.0.0/Classes/MMWormholeSessionContextTransiting.html) as the best place to start. Check out the [documentation](https://developer.apple.com/library/prerelease/watchos/documentation/WatchConnectivity/Reference/WatchConnectivity_framework/index.html#//apple_ref/doc/uid/TP40015269) and header comments for descriptions about the other messaging types.

You can get started quickly with a wormhole using one of the built in transiting types by calling the optional initializer to set up an instance with the right transiting type for your use case.

Objective-C:
```objective-c
self.wormhole = [[MMWormhole alloc] initWithApplicationGroupIdentifier:@"group.com.mutualmobile.wormhole"
                                                     optionalDirectory:@"wormhole"
                                                     	transitingType:MMWormholeTransitingTypeSessionContext];
```

Swift:
```swift
let wormhole = MMWormhole(applicationGroupIdentifier: "group.com.mutualmobile.wormhole", 
								   optionalDirectory: "wormhole",
								      transitingType: .SessionContext)
```

## Requirements

MMWormhole requires iOS 7.0 or higher or OS X 10.10 or higher.
MMWormholeSession requires iOS 9.0 or higher.

## Troubleshooting

If messages are not received on the other end, check Project->Capabilities->App Groups.<br/>
Three checkmarks should be displayed in the steps section.

<p align="center">
<img src="MMWormhole_correct.png") alt="Correct App Group Capabilities"/>
</p>

<p align="center">
<img src="MMWormhole_incorrect.png") alt="Incorrect App Group Capabilities"/>
</p>

## Installation Instructions

To install MMWormhole, follow these steps:

1. Add MMWormhole to your project using CocoaPods, Carthage, or by downloading the source files.
2. Configure your app and extension to support App Groups.
3. Initialize MMWormhole with your App Group identifier and an optional directory name.
4. Begin using MMWormhole to pass messages between your app and extension.

## Detailed Usage Examples

Here are some detailed usage examples for MMWormhole:

### Example 1: Passing a Message

Objective-C:
```objective-c
[self.wormhole passMessageObject:@{@"titleString" : title} 
                      identifier:@"messageIdentifier"];
```

Swift:
```swift
wormhole.passMessageObject("titleString", identifier: "messageIdentifier")
```

### Example 2: Reading a Message

Objective-C:
```objective-c
id messageObject = [self.wormhole messageWithIdentifier:@"messageIdentifier"];
```

Swift:
```swift
let messageObject = wormhole.messageWithIdentifier("messageIdentifier")
```

### Example 3: Listening for Message Changes

Objective-C:
```objective-c
[self.wormhole listenForMessageWithIdentifier:@"messageIdentifier" 
 listener:^(id messageObject) {
    // Do Something
}];
```

Swift:
```swift
wormhole.listenForMessageWithIdentifier("messageIdentifier", listener: { (messageObject) -> Void in
    if let message: AnyObject = messageObject {
        // Do something
    }
})
```

## Troubleshooting Common Issues

Here are some common issues and their solutions:

### Issue 1: Messages are not being received

Solution: Check Project->Capabilities->App Groups. Three checkmarks should be displayed in the steps section.

### Issue 2: Messages are being received but not processed

Solution: Ensure that your listeners are set up correctly and that you are calling the appropriate methods to process the messages.

### Issue 3: Messages are being processed but not displayed

Solution: Verify that your UI is being updated correctly and that you are using the correct message identifiers.

## Contributing

We welcome contributions to MMWormhole! If you would like to contribute, please read our [CONTRIBUTING.md](CONTRIBUTING.md) file for guidelines on how to get started.

## Code of Conduct

We are committed to fostering a welcoming and inclusive community. Please read our [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md) file to understand the expectations for behavior and interactions within the community.

## License

MMWormhole is available under the MIT license. See the [LICENSE](LICENSE) file for more info.
