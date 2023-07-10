/*
    Buttons class for momentary buttons (not switches).
    The class allows for far better debouncing in buttons.
    Created by Timo Raab, 10.07.2023.
    v1.0

    Idea after
    https://hackaday.com/2015/12/10/embed-with-elliot-debounce-your-noisy-buttons-part-ii/
    from 
    Elliot Williams
*/

#ifndef ButtonDebounce_h
#define ButtonDebounce_h

#include "Arduino.h"

class ButtonDebounce {

    private:
        unsigned char _pin;             // Pin
        bool _pullUp;                   // Use of internal pull up resistor,
											// if not pull down is assumend,
											// standard: true

        //Standard Operation
        bool _executeAtRelease;         // Choose, when the button press should be registered
                                        // Standard: at buttonPress (_executeAtRelease = false)
        void (*_bFunc)();          		// function call at button press
		
        //Long Press Operation, only available with setter-functions  
        //Long Press is only available with execute at start
        unsigned long _longPressDuration;// Duration till long press triggers
        void (*_bFuncLong)();           // function call at long press activation

        //Internal handling
        uint8_t _buttonHistory;         // saves history for debounce     
        bool _isPressedTemp;            // for longPress needed
        unsigned long _pressTimeTemp;   // time when button is pressed for longPress

        uint8_t readButton();           // read current button status

    public:
		// constructor
        ButtonDebounce(unsigned char pin);
        ButtonDebounce(unsigned char pin, bool pullUp, bool executeAtRelease);
        ButtonDebounce(unsigned char pin, bool pullUp, bool executeAtRelease, void (*bFunction)()) ;

		// set Methods
		bool setPullUp(bool pullUp);
        bool setFunction(void (*bFunction)());
        bool setLongPressDuration(unsigned long duration);
        bool setLongPressFunction(void (*bFunction)());
		
        // check for button presses
        // returns if a button is pressed!
        bool isPressed(bool execute);
        bool isLongPressed(bool execute);

        void updateButton();

        // Debug functions
        uint8_t getButtonHistory();

};
// function for internal use only. Standard function call for library
void nullFunction();

#endif

//EOF