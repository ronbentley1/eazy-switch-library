//
// sketch combines a visual on/off indicator for a button switch
// using linked output (LED_BUILTIN) and a LED that illuminates
// whilst the button switch is in transition (pressed).
//

#include <ez_switch_lib.h>

Switches ms(1); // only 1 switch in this sketch

byte sw;  // stores switch id from add_switch function

#define button_pin          2
#define in_transition_LED   8 // visible indicator whilst sw in transition

void setup() {
  sw = ms.add_switch(button_switch, button_pin, circuit_C1); // set up the switch
  ms.link_switch_to_output(sw, LED_BUILTIN, LOW); // pin 13, LED_BUILTIN, indicates if sw is on or off
  pinMode(in_transition_LED, OUTPUT);             // visible indicator whilst sw in transition
}

void loop() {
  do {
    // keep reading/testing the switch for any changes,
    // the linked output will be flipped at each press/release
    // cycle automatically.
    ms.read_switch(sw);
    if (ms.switches[sw].switch_pending) {
      digitalWrite(in_transition_LED, on);   // button pressed but not yet released
    } else {
      digitalWrite(in_transition_LED, !on);  // button not in transition
    }
  } while (true);
}
