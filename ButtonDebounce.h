/*
    Buttons class for push buttons (not switches).
    Created by Timo Raab, June 28, 2023.
    v0.1

    Algorithm after
    https://hackaday.com/2015/12/10/embed-with-elliot-debounce-your-noisy-buttons-part-ii/
*/

#ifndef ButtonDebounce_h
#define ButtonDebounce_h

#include "Arduino.h"

class ButtonDebounce {

    private:
        unsigned char _pin;             // Arduino Pin
        bool _pullUp;                   // Use of internal pull up resistor,
											// if not pull down is assumend,
											// standard: true
        bool _execAtRelease;            // define function execution time
											// (at button press or release)
											// standard: false (release)
        byte _resolution;               // how many bytes should be used for debouncing
        void (*_bFunc)();          		// function call at button press
											// standard: do nothing
		bool _await;

        uint8_t _buttonHistory_1;
        uint16_t _buttonHistory_2;
        uint32_t _buttonHistory_4;
        uint64_t _buttonHistory_8;


        byte readButton();

    public:
		// constructor
        ButtonDebounce(unsigned char pin);
        ButtonDebounce(unsigned char pin, bool pullUp);
        ButtonDebounce(unsigned char pin, bool pullUp, bool execAtRelease, byte resolution, void (*bFunction)());
		ButtonDebounce(unsigned char pin, bool pullUp, bool execAtRelease, byte resolution, void (*bFunction)(), bool await);

		// set Methods
		bool setPullUp(bool pullUp);
        bool setExecAtRelease(bool execAtRelease);
        bool setResolution(byte resolution);
        bool setFunction(void (*bFunction)());
		
		// detect if a button is pressed
		// par: if function call should be executed
		// ret:	if button is pressed 
        bool isPressed(bool execute);

        bool updateButton();


};
// function for internal use only. Standard function call for library
void nullFunction();

#endif