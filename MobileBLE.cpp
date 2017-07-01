
/*
Name: MobileBLE.cpp
Created: 6/28/2017 11:00:39 AM
Author: Jae An
Contact: jaean37@gmail.com
*/

#include "MobileBLE.h"
#include "Arduino.h"


MobileBLE::MobileBLE(Stream &output) :
        _bluetooth(output)
{

}

int MobileBLE::checkBluetooth() {
    while (_bluetooth.available() > 0) {
        // while bluetooth singal is available

        int input = _bluetooth.read();

        push(input);

        if (input == 254) {
            // end of transmission for orientation
            _throttle = _signal[0];
            _steering = _signal[1];
            deleteElements(); // delete all elements in the singal array
        }
        else if (input == 255) {
            // end of transmission for button click
            _button = (char)_signal[0];
            deleteElements();
        }
    }
}

void MobileBLE::push(int elem) {
    _signal[_signalLength] = elem;
    _signalLength++;
}

void MobileBLE::deleteElements() {
    for (int i = 0; i < _signalLength; i++) {
        _signal[i] = NULL;
    }
    _signalLength = 0;
}


char MobileBLE::getButton() {
    char btn = _button;
    _button = NULL;
    return btn;
}

int MobileBLE::getThrottle() {
    return _throttle;
}

int MobileBLE::getSteering() {
    return _steering;
}