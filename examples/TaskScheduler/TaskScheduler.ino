#include <ButtonDebounce.h>
#include <TaskScheduler.h>

void printButtonPress();
void longButtonPress();

void updateButtons();

ButtonDebounce p1 = ButtonDebounce(35, true, false, *printButtonPress);
ButtonDebounce p2 = ButtonDebounce(36, true, true, *longButtonPress);
int counter = 0;
int counterLong = 1;

Task t1(15, TASK_FOREVER, &updateButtons);

Scheduler buttonRunner;

void setup() {
  Serial.begin(115200);
  Serial.println("Startup");
  counter = 1;
  counterLong = 1;
  p1.setLongPressDuration(1000);
  p1.setLongPressFunction(*longButtonPress);

  buttonRunner.init();
  buttonRunner.addTask(t1);
  t1.enable();
  buttonRunner.startNow();
}

void loop() {
  // you have to update the button regularly 
  buttonRunner.execute();
  p1.isPressed(true);
  p1.isLongPressed(true);
  //Parameter only sets, if the stored function should be executed
  //To get the information if a button is pressed, use the return value
  if (p2.isPressed(false)) {
    test4Locking();
  }

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

void test4Locking() {
  Serial.println("inside locking");
  unsigned long tmpMillis = millis();
  unsigned long tmpMillisVol = tmpMillis;
  while (!p1.isPressed(false)) {
    buttonRunner.execute();
    if (millis()-tmpMillisVol > 1000) {
      Serial.println((millis()-tmpMillis)/1000);
      tmpMillisVol = millis();
    }
  }
}

void updateButtons() {
  p1.updateButton();
  p2.updateButton();
}