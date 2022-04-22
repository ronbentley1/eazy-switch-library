//
// A basic ez_switch_lib example - Quick Start, example 3
// A sketch configured for four switches sketch.  The sketch will:
// 1. flip the status of the in-built LED for all switch actuations
// 2. flip the linked output pins defined for 3 of the switches. The serial
//    monitor is used to show that the linked pins are flipped each time
//    its associated switch is actuated.
//
// For Arduino this is on pin 13 (LED_BUILTIN) and for ESP 32 pin 2. This example is
// configured for an ESP 32 microcontroller
//
// Compatible for both Arduino and ESP 32 microcontrollers, but note:
//
//   Not all ESP 32 GPIO pins have a pull_up/pull_down capability.
//   Those that do not are documented as:
//     GPIO pins 34-39 – these can only be set as input mode and
//     do not have software enabled pullup or pulldown functions.
//     (Source: espessif)
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
#define LED 2 // this is the pin of the internal built LED on ESP 32

#define max_switches 4
uint8_t My_Switches[max_switches][5] {
  button_switch, 23, INPUT_PULLDOWN, 21,  LOW, // switch 0, start with linked pin LOW
  toggle_switch, 22, INPUT_PULLUP,   19, HIGH, // switch 1, start with linked pin HIGH
  button_switch, 25, INPUT_PULLUP,    0,    0, // switch 2, no linked  pin on this switch
  toggle_switch, 26, INPUT_PULLDOWN, 18,  LOW  // switch 3, start with linked pin LOW
};
//...
Switches my_switches(max_switches); // instantiate the class for the required number of switches
//...
void setup() {
  int result;
  for (uint8_t sw = 0; sw < max_switches; sw++) {
    result = my_switches.add_switch(My_Switches[sw][0], //switch type
                                    My_Switches[sw][1], //GPIO pin switch connected to
                                    My_Switches[sw][2]);//circuit type – pinMode
    if (result < 0) { // error reported – process error
      //...
    }
    if (My_Switches[sw][3] != 0) { // there is an output pin to be linked
      result = my_switches.link_switch_to_output(sw, // id of switch to be linked
               My_Switches[sw][3], // GPIO link
               My_Switches[sw][4]);// initial setting of link
      if (result < 0) { // error reported – process error
        //...
      }
    }
  }
  my_switches.set_debounce(50); // set debounce to 50 millisecs
  pinMode(LED, OUTPUT); // internal built in LED
  Serial.begin(115200); // open serial monitor
  //...
}
//...
void loop() {
  //...
  do {// keep reading switches each loop
    for (uint8_t sw = 0; sw < max_switches; sw++) {
      if (my_switches.read_switch(sw) == switched) {
        // this switch (sw) has been actuated, so process it
        // flip status of in built LED to show actuation for all switches
        // but report switch status to the serial monitor also
        digitalWrite(LED, digitalRead(LED) ^ 1); // flip in-built LED
        // now report status of this sw(itch) which has been actuated
        Serial.print("switch ");
        Serial.print(sw);
        Serial.print(" (");
        if (My_Switches[sw][0] == button_switch)Serial.print("button switch)"); else
        Serial.print("toggle switch)");
        Serial.print(" actuated on pin ");
        Serial.print(My_Switches[sw][1]);        // the pin this sw(itch) is connected to
        uint8_t linked_pin = My_Switches[sw][3]; // the pin this sw(itch) is linked to
        if (linked_pin != 0) {
          // this switch has a linked output so read that pins current status
          // and report
          bool pin_status = digitalRead(linked_pin);
          Serial.print(", linked pin is ");
          Serial.print(linked_pin);
          Serial.print(", current status is ");
          if (pin_status == HIGH)Serial.println("HIGH"); else Serial.println("LOW");
        } else {
          Serial.println(", no linked pin");
        }
        Serial.flush();
        //...
      }
      //...
    }
    //...
  } while (true);
}
