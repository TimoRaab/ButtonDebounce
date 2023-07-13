#include "ButtonDebounce.h"
#include <Arduino.h>

#define MASK        0b11000111
#define COMPARATOR  0b00000111
#define HISTORYINIT 0b11111111

//_____________________________________________________________________________
//_CONSTRUCTOR_________________________________________________________________
//_____________________________________________________________________________
ButtonDebounce::ButtonDebounce(unsigned char pin, 
    bool pullUp, 
    bool executeAtRelease, 
    void (*bFunction)()) {
        _pin = pin;

        _pullUp = pullUp;
        _bFunc = bFunction;
        _executeAtRelease = executeAtRelease;
        _buttonHistory = HISTORYINIT;

        _pressTimeTemp = 0;
        _isPressedTemp = false;
        _longPressDuration = 1000;
        _bFuncLong = *nullFunction;

        if (_pullUp) {
            pinMode(_pin, INPUT_PULLUP);
        } else {
            pinMode(_pin, INPUT);
        }
}

ButtonDebounce::ButtonDebounce(unsigned char pin, bool pullUp, bool executeAtRelease) : 
    ButtonDebounce(pin, pullUp, executeAtRelease, nullFunction){}

ButtonDebounce::ButtonDebounce(unsigned char pin, bool pullUp) :
    ButtonDebounce(pin, pullUp, false, nullFunction){}

ButtonDebounce::ButtonDebounce(unsigned char pin) : 
    ButtonDebounce(pin, true, false, nullFunction){}
//_____________________________________________________________________________



//_____________________________________________________________________________
// set Methods_________________________________________________________________
//_____________________________________________________________________________
bool ButtonDebounce::setPullUp(bool pullUp) {
    _pullUp = pullUp;
    return true;
}


bool ButtonDebounce::setFunction(void (*bFunction)()) {
    _bFunc = bFunction;
    return true;
}

bool ButtonDebounce::setLongPressDuration(unsigned long duration) {
    _longPressDuration = duration;
    return true;
}

bool ButtonDebounce::setLongPressFunction(void (*bFunction)()) {
    _bFuncLong = bFunction;
    return true;
}
//_____________________________________________________________________________



//_____________________________________________________________________________
//_DETECTION___________________________________________________________________
//_____________________________________________________________________________
uint8_t ButtonDebounce::readButton() {
    byte tempRead = digitalRead(_pin);
    if (_pullUp ^ _executeAtRelease) {
        tempRead = ~tempRead;
        tempRead = (tempRead & 0b00000001);
    }
    return tempRead;
}

void ButtonDebounce::updateButton() {
    _buttonHistory = _buttonHistory << 1;
    _buttonHistory |= readButton();
}
//_____________________________________________________________________________



//_____________________________________________________________________________
//_EVALUATION__________________________________________________________________
//_____________________________________________________________________________
bool ButtonDebounce::isPressed() {
    return isPressed(false);
}

bool ButtonDebounce::isPressed(bool execute) {  
    if (~_isPressedTemp) {
        if ((_buttonHistory & MASK) == COMPARATOR){
            _buttonHistory = HISTORYINIT;
            _isPressedTemp = true;
            _pressTimeTemp = millis();
            if (execute) {
                _bFunc();
            }
            return true;
        }
    }
    return false;
}

bool ButtonDebounce::isLongPressed() {
    return isLongPressed(false);
}

bool ButtonDebounce::isLongPressed(bool execute) {
    byte tempButtonHistory = (_executeAtRelease) ? ~_buttonHistory : _buttonHistory;
    if (_isPressedTemp) {
        if ((tempButtonHistory & 0b00000001) == 1) {//Still pressed
            if (millis()-_pressTimeTemp > _longPressDuration) {
                if (execute) {
                    _bFuncLong();
                }
                _isPressedTemp = false;
                return true;
            }
        } else {
            _isPressedTemp = false;
            return false;
        }
    } else {
        if ((tempButtonHistory & MASK) == COMPARATOR){ 
            _isPressedTemp = true;
            _pressTimeTemp = millis();
        }
    }
    return false;
}
//_____________________________________________________________________________


//_____________________________________________________________________________
//_DEBUG_FUNCTIONS_____________________________________________________________
//_____________________________________________________________________________
uint8_t ButtonDebounce::getButtonHistory() {
    return _buttonHistory;
}
//_____________________________________________________________________________



// other methods
void nullFunction(){
	//Do Nothing
};

//EOF