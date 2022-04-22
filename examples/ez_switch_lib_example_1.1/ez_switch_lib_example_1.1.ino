/*
   Ron D Bentley, Stafford, UK
   Mar 2021

   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   -        Example of use of the ez_switch_lib library        -
   Example 1.1
   Reading single button switch to turn built in led on/off.
   When the button switch is actuated, the in built led
   (LED_BUILTIN), will be flipped ON/OFF etc by using suitable
   coding in the sketch's main loop.
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

   This example and code is in the public domain and
   may be used without restriction and without warranty.

*/
#include <Arduino.h>
#include <ez_switch_lib.h>  // ez_switch_lib .h & .cpp files are stored under ...\Arduino\libraries\ez_switch_lib\

int  switch_id;
bool led_level = LOW;   // start with led off

#define num_switches 1  // only a single switch in this sketch example

// Declare/define the switch instance of given size
Switches my_switches(num_switches);

void setup() {
  // Attach a button switch to digital pin 2, with
  // an external pull down resistor, circuit_C1,
  // and store the switch's id for later use.
  switch_id = my_switches.add_switch(
                button_switch,
                2,
                circuit_C1);
  // validate the return
  if (switch_id < 0) {
    // Error returned - there is a data compatibilty mismatch (-2, bad_params),
    // or no room left to add switch (-1, add_failure).
    Serial.begin(9600);
    Serial.println(F("Failure to add a switch"));
    if (switch_id == add_failure) {
      Serial.println(F("add_switch - no room to create given switch"));
    } else {
      // Can only be that data for switch is invalid
      Serial.println(F("add_switch - one or more parameters is/are invalid"));
    }
    Serial.println(F("!!PROGRAM TERMINATED!!"));
    Serial.flush();
    exit(1);
  }
  // Inititialise built in led and turn to off
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  // Keep reading the switch we have created and toggle the built in
  // led on/off for each press.
  do {
    if (my_switches.read_switch(switch_id) == switched) {
      // Flip between HIGH and LOW each cycle
      led_level = HIGH - led_level;
      digitalWrite(LED_BUILTIN, led_level);
    }
  } while (true);
}
