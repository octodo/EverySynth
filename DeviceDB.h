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
