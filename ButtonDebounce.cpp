#include "ButtonDebounce.h"

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
        _bFuncLong = nullptr;

        if (_pullUp) {
            pinMode(_pin, INPUT_PULLUP);
        } else {
            pinMode(_pin, INPUT);
        }
}
//_____________________________________________________________________________



//_____________________________________________________________________________
// set Methods_________________________________________________________________
//_____________________________________________________________________________
bool ButtonDebounce::setPullUp(bool pullUp) {
    _pullUp = pullUp;
    return true;
}

bool ButtonDebounce::setExecuteAtRelease(bool executeAtRelease){
    _executeAtRelease = executeAtRelease;
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

bool ButtonDebounce::stillPressed(bool execute) {
    byte tempButtonHistory = (_executeAtRelease) ? ~_buttonHistory : _buttonHistory;
    if (!_isPressedTemp) return false;
    if ((tempButtonHistory & 0b00000001) == 1) {
        if (execute) {
            _bFunc();
        }
        return true;
    }
    return false;
}

bool ButtonDebounce::anyPressed(bool execute) {
    boolean temp = isPressed(execute);
    temp = temp || stillPressed(execute);
    return temp;
}
//_____________________________________________________________________________


//_____________________________________________________________________________
//_DEBUG_FUNCTIONS_____________________________________________________________
//_____________________________________________________________________________
uint8_t ButtonDebounce::getButtonHistory() {
    return _buttonHistory;
}
//_____________________________________________________________________________

//EOF