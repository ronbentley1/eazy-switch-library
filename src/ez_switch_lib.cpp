// Arduino Switch Library for configuring different switch type wired
// in common circuit schemes.
//
// Ron Bentley, Stafford (UK), March 2021, updated Sept 2022, version 3.0
//
// This example and code is in the public domain and
// may be used without restriction and without warranty.
//

#include <Arduino.h>
#include <ez_switch_lib.h> // change "ez_switch_lib" to <ez_switch_lib> when deploying to libraries

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Set up switch cotrol structure and initialise internal variables.
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Switches::Switches(byte max_switches)
{
  // Establish the switch control structure (switches) of the size required.
  //
  switches = (switch_control *)malloc(sizeof(*switches) * max_switches);
  if (switches == NULL) {
    // malloc failure
    Serial.begin(115200);
    Serial.println("!!Failure to acquire memory of required size - PROGRAM TERMINATED!!");
    Serial.flush();
    exit(1);
  }

  // Initialise private variables
  _num_entries  = 0;            // will be incremented each time a switch is added, up to _max_switches
  _max_switches = max_switches; // transfer to internal variable
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Generic switch read function.
// Read the switch defined by the function parameter.
// Function returns a value indicating if the switch
// has undergone a transition or not.
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool Switches::read_switch(byte sw) {
  bool sw_status;
  if (sw < 0 || sw >= _num_entries) return !switched; // out of range, slot 'sw' is not configured with a switch
  if (switches[sw].switch_type == button_switch) {
    sw_status = read_button_switch(sw);
  } else {
    sw_status = read_toggle_switch(sw);
  }
  // now determine if switch has output pin associated and if switched
  // flip the output's status, ie HIGH->LOW, or LOW->HIGH
  if (sw_status == switched && switches[sw].switch_out_pin > 0)
  {
    // flip the output level of associated switch output pin, if defined
    bool status = switches[sw].switch_out_pin_status; // last status value of out_pin
    status = HIGH - status;                           // flip the status value
    switches[sw].switch_out_pin_status = status;      // update current status value
    digitalWrite(switches[sw].switch_out_pin, status);// change status of linked pin
  }
  return sw_status;
}  // End of read_switch

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Generic toggle switch read function.
// Test the toggle switch to see if its status has changed since last look.
// Note that although switch status is a returned value from the function,
// the current status of the switch ('switches[sw].switch_status') is always
// maintained and can be tested outside of the function at any point/time.
// It will either have a status of 'on' or '!on' (ie off).
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool Switches::read_toggle_switch(byte sw) {
  byte switch_pin_reading = digitalRead(switches[sw].switch_pin);  // test current state of toggle pin
  if (switches[sw].switch_circuit_type == circuit_C2) {
    // Need to invert HIGH/LOW if circuit design sets
    // pin HIGH representing switch in off state.
    // ie inititialised as INPUT_PULLUP
    switch_pin_reading = !switch_pin_reading;
  }
  if (switch_pin_reading != switches[sw].switch_status && !switches[sw].switch_pending) {
    // Switch change detected so start debounce cycle
    switches[sw].switch_pending = true;
    switches[sw].switch_db_start = millis();  // set start of debounce timing
  }
  if (switches[sw].switch_pending) {
    // We are in the switch transition cycle so check if debounce period has elapsed
    if (millis() - switches[sw].switch_db_start >= _debounce) {
      // Debounce period elapsed so assume switch has settled down after transition
      switches[sw].switch_status  = !switches[sw].switch_status;  // flip status
      switches[sw].switch_pending = false;                        // cease transition cycle
      last_switched_id = sw;   // indicates the last switch to have been processed by read function
      return switched;
    }
  }
  return !switched;
} // End of read_toggle_switch

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Read button switch function.
// Generic button switch read function.
// Reading is controlled by:
//   a. the function parameter which indicates which switch
//      is to be polled, and
//   b. the switch control struct(ure), referenced by a).
//
// Note that this function works in a nonexclusive way
// and incorporates debounce code.
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool Switches::read_button_switch(byte sw) {
  byte switch_pin_reading = digitalRead(switches[sw].switch_pin);
  if (switch_pin_reading == switches[sw].switch_on_value) {
    // Switch is pressed (ON), so start/restart debounce process
    switches[sw].switch_pending = true;
    switches[sw].switch_db_start   = millis();  // start elapse timing
    return !switched;                           // now waiting for debounce to conclude
  }
  if (switches[sw].switch_pending && switch_pin_reading != switches[sw].switch_on_value) {
    // Switch was pressed, now released (OFF), so check if debounce time elapsed
    if (millis() - switches[sw].switch_db_start >= _debounce) {
      // debounce time elapsed, so switch press cycle complete
      switches[sw].switch_pending = false;
      last_switched_id = sw;   // indicates the last switch to have been processed by read function
      return switched;
    }
  }
  return !switched;
}  // End of read_button_switch

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Add given switch to switch control structure, but validate
// given paramters and ensure there is a free slot.
//
// Return values from add_switch are:
//
//    >= 0 the switch control structure entry number ('switch_id')
//         for the switch added,
//      -1 add_falure - no slots available in the switch
//         control structure,
//      -2 bad_params - given paramter(s) for switch are
//         not valid.
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

int Switches::add_switch(byte sw_type, byte sw_pin, byte circ_type) {
  if ((sw_type != button_switch && sw_type != toggle_switch) ||
      (circ_type != circuit_C1 &&
       circ_type != circuit_C2 &&
       circ_type != circuit_C3)) return bad_params;  // bad paramters
  if (_num_entries < _max_switches) {
    // room to add another switch...initialise the switch's
    // data depending on type of switch and circuit
    switches[_num_entries].switch_type         = sw_type;
    switches[_num_entries].switch_pin          = sw_pin;
    switches[_num_entries].switch_circuit_type = circ_type;
    switches[_num_entries].switch_pending      = false;
    switches[_num_entries].switch_db_start     = 0;
    // define what on means for this switch:
    if (circ_type != circuit_C2) {
      // circuit_C1 and circuit_C3 are used with pull down resistors
      // if circuit_C1 (INPUT) then and external 10k ohm resistor needs to be fitted
      // to the switch circuit, but if circuit_C3 (INPUT_PULLDOWN) then the resistor
      // is internal to the microcontroller.
      // Note that the 'on' state is represented by HIGH (3.3/5v)
      switches[_num_entries].switch_on_value = HIGH;
    } else {
      // circuit_C2 (INPUT_PULLUP) with internal microcontroller pull up resistor
      // Note that the 'on' state is represented by LOW (0v)
      switches[_num_entries].switch_on_value = LOW;
    }
    if (sw_type == button_switch) {
      switches[_num_entries].switch_status = not_used;
    } else {
      switches[_num_entries].switch_status = !on;
    }
    pinMode(sw_pin, circ_type);  // establish pin set up
    // ensure no mapping to an output pin until created explicitly
    switches[_num_entries].switch_out_pin        = 0;
    switches[_num_entries].switch_out_pin_status = LOW;  // set LOW unless explicitly changed

    _num_entries++;              // point to next free slot
    return _num_entries - 1;     // return 'switch_id' - given switch now added to switch control structure
  } else return add_failure;     // no room left to add another switch!
}  // End add_switch


// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Link or delink the given switch to the given digital pin as an output
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

int Switches::link_switch_to_output(byte switch_id, byte output_pin, bool HorL) {
  if (switch_id > _num_entries) {
    return link_failure; // no such switch
  }
  if (output_pin == 0) {
    // delink this output from this switch, set the output to the required level first
    if (switches[switch_id].switch_out_pin == 0) {
      // no output pin previously defined
      return link_failure;
    }
    // set existing pin to level required state before clearing the link
    digitalWrite(switches[switch_id].switch_out_pin, HorL);
  } else {
    // initialise given output pin
    pinMode(output_pin, OUTPUT);
    digitalWrite(output_pin, HorL); // set to param value until switched
  }
  switches[switch_id].switch_out_pin        = output_pin;
  switches[switch_id].switch_out_pin_status = HorL;
  return link_success;           // success
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Return the number of slots left unused
// in the switch control structure.
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

int Switches::num_free_switch_slots() {
  return _max_switches - _num_entries;
} // End num_free_switch_slots

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Set debounce period (milliseconds).
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void Switches::set_debounce(int period) {
  if (period >= 0)  _debounce = period;
}  // End set_debounce

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % reset the given switch to its
// % non-pending (non-transition) state
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void Switches::reset_switch(uint8_t switch_id) {
  if (switch_id < _num_entries - 1) {
    switches[switch_id].switch_pending  = false;
  }
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % reset all declared switches to
// % their non-pending (non-transition) state
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void Switches::reset_switches() {
  for (uint8_t switch_id = 0; switch_id < _num_entries; switch_id++) {
    switches[switch_id].switch_pending  = false;
  }
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// %    Note: this function applies to button
// %             switches ONLY.
// % If the specified switch id is a button
// % switch, then the function will return
// % true if the button switch is CURRENTLY
// % being pressed, ie in transition, and false
// % otherwise.
// % Note: If the button has a linked output then this 
// % will be automatically switched (toggled)
// % if the 'process_link' parameter is true,
// % otherwise, it will not be switched (toggled)
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool Switches::button_is_pressed(uint8_t switch_id, bool process_link) {
  // check switch is declared
  if (switch_id < _num_entries) {
    // check if switch is a button switch
    if (switches[switch_id].switch_type == button_switch) {
      // all checks done, read the switch
      if (process_link) {
        // request is to process any lined output.
        // only the read_switch function will process any linked output
        read_switch(switch_id);
      } else {
        // request is NOT to process any linked output, so we need to use
        // read_button_switch rather than read_switch, which ignores any
        // linked output
        read_button_switch(switch_id);
      }
      return switches[switch_id].switch_pending; // this will be either true if in trasition or false if not
    }
  }
  return false;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// %    Note: this function applies to button
// %             switches ONLY.
// % If the specified switch id is a button
// % switch, then the function will return
// % true if the button switch is CURRENTLY
// % being pressed, ie in transition, and false
// % otherwise.
// % Note that this function will NOT process
// % any linked output associated with this
// % switch. If this is required then use the
// % overloaded version, thus:
// % button_is_pressed(switch_id, true);
// %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool Switches::button_is_pressed(uint8_t switch_id) {
  return button_is_pressed(switch_id, false);
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Print given switch control data.
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void Switches::print_switch(byte sw) {
  if (0 <= sw && sw < _num_entries ) {
    Serial.print(F("sw_id:   = "));
    Serial.println(sw);
    Serial.print(F("sw_type  = "));
    uint8_t sw_type = switches[sw].switch_type;
    if (sw_type == button_switch)Serial.print(F("BUTTON SWITCH"));
    else Serial.print(F("TOGGLE SWITCH"));
    Serial.print(F("  sw_pin  = "));
    Serial.print(switches[sw].switch_pin);
    Serial.print(F("\tcirc_type = "));
    uint8_t circ_type = switches[sw].switch_circuit_type;
    if (circ_type == circuit_C1) {
      Serial.print(F("INPUT/circuit_C1/"));
    } else if (circ_type == circuit_C2) {
      Serial.print(F("INPUT_PULLUP/circuit_C2/"));
    } else {
      Serial.print(F("INPUT_PULLDOWN/circuit_C3/"));
    }
    Serial.println(circ_type);
    Serial.print("on_value = ");
    if (switches[sw].switch_on_value == HIGH)Serial.print(F("HIGH"));
    else Serial.print(F("LOW "));
    Serial.print(F("\t\tsw_status = "));
    uint8_t sw_status = switches[sw].switch_status;
    if (sw_type == button_switch)Serial.print(F("n/a")); else {
      if (sw_status == on)Serial.print(F("ON"));
      else Serial.print(F("OFF"));
    }
    Serial.print(F("\tpending   = "));
    if (switches[sw].switch_pending)Serial.print(F("YES"));
    else Serial.print(F("NO "));
    Serial.print(F("\tdb_start = "));
    Serial.print(switches[sw].switch_db_start);
    Serial.println(F(" msecs"));
    if (switches[sw].switch_out_pin == 0) {
      // switch does not have a linked output
      Serial.println(F("*** No linked output pin"));
    } else {
      // switch has a linked output
      Serial.print(F("Linked output pin = "));
      Serial.print(switches[sw].switch_out_pin);
      Serial.print(F("\tlinked pin status = "));
      if (switches[sw].switch_out_pin_status == LOW)Serial.println(F("LOW"));
      else Serial.println(F("HIGH"));
    }
    Serial.println();
    Serial.flush();
  }
} // End print_switch

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Print all switch control data set up.
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void Switches::print_switches() {
  Serial.println(F("\nDeclared & configured switches:"));
  for (byte sw = 0; sw < _num_entries; sw++) {
    print_switch(sw);
  }
} // End print_switches