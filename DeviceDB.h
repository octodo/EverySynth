/*
 *  DeviceDB.h
 *  EverySynth
 *
 *  Created by Philip Kranz on 27.09.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <CoreServices/CoreServices.h>

typedef struct {
    UInt8 msb;
    UInt8 lsb;
} BankSelectInfo;

class DeviceDB
{
public:
    DeviceDB(CFStringRef theDbDir);
    ~DeviceDB();

    CFDictionaryRef getDeviceInfo(CFStringRef deviceName);
    CFArrayRef getDeviceList(CFStringRef manufacturer = NULL);

    void loadDevice(CFStringRef deviceName);

    CFArrayRef getBankNames();
    CFArrayRef getPatchNames(int banknr);
    BankSelectInfo getBankSelectInfo(int banknr);
    
private:
    CFStringRef dbDir;
    CFDictionaryRef currentDevice;
};
