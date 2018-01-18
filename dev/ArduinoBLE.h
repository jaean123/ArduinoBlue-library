/*
Name: ArduinoBLE.cpp
Created: 6/28/2017 11:00:39 AM
Author: Jae An
Contact: jaean37@gmail.com
*/

#ifndef _ArduinoBLE_h
#define _ArduinoBLE_h
#include <Arduino.h>
#include <AltSoftSerial.h>

const int DEFAULT_STEERING = 49;
const int DEFAULT_THROTTLE = 49;
const int MAX_SHORT_SIGNAL_LENGTH = 3;

class ArduinoBLE
{
public:
    ArduinoBLE(Stream &output);
    int getButton();
    int getSliderId();
    int getSliderVal();
    int getThrottle();
    int getSteering();
    bool checkBluetooth();
    bool isConnected();
    void sendMessage(String msg);
    String getText();
private:
    Stream & _bluetooth;
    int _signal[MAX_SHORT_SIGNAL_LENGTH];
    int _signalLength = 0;
    int _throttle = DEFAULT_STEERING;
    int _steering = DEFAULT_THROTTLE;
    int _sliderVal;
    int _sliderId;
    int _button;
    int _currentTransmission;
    String _text;
    void clearSignalArray();
    void pushToSignalArray(int elem);
    void storeShortTransmission();
    void processDriveTransmission();
    void processButtonTransmission();
    void processSliderTransmission();
    void processTextTransmission();
    void processPathTransmission();
    String readString();
};

#endif