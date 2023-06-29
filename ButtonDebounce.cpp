#include "ButtonDebounce.h"
#include <Arduino.h>

#define MASK 0b11000111
#define COMPARATOR 0b00000111
#define HISTORYINIT 0b11111111
ButtonDebounce::ButtonDebounce(unsigned char pin, bool pullUp, bool executeAtRelease, void (*bFunction)()) {
    _pin = pin;
    _pullUp = pullUp;
    _bFunc = bFunction;
    _executeAtRelease = executeAtRelease;
    _buttonHistory = HISTORYINIT;
    if (_pullUp) {
        pinMode(_pin, INPUT_PULLUP);
    } else {
        pinMode(_pin, INPUT);
    }
}

ButtonDebounce::ButtonDebounce(unsigned char pin, bool pullUp, bool executeAtRelease) : ButtonDebounce(pin, pullUp, executeAtRelease, nullFunction){}

ButtonDebounce::ButtonDebounce(unsigned char pin) : ButtonDebounce(pin, true, false, nullFunction){}



// set Methods
bool ButtonDebounce::setPullUp(bool pullUp) {
    _pullUp = pullUp;
    return true;
}


bool ButtonDebounce::setFunction(void (*bFunction)()) {
    _bFunc = bFunction;
    return true;
}

uint8_t ButtonDebounce::getButtonHistory() {
    return _buttonHistory;
}


bool ButtonDebounce::isPressed(bool execute) {  
    if ((_buttonHistory & MASK) == COMPARATOR){ 
        _buttonHistory = HISTORYINIT;
        if (execute) {
            _bFunc();
        }
        return true;
    }
    return false;
}

void ButtonDebounce::updateButton() {
    _buttonHistory = _buttonHistory << 1;
    _buttonHistory |= readButton();

}

uint8_t ButtonDebounce::readButton() {
    byte tempRead = digitalRead(_pin);
    if (_pullUp ^ _executeAtRelease) {
        tempRead = ~tempRead;
        tempRead = (tempRead & 0b00000001);
    }
    return tempRead;

}

// other methods
void nullFunction(){
	//Do Nothing
};