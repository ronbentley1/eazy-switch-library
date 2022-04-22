//
// A basic ez_switch_lib example - Quick Start, example 1
// A single switch sketch designed to flip the status of the in-built LED
// for Arduino this is on pin 13 (LED_BUILTIN) and for ESP 32 pin 2 This example is
// configured for an Arduino microcontroller
//
// Compatible for both Arduino and ESP 32 microcontrollers
//
// Add your own code where required, eg where you see '...'
//
// R D Bentley (Stafford UK)
//
// This example and code is in the public domain and may only be
// used for non-commercial purposes without restriction and
// without warranty.
//
#include <ez_switch_lib.h>
//...
#define LED LED_BUILTIN // built in LED on Arduino boards
int switch_id; // use to record switch id we add to the class

//...
Switches my_switch(1); // create class size for 1 switch
//...
void setup() {
  // create switch with a 10k ohm external pull down resistor - INPUT pinMode parameter
  switch_id = my_switch.add_switch(button_switch,// switch type
                                   2,            // pin switch connected to
                                   INPUT);       // circuit type – pinMode
  if (switch_id < 0) { // error reported – process error
    //...
  }
  my_switch.set_debounce(50); // set debounce to 50 millisecs
  pinMode(LED, OUTPUT); // internal built in LED
  //...
}
//...
void loop() {
  //...
  do {
    if (my_switch.read_switch(switch_id) == switched) {
      // this switch (swith_id) has been actuated, so process it
      digitalWrite(LED, digitalRead(LED) ^ 1); // flip status of in built LED to show actuation
      //...
    }
    //...
  } while (true);
}
