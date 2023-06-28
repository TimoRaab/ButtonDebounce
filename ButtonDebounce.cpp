/*
    Button class for push buttons (not switches).
    Created by Timo Raab, June 28, 2023.
    v0.1
*/

#include "ButtonDebounce.h"


extern uint8_t _buttonHistory_1;

#define MASK   0b11000111

// Constructors
ButtonDebounce::ButtonDebounce(unsigned char pin, bool pullUp, bool execAtRelease, 
            byte resolution, void (*bFunction)(), bool await) {
                _pin = pin;
                _pullUp = pullUp;
                _execAtRelease = execAtRelease;
                _resolution = resolution;
                _bFunc = bFunction;
				
				if (_pullUp) {
					pinMode(_pin, INPUT_PULLUP);
				} else {
					pinMode(_pin, INPUT);
				}
				
				_await = await;
            }

ButtonDebounce::ButtonDebounce(unsigned char pin, bool pullUp, bool execAtRelease, 
            byte resolution, void (*bFunction)()) {
                _pin = pin;
                _pullUp = pullUp;
                _execAtRelease = execAtRelease;
                _resolution = resolution;
                _bFunc = bFunction;
				_await = true;
				
				if (_pullUp) {
					pinMode(_pin, INPUT_PULLUP);
				} else {
					pinMode(_pin, INPUT);
				}
				_await = true;
            }


ButtonDebounce::ButtonDebounce(unsigned char pin, bool pullUp) {
    _pin = pin;
	_pullUp = pullUp;
	_execAtRelease = true;
	_resolution = 1;
	_bFunc = nullFunction;
	
	if (_pullUp) {
		pinMode(_pin, INPUT_PULLUP);
	} else {
		pinMode(_pin, INPUT);
	}
	_await = true;
}

ButtonDebounce::ButtonDebounce(unsigned char pin) {
	_pin = pin;
	_pullUp = true;
	_execAtRelease = true;
	_resolution = 1;
	_bFunc = nullFunction;
	
	if (_pullUp) {
		pinMode(_pin, INPUT_PULLUP);
	} else {
		pinMode(_pin, INPUT);
	}
	_await = true;
}

// set Methods
bool ButtonDebounce::setPullUp(bool pullUp) {
    _pullUp = pullUp;
    return true;
}

bool ButtonDebounce::setExecAtRelease(bool execAtRelease) {
    _execAtRelease = execAtRelease;
    return true;
}

bool ButtonDebounce::setFunction(void (*bFunction)()) {
    _bFunc = bFunction;
    return true;
}

// Methods
bool ButtonDebounce::isPressed(bool execute) {
	uint8_t pressed = 0;   
    if (_pullUp) { 
    	if ((_buttonHistory_1 & MASK) == 0b00000111){ 
			pressed = 1;
			_buttonHistory_1 = 0b11111111;
    	}
    } else {
		if ((_buttonHistory_1 & (~MASK)) == 0b11111000) {
			pressed = 1;
			_buttonHistory_1 = 0b00000000;
		}
	}
    return pressed;
}

bool ButtonDebounce::updateButton() {
		byte tempButtonStatus = readButton();
		_buttonHistory_1 = _buttonHistory_1 << 1;
		_buttonHistory_1 |= tempButtonStatus;
		return true;
}

byte ButtonDebounce::readButton() {
	return digitalRead(_pin);
}

// other methods
void nullFunction(){
	//Do Nothing
};


















