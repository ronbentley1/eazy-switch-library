//
// A basic ez_switch_lib example - Quick Start, example 2
// A sketch configured for four switches sketch to flip the status of the in-built LED
// for Arduino this is on pin 13 (LED_BUILTIN) and for ESP 32 pin 2. This example is
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

#define max_switches 4
uint8_t My_Switches[max_switches][3] {
  button_switch, 2, INPUT,        // switch 0
  toggle_switch, 3, INPUT,        // switch 1 
  button_switch, 4, INPUT_PULLUP, // switch 2 
  toggle_switch, 5, INPUT_PULLUP  // switch 3 
};
//...
Switches my_switches(max_switches);
//...
void setup() {
  int result;
  for (uint8_t sw = 0; sw < max_switches; sw++) {
    result = my_switches.add_switch(My_Switches[sw][0], //switch type
                                    My_Switches[sw][1], //pin switch connected to
                                    My_Switches[sw][2]);//circuit type – pinMode
    if (result < 0) { // error reported – process error
      //...
    }
  }
  my_switches.set_debounce(50); // set debounce to 50 millisecs
  pinMode(LED, OUTPUT); // internal built in LED
  //...
}
//...
void loop() {
  //...
  do {
    for (uint8_t sw = 0; sw < max_switches; sw++) {
      if (my_switches.read_switch(sw) == switched) {
        // this switch (sw) has been actuated, so process it
        digitalWrite(LED, digitalRead(LED) ^ 1); // flip status of in built LED to show actuation
        //...
      }
      //...
    }
    //...
  } while (true);
}
