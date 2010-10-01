/*
 EverySynth - A multi-timbral AudioUnit for external synthesizers.
 Copyright (C) 2010  Philip Kranz
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
