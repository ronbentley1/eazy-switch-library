/*
   Ron D Bentley, Stafford, UK
   Mar 2021

   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   -      Example of use of the ez_switch_lib library        -
   Example 2.2
   Reading single toggle switch to turn built in led on/off.
   Toggle switch is associated with an output pin (LED_BUILTIN)
   using a ez_switch_lib function, so that when activated, the
   output pin will be automatically flipped, HIGH-LOW etc  each
   time the toggle switch is actuated WITHOUT any further coding.
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

   This example and code is in the public domain and
   may be used without restriction and without warranty.

*/
#include <Arduino.h>
#include <ez_switch_lib.h>  // ez_switch_lib .h & .cpp files are stored under ...\Arduino\libraries\ez_switch_lib\

int  switch_id;

#define num_switches 1  // only a single switch in this sketch example

// Declare/define the switch instance of given size
Switches my_switches(num_switches);

void setup() {
  // Attach a toggle switch to digital pin 2, with
  // an external pull down resistor, circuit_C1,
  // and store the switch's id for later use.
  switch_id = my_switches.add_switch(
                toggle_switch,
                2,
                circuit_C1);
  // Validate the return
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

  // Link/associate the LED_BUILT digital pin (normally pin 13)
  // with the switch we have justed installed/created
  // so that every time the switch is activated the built in
  // LED will be automatically be flipped.  Start with LED at HIGH setting.
  int link_result = my_switches.link_switch_to_output(
                      switch_id,
                      LED_BUILTIN,
                      HIGH);
  if (link_result == link_failure) {
    // Linking failed, invalid switch id
    Serial.begin(9600);
    Serial.println(F("Failure to link an output to a switch"));
    Serial.println(F("!!PROGRAM TERMINATED!!"));
    Serial.flush();
    exit(2);
  }
}

void loop() {
  do {
    if (my_switches.read_switch(switch_id) == switched) {
      // just keep reading, LED_BUILTIN will automatically be flipped for us
      // so we dont need to do anything else
    }
  } while (true);
}
