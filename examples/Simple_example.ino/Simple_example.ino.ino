#include <ButtonDebounce.h>

void printButtonPress();
void longButtonPress();


ButtonDebounce p1 = ButtonDebounce(35, true, false, *printButtonPress);
ButtonDebounce p2 = ButtonDebounce(36, true, true, *longButtonPress);
ButtonDebounce p3 = ButtonDebounce(37); // Not used in this sketch, just for showing
ButtonDebounce p4 = ButtonDebounce(38, true, false); // not used in this sketch, just for showing
int counter = 0;
int counterLong = 1;

void setup() {
  Serial.begin(115200);
  Serial.println("Startup");
  counter = 1;
  counterLong = 1;
  p1.setLongPressDuration(1000);
  p1.setLongPressFunction(*longButtonPress);
}

void loop() {
  // you have to update the button regularly 
  p1.updateButton();
  p2.updateButton();
  p1.isPressed(true);
  p1.isLongPressed(true);
  //Parameter only sets, if the stored function should be executed
  //To get the information if a button is pressed, use the return value
  if (p2.isPressed(false)) {
    longButtonPress();
  }

  if (p2.isLongPressed()) {
    printButtonPress();
  }

  delay(10);
}


void printButtonPress() {
    Serial.print("Pressed: ");
    Serial.println(counter);
    counter++;
}

void longButtonPress() {
  Serial.print("PressedLong: ");
  Serial.println(counterLong);
  counterLong++;
}