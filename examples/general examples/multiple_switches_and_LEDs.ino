/*
   Ron D Bentley, Stafford, UK
   Mar 2021

   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   -      Example of use of the ez_switch_lib library        -
   Example 3.1
   Reading multiple button & toggle switches wired with
   different circuit types with each switch turning associated
   leds on/off.
   This example uses a struct(ure) data type to define the
   switch and led data.
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

   This example and code is in the public domain and
   may be used without restriction and without warranty.

*/
#include <Arduino.h>
#include <ez_switch_lib.h>  // ez_switch_lib .h & .cpp files are stored under ...\Arduino\libraries\ez_switch_lib\

#define not_configured     255  // used to indicate if a switch_control data entry has be configured

#define num_switches 4

// We will use a struct(ure) data type to keep our switch/LED
// data tidy and readily accessible
struct switch_control {     // struct member meanings:
  byte  sw_type;            // type of switch connected
  byte  sw_pin;             // digital input pin assigned to the switch
  byte  sw_circuit_type;    // the type of circuit wired to the switch
  byte  sw_id;              // holds the switch id given by the add.switch function for this switch
  byte  sw_led_pin;         // digital pin connecting the LED for this switch
  bool  sw_led_status;      // current status LOW/HIGH of the LED connected to this switch
} btl[num_switches] = {     // 'btl' = buttons, toggles & LEDs

  //.................switch data...............>  <led data, initial setting level>
  button_switch, 2, circuit_C1, not_configured,   8,  HIGH,
  button_switch, 3, circuit_C2, not_configured,   9,  LOW,
  toggle_switch, 4, circuit_C1, not_configured,   10, HIGH,
  toggle_switch, 5, circuit_C2, not_configured,   11,  LOW
};

// Declare/define the switch instance of given size
Switches my_switches(num_switches);

void setup() {
  // Attach each switch to its defined digital pin/circuit type
  // and store the switch's id back in its struct entry for later use.
  for (byte sw = 0; sw < num_switches; sw++) {
    int switch_id  = my_switches.add_switch(
                       btl[sw].sw_type,
                       btl[sw].sw_pin,
                       btl[sw].sw_circuit_type);
    // Validate the return
    if (switch_id < 0) {
      // Error returned - there is a data compatibility  mismatch (-2, bad_params),
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
    btl[sw].sw_id = switch_id; // store given switch id fo this sw for use later

    // Now initialise the switch's associated LED and turn on/off according to preset
    pinMode(btl[sw].sw_led_pin, OUTPUT);
    digitalWrite(btl[sw].sw_led_pin, btl[sw].sw_led_status);
  }
}

void loop() {
  // Keep reading the switches we have created and flp their
  // associated LEDs on/off
  do {
    for (byte sw = 0; sw < num_switches; sw++) {
      if (my_switches.read_switch(btl[sw].sw_id) == switched) {
        // Flip between HIGH and LOW each cycle
        btl[sw].sw_led_status = HIGH - btl[sw].sw_led_status;
        digitalWrite(btl[sw].sw_led_pin, btl[sw].sw_led_status);
      }
    }
  } while (true);
}
