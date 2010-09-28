/*
 *  DeviceDB.cpp
 *  EverySynth
 *
 *  Created by Philip Kranz on 27.09.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "DeviceDB.h"

DeviceDB::DeviceDB(CFStringRef theDbDir)
{
    dbDir = theDbDir;
    currentDevice = NULL;
}

DeviceDB::~DeviceDB()
{
    CFRelease(dbDir);
    CFRelease(currentDevice);
}

CFDictionaryRef DeviceDB::getDeviceInfo(CFStringRef deviceName)
{
    CFURLRef resUrl = CFBundleCopyResourcesDirectoryURL(CFBundleGetBundleWithIdentifier(CFSTR("net.pmlk.audiounits.EverySynth")));
   
    CFURLRef devicesUrl = CFURLCreateCopyAppendingPathComponent(NULL, resUrl, CFSTR("Devices"), true);
    CFURLRef devUrl = CFURLCreateCopyAppendingPathComponent(NULL, devicesUrl, deviceName, false);
    CFURLRef devUrlWithExt = CFURLCreateCopyAppendingPathExtension(NULL, devUrl, CFSTR("plist"));
    
    CFDataRef xmlData;
    CFPropertyListRef plist;
    CFURLCreateDataAndPropertiesFromResource(NULL, devUrlWithExt, &xmlData, NULL, NULL, NULL);
    plist = CFPropertyListCreateFromXMLData(NULL, xmlData, 0, NULL);
    
    CFRelease(resUrl);
    CFRelease(devicesUrl);    
    CFRelease(devUrl);    
    CFRelease(devUrlWithExt);
    //CFRelease(xmlData);
    
    return (CFDictionaryRef)plist;
}

CFArrayRef DeviceDB::getDeviceList(CFStringRef manufacturer)
{
    CFMutableArrayRef ret = CFArrayCreateMutable(NULL, 10, NULL);
    CFArrayRef urls = CFBundleCopyResourceURLsOfType(CFBundleGetBundleWithIdentifier(CFSTR("net.pmlk.audiounits.EverySynth")), CFSTR("plist"), CFSTR("Devices"));
    
    for (int i=0; i<CFArrayGetCount(urls); i++) {
        CFStringRef lastComponent = CFURLCopyLastPathComponent((CFURLRef)CFArrayGetValueAtIndex(urls, i));
        CFStringRef item = CFStringCreateWithSubstring(NULL, lastComponent, CFRangeMake(0, CFStringGetLength(lastComponent) - 6));
        CFArrayAppendValue(ret, item);
        CFRelease(lastComponent);
    }
    
    CFRelease(urls);
    
    return ret;
}

void DeviceDB::loadDevice(CFStringRef deviceName)
{
    currentDevice = getDeviceInfo(deviceName);
}

CFArrayRef DeviceDB::getBankNames()
{
    if (currentDevice == NULL)
        return NULL;
    
    CFArrayRef banks = (CFArrayRef)CFDictionaryGetValue(currentDevice, CFSTR("Banks"));
    CFMutableArrayRef ret = CFArrayCreateMutable(NULL, 10, NULL);
    for (int i=0; i<CFArrayGetCount(banks); i++) {
        CFDictionaryRef bank = (CFDictionaryRef)CFArrayGetValueAtIndex(banks, i);
        CFArrayAppendValue(ret, (CFStringRef)CFDictionaryGetValue(bank, CFSTR("Name")));
    }
    return ret;
}

CFArrayRef DeviceDB::getPatchNames(int banknr)
{
    if (currentDevice == NULL)
        return NULL;
    
    CFArrayRef banks = (CFArrayRef)CFDictionaryGetValue(currentDevice, CFSTR("Banks"));
    if (CFArrayGetCount(banks) < banknr + 1)
        return NULL;
    
    CFDictionaryRef bank = (CFDictionaryRef)CFArrayGetValueAtIndex(banks, banknr);        
    CFArrayRef ret = (CFArrayRef)CFDictionaryGetValue(bank, CFSTR("Patches"));
    CFShow(ret);
    return ret;
}

BankSelectInfo DeviceDB::getBankSelectInfo(int banknr)
{
    BankSelectInfo ret;
    ret.msb = 0;
    ret.lsb = 0;
    
    if (currentDevice == NULL)
        return ret;
    
    CFArrayRef banks = (CFArrayRef)CFDictionaryGetValue(currentDevice, CFSTR("Banks"));
    if (CFArrayGetCount(banks) < banknr + 1)
        return ret;
    
    CFDictionaryRef bank = (CFDictionaryRef)CFArrayGetValueAtIndex(banks, banknr);        
    CFNumberGetValue((CFNumberRef)CFDictionaryGetValue(bank, CFSTR("MSB")), kCFNumberSInt8Type, &(ret.msb));
    CFNumberGetValue((CFNumberRef)CFDictionaryGetValue(bank, CFSTR("LSB")), kCFNumberSInt8Type, &(ret.lsb));
    
    return ret;
}
