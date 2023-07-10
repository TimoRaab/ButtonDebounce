# ButtonDebounce
Library for push buttons (software debounce), Work in progress

A class for push buttons which is pretty debounce resistant. 
Idea after https://hackaday.com/2015/12/10/embed-with-elliot-debounce-your-noisy-buttons-part-ii/ form Elliot Williams. 

For usage you have to call the updateButton-Method regularly, I recommened a task scheduler. Examples will follow soon. 

Check for button presses with isPressed and isLongPressed. The return value will give you if the button is pressed, the parameter only chooses if you want to execute the corresponding function for the button. 


