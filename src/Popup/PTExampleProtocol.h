#ifndef peertalk_PTExampleProtocol_h
#define peertalk_PTExampleProtocol_h
#import <Foundation/Foundation.h>
#include <stdint.h>
#import <CoreMIDI/CoreMIDI.h>

static const int PTExampleProtocolIPv4PortNumber = 2349;

enum {
    PTExampleFrameTypeDeviceInfo = 100,
    PTExampleFrameTypeTextMessage = 101,
    PTExampleFrameTypePing = 102,
    PTExampleFrameTypePong = 103,
};

typedef struct _PTExampleTextFrame {
    uint32_t length;
    uint8_t utf8text[0];
} PTExampleTextFrame;


static dispatch_data_t PTExampleTextDispatchDataWithString(const MIDIPacketList *message) {
    // Use a custom struct
    PTExampleTextFrame *textFrame;
    // const char *utf8text = [message cStringUsingEncoding:NSUTF8StringEncoding];
    size_t length = sizeof(message);
    textFrame = (PTExampleTextFrame*)CFAllocatorAllocate(nil, sizeof(PTExampleTextFrame) + length, 0);
    memcpy(textFrame->utf8text, message, length); // Copy bytes to utf8text array
    textFrame->length = htonl(length); // Convert integer to network byte order
    
    // Wrap the textFrame in a dispatch data object
    return dispatch_data_create((const void*)textFrame, sizeof(PTExampleTextFrame)+length, nil, ^{
        CFAllocatorDeallocate(nil, textFrame);
    });
}

static dispatch_data_t PTExampleTextDispatchDataWithBytes(const UInt8 *data,  UInt32 size) {
    // Use a custom struct
    PTExampleTextFrame *textFrame;
    // const char *utf8text = [message cStringUsingEncoding:NSUTF8StringEncoding];
    size_t length = size;
    textFrame = (PTExampleTextFrame*)CFAllocatorAllocate(nil, sizeof(PTExampleTextFrame) + length, 0);
    memcpy(textFrame->utf8text, data, length); // Copy bytes to utf8text array
    textFrame->length = htonl(length); // Convert integer to network byte order
    
    // Wrap the textFrame in a dispatch data object
    return dispatch_data_create((const void*)textFrame, sizeof(PTExampleTextFrame)+length, nil, ^{
        CFAllocatorDeallocate(nil, textFrame);
    });
}

#endif
