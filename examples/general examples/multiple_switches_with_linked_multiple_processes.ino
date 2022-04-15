/*
   Ron D Bentley, Stafford, UK
   Mar 2021.

   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   -          Example of use of the ez_switch_lib library           -
   Example 4
   Reading multiple swiches (6) of different types and of mixed wiring
   schemes. Additionally, some switches are linked to a digital
   output such that when they are actuated the linked output level
   is flipped (HIGH->LOW, or LOW->HIGH) automatically without the
   need for any end user coding.

   Swith data in this example is preset in a two dimension array
   and may be varied as appropriate.

   The sketch is configured for 6 switches, 3 toggle and 3 button
   with switches performing the following actions:
   toggle 1 switches a relay, without direct coding, using output linking
   toggle 2 switches a led, without direct coding, using output linking
   toggle 3 also switches a led, but not via switch linking
   button 1 produces a switch report using a ez_switch_lib function
   button 2 switches a relay, without direct coding, using output linking
   button 3 switches a led, without direct coding, using output linking
   
   The switches are polled in succession and processing occurs
   via a switch-case set of control statements.
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

   This example and code is in the public domain and
   may be used without restriction and without warranty.

*/
#include <Arduino.h>
#include <ez_switch_lib.h>  // ez_switch_lib .h & .cpp files are stored under ...\Arduino\libraries\ez_switch_lib\

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Declare/define specific 'my_data' for 'my_project'
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#define num_switches 6

// Switch to Pin Macro Definition List:
#define my_toggle_pin_1  2 // digital pin number
#define my_toggle_pin_2  3 // etc
#define my_toggle_pin_3  4

#define my_button_pin_1  5
#define my_button_pin_2  6
#define my_button_pin_3  7

// Digital output pins for linking to switches:
#define relay_1    8  // output pin for relay 1
#define relay_2    9  // output pin for relay 2
#define led_1     10  // output pin for led 1
#define led_2     11  // output pin for led 2
#define led_3     12  // output pin for led 3

#define not_configured 255  // used to indicate if the my_switch_data switch output pin is to be configured

// Establish type of switch, assigned digital pin and circuit type
// for each switch we are connecting. Until we present each
// switch entry to the add.switch function it will not be
// recorded as configured, hence the use of the final column.
//
// Array row definitions are:
// [sw][0] = switch type, button or toggle
// [sw][1] = digital input pin for the switch
// [sw][2] = how the switch is wired/connected
// [sw][3] = this stores the switch_id returned from add.switch function
//           to be used in all calls the the library's functions where
//           switches are referenced
// [sw][4] = the digital output pin linked to this switch, if defined
// [sw][5] = the level the output pin is to be set to at initialisation (linking),
//           if a linked output is configured - must be LOW or HIGH
//
// Note that:
// 'on', 'switched', 'button_switch', 'toggle_switch', 'circuit_C1'
// and 'circuit_C2' are library defined macros.

byte my_switch_data[num_switches][6] =
{
  // <................switch data..............>  <.output pin data.>
  toggle_switch,  my_toggle_pin_1, circuit_C1, 0, relay_1,        HIGH,// linked to relay_1
  toggle_switch,  my_toggle_pin_2, circuit_C2, 0, led_1,           LOW,// linked to led_1
  toggle_switch,  my_toggle_pin_3, circuit_C1, 0, not_configured,    0,// not linked, flip led_3 by direct code
  button_switch,  my_button_pin_1, circuit_C2, 0, not_configured,    0,// not linked, produces switch report
  button_switch,  my_button_pin_2, circuit_C1, 0, relay_2,        HIGH,// linked to relay_2
  button_switch,  my_button_pin_3, circuit_C2, 0, led_2,           LOW // linked to led_2
};

// Declare/define the switch instance of given size
Switches my_switches(num_switches);

//
// Set up connected switches as per 'my_switch_data' configs
//

void setup()
{
  Serial.begin(9600);
  // Create/install the defined switches...
  create_my_switches();

  // Set debounce for 20 msecs
  my_switches.set_debounce(20);

  // initialise the output pin for led_3, as we will deal with
  // flipping this led by direct coding
  pinMode(led_3, OUTPUT);
  digitalWrite(led_3, LOW);
}

void loop()
{
  do {
    // Poll all switches - examine each connected switch in turn and, if switched,
    // process its associated purpose.
    for (int sw = 0; sw < num_switches; sw++) {
      byte switch_id = my_switch_data[sw][3];  // extract the switch id for this switch, sw
      if (my_switches.read_switch(switch_id) == switched) {
        // This switch ('switch_id') has been pressed, so process via its switch-case code
        if (my_switches.switches[switch_id].switch_type == button_switch) {
          Serial.print(F("\nbutton switch on digital pin "));
        } else {
          Serial.print(F("\ntoggle switch on digital pin "));
        }
        byte my_switch_pin = my_switches.switches[switch_id].switch_pin;
        Serial.print(my_switch_pin);
        Serial.println(F(" triggered"));
        // Move to switch's associated code section
        switch (my_switch_pin)
        {
          case my_toggle_pin_1:
            // toggle switch 1 triggers a relay (1) which is a linked output
            // so nothing to do here to process the relay
            Serial.print(F("relay 1 switched"));
            break;
          case my_toggle_pin_2:
            // toggle switch 2 flips a led (1) which is a linked output
            // so nothing to do here to process the relay
            Serial.print(F("led 1 switched"));
            break;
          case my_toggle_pin_3:
            // direct coding to flip led_3 following switch actuation (toggle 3)
            static bool led_3_status = LOW; // static because we need to retain current state between switching
            led_3_status = HIGH - led_3_status; // flip led status
            digitalWrite(led_3, led_3_status);
            Serial.print(F("led 3 switched "));
            break;
          case my_button_pin_1:
            // button switch 1 used to reveal the current status of the switch control structure
            // members, number of free switch control slots and the on/off status of all
            // all toggle switches as their status is maintained
            my_switches.print_switches();  // confirm we are set up correctly
            // Report number of free switch slots remaining
            Serial.print(F("\nNumber of free switch slots remaining = "));
            Serial.println(my_switches.num_free_switch_slots());
            // Report on the current status of the toggle switches
            print_toggle_status();
            Serial.flush();
            break;
          case my_button_pin_2:
            // button switch 2 triggers a relay (2) which is a linked output
            // so nothing to do here to process the relay
            Serial.print(F("relay 2 switched"));
            break;
          case my_button_pin_3:
            // button switch 3 flips a led (2) which is a linked output
            // so nothing to do here to process the relay
            Serial.print(F("led 2 switched"));
            break;
          default:
            // Spurious switch index!  Should never arise as this is controlled
            // by the for loop within defined upper bound
            break;
        }
        Serial.flush();  // flush out the output buffer
      }
    }
  }
  while (true);
}

//
// Print the current status/setting of each toggle switch configured.
// We scan down my_switch_data to pick out toggle switches and if they
// are configured access their status.
//

void print_toggle_status() {
  Serial.println(F("\nToggle switches setting: "));
  for (byte sw = 0; sw < num_switches; sw++) {
    if (my_switch_data[sw][0] == toggle_switch) {
      Serial.print(F("toggle switch on digital pin "));
      Serial.print(my_switch_data[sw][1]);
      Serial.print(F(" is "));
      byte switch_id = my_switch_data[sw][3]; // this is the position in the switch control struct for this switch
      if (my_switches.switches[switch_id].switch_status == on) {
        Serial.println(F("ON"));
      } else {
        Serial.println(F("OFF"));
      }
    }
  }
}

//
// Create a switch entry for each wired up switch, in accordance
// with 'my' declared switch data.
// add_switch params are - switch_type, digital pin number and circuit type.
// Return values from add_switch are:
//    >= 0 the switch control structure entry number ('switch_id') for the switch added,
//      -1 no slots available in the switch control structure,
//      -2 given parameter(s) for switch are not valid.

void create_my_switches() {
  for (int sw = 0; sw < num_switches; sw++) {
    int switch_id =
      my_switches.add_switch(
        my_switch_data[sw][0], // switch type
        my_switch_data[sw][1], // digital pin number
        my_switch_data[sw][2]);// circuit type
    if (switch_id < 0)
    { // There is a data compatibilty mismatch (-2, bad_params),
      // or no room left to add switch (-1, add_failure).
      Serial.print(F("Failure to add a switch:\nSwitch entry:"));
      Serial.print(sw);
      Serial.print(F(", data line = "));
      Serial.print(my_switch_data[sw][0]);
      Serial.print(F(", "));
      Serial.print(my_switch_data[sw][1]);
      Serial.print(F(", "));
      Serial.println(my_switch_data[sw][2]);
      Serial.println(F("!!PROGRAM TERMINATED!!"));
      Serial.flush();
      exit(1);
    } else {
      // 'switch_id' is the switch control slot entry for this switch (sw),
      // so we can use this to know where our switches are
      // in the control structure by keeping a note of them in their
      // my_switch_data config settings.
      my_switch_data[sw][3] = switch_id;

      // Now deal with any linked output requirement
      if (my_switch_data[sw][4] != not_configured) {
        // there is an output defined for this switch, so link it
        int link_result =
          my_switches.link_switch_to_output(
            switch_id,              // id of switch to link output to
            my_switch_data[sw][4],  // digital output pin number
            my_switch_data[sw][5]); // initial level, HIGH or LOW
        if (link_result == link_failure ) {
          // linking failed, invalid switch id
          Serial.println(F("Failure to link an output to a switch"));
          Serial.println(F("!!PROGRAM TERMINATED!!"));
          Serial.flush();
          exit(2);
        }
      }
    }
  }
}  // End create_my_switches
