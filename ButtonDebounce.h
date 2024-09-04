/**
 * @file ButtonDebounce.h
 * @author Timo Raab
 * @brief Register Button Presses with Software Debounce
 * @version 1.2
 * @date 2023-12-23
 * 
 * @copyright Copyright (c) 2023
 * 
 * Buttons class for momentary buttons (not switches).
 * The class allows for far better debouncing in buttons.
 * Debouncing is completely done via software.
 *
 * @note Idea after 
 * https://hackaday.com/2015/12/10/embed-with-elliot-debounce-your-noisy-buttons-part-ii/
 * from  Elliot Williams
 * 
 */

#ifndef ButtonDebounce_h
#define ButtonDebounce_h

#include "Arduino.h"


/**
 * @brief Button Class using software debounce
 * 
 */
#define EXECUTENUMBERALL 3
#define EXECUTENUMBERISPRESSED 2
#define EXECUTENUMBERSTILLPRESSED 1
#define EXECUTENUMBERNONE 0

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
        /**
         * @brief Construct a new Button Debounce object
         * 
         * @param pin Physical pin at microcontroller
         * @param pullUp Chooses if internal pullup resistor should be used. Does not check if pullup is present.
         * @param executeAtRelease True: Function is executed at button release.
         *                          False: Function is executed at button press.
         * @param bFunction Pointer to function which shall be executed at button press
         * 
         * Constructor for ButtonDebounce object. A pin is required, all other arguemnts are optional. Additionally the longPressDuration is set to 1000ms, no longpress function is registered. This has to be done via the corresponding setter methods.
         */
        ButtonDebounce(unsigned char pin, bool pullUp = true, bool executeAtRelease = false, void (*bFunction)() = nullptr) ;

        /**
         * @name Setter Methods
         * @brief Setter Methods for objects of the ButtonDebounce class.
         *  
         * All Setter Methods available. The functions always returns a true value.
         * Pin cannot be changed after initialization.
         * If a long press behavior is wanted, one can set the duration (Standard is 1000ms)
         * and the function, which shall be executed automatically.
         */
		bool setPullUp(bool pullUp);
        bool setExecuteAtRelease(bool executeAtRelease);
        bool setFunction(void (*bFunction)());
        /** Set the duration that a long press is registered*/
        bool setLongPressDuration(unsigned long duration);
        /** Set the function which can be automatically called when a long press is registered.*/
        bool setLongPressFunction(void (*bFunction)());
        ///@}
		
        /**
         * @brief Checks if the button is short pressed. 
         * @param execute If the corresponding function should be executed
         * @return if the button is pressed
         */
        bool isPressed(bool execute = false);


        /**
         * @brief Checks if the button is long pressed. 
         * @param execute If the corresponding function should be executed
         * @return if the button is long pressed
         */
        bool isLongPressed(bool execute = false);

        /**
         * @brief Checks if a Button is still pressed
         * 
         * @param execute If the corresponding function should be executed
         * @return true if the button is pressed
         * @return false if the button is not pressed
         */
        bool stillPressed(bool execute = false);

        /**
         * @brief Checks if a button is newly pressed or still pressed
         * 
         * @param execute If the corresponding function should be executed
         * @return true if the button is pressed
         * @return false if the button is not pressed 
         */
        bool anyPressed(bool execute = false);

        /**
         * @brief Checks if a button is newly pressed or still pressed
         * 
         * @param executeNumber If and when the corresponding function should be executed
         * @return true if the button is pressed
         * @return false if the button is not pressed 
         */
        bool anyPressed(uint8_t executeNumber = EXECUTENUMBERNONE);

        /**
         * @brief Has to be called regularly! Update the current status of the button.
         * This method reads the current status of the button and stores in history. 
         * 
         * This has to be called regularly to register button presses! If not called regularly, button presses will be missed!
         */
        void updateButton();

        /**
         * @brief For Debugging. Get current button history.
         * @return Current button history
         * This method is solely meant for class debugging purposes. Can be used e.g. for checking if updating the button history works.
         */
        uint8_t getButtonHistory();
};

#endif

//EOF