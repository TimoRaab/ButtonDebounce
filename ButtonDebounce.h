#ifndef ButtonDebounc_h
#define ButtonDebounce_h

#include "Arduino.h"

class ButtonDebounce {

    private:
        unsigned char _pin;             // Arduino Pin
        bool _pullUp;                   // Use of internal pull up resistor,
											// if not pull down is assumend,
											// standard: true
        bool _executeAtRelease;
        void (*_bFunc)();          		// function call at button press
		
        
        uint8_t _buttonHistory;
        uint8_t _mask;		
        uint8_t _comparator;
        uint8_t _historyInit;

        uint8_t readButton();

    public:
		// constructor
        ButtonDebounce(unsigned char pin);
        ButtonDebounce(unsigned char pin, bool pullUp, bool executeAtRelease);
        ButtonDebounce(unsigned char pin, bool pullUp, bool executeAtRelease, void (*bFunction)()) ;

		// set Methods
		bool setPullUp(bool pullUp);
        bool setFunction(void (*bFunction)());

        uint8_t getButtonHistory();
		
		// detect if a button is pressed
		// par: if function call should be executed
		// ret:	if button is pressed 
        bool isPressed(bool execute);
        void updateButton();

};
// function for internal use only. Standard function call for library
void nullFunction();

#endif

//EOF