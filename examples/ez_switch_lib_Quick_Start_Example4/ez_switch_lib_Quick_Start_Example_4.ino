//
// A basic ez_switch_lib example - Quick Start, example
// A sketch configured for four switches sketch.  The sketch will
// demonstrate how we are able to use external interrupt routines (ISRs)
// by linking switches to output pins such that when a switch is actuated
// the associated ISR is triggered.
// The in-built LED is used to indicate a switch actuation, plus the serial
// monitor is also used to confirm that the ISR is entered for any switch
// for which a linked output is defined.
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

#define common_interrupt_pin 21 // external (common) interrupt GPIO pin that all linked switches will use

#define max_switches 4

// This array (My_Switches)includes all of the configuration data for the switch(es)
// to be defined. Switch types and wiring schemes may be mixed.  Each row
// of the array references the configuration data for each switch to be defined.
// My_Switches array - rows are defined as:
// [row][0] - switch type, either 'button_switch' or 'toggle_switch', these are
//            reserved library macros and may be used without declaration
// [row][1] - digital pin assigned to the switch
// [row][2] - circuit type switch is wired as and initialised for via a pinMode call:
//            This parameter must take one of three values:
//            1. INPUT, or circuit_C1. This requires an external 10k ohm pull down resistor
//               to be wired in the switch circuit
//            2. INPUT_PULLUP, or circuit_C2. NO external pull up resistor is required
//            3. INPUT_PULLDOWN, or circuit_C3. NO external pull down resistor is required
//
//            Note: INPUT, INPUT_PULLUP, INPUT_PULLDOWN, circuit_C1, circuit_C2 and circuit_C3
//            are all library reserved macros and may be used without any declaration
//
// [row][3] - digital pin number of any linked output pin to this switch
// [row][4] - If a switch has a linked output pin, this is the value for the pin
//            to be initialised with (HIGH or LOW) when linked via the function
//            link_switch_to_output.
//            NOTE:
//            1. this parameter must be the SAME for ALL switches
//               linked to the SAME common interrupt pin
//            2. if the interrupt is triggered on
//               a. RISING, then this value must be set LOW
//               b. FALLING, then this value must be set HIGH
//               c. CHANGE, then this parameter can be either LOW or HIGH

volatile uint8_t My_Switches[max_switches][5] {
  button_switch, 23, INPUT_PULLDOWN, common_interrupt_pin, HIGH,//switch 0
  toggle_switch, 22, INPUT_PULLUP,   common_interrupt_pin, HIGH,//switch 1
  button_switch, 25, INPUT_PULLUP,                      0,    0,//switch 2, no linked pin for switch
  toggle_switch, 26, INPUT_PULLDOWN, common_interrupt_pin, HIGH //switch 3
};

volatile const uint8_t interrupt_trigger_type = CHANGE;   // can be RISING, FALLING or CHANGE

volatile uint16_t ISR_count = 0;    // used to show the ISR is being entered

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
    attachInterrupt(digitalPinToInterrupt(common_interrupt_pin),
                    switch_ISR,// name of the sketch's ISR handler for switch interrupts
                    interrupt_trigger_type);// how the interrupt will be triggered
  }
  my_switches.set_debounce(50); // set debounce to 50 millisecs
  pinMode(LED, OUTPUT); // internal built in LED
  Serial.begin(115200);
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
        if (My_Switches[sw][0] == button_switch)Serial.print("button switch)");
        else Serial.print("toggle switch)");
        Serial.print(" actuated on pin ");
        Serial.print(My_Switches[sw][1]);        // the pin this sw(itch) is connected to
        uint8_t linked_pin = My_Switches[sw][3]; // the pin this sw(itch) is linked to
        if (linked_pin != 0) {
          // this switch has a linked output hthat triggered its ISR
          // so display the ISR count
          Serial.print(", ISR linked pin is ");
          Serial.print(linked_pin);
          Serial.print(", ISR counter = ");
          Serial.println(ISR_count);
        } else Serial.println();
        Serial.flush();
        //...
      }
      //...
    }
    //...
  } while (true);
}
//
// A common ISR associated with defined switches with linked outputs (standard
// feature of the ez_switch_ib library).
// Note that this ISR is a common framework that can be used for all uses
// of the ez_switch_lib library where switches are linked to output(s) that are
// configured as external interrupts.  The ISR indicates where end user code
// should be inserted, depending on the switch type triggering the ISR.
//
void switch_ISR() {
  ISR_count++; //increment ISR counter, for demo only
  uint8_t switch_id = my_switches.last_switched_id; // switch id of switch currently switched
  if (my_switches.switches[switch_id].switch_type == button_switch) {
    // this was a button switch triggering the interrupt
    // *** Button switch processing
    // *** Put end user code here to deal with this event
    //...
  } else {
    // this was a toggle switch triggering the interrupt
    // determine if the switch transitioned to 'on' or 'off'
    // and process accordingly
    if (my_switches.switches[switch_id].switch_status == on) {
      // *** Toggle switch processing for switch being 'on'
      // *** Put end user code here to deal with this event
      //...
    } else {
      // *** Toggle switch processing for switch being 'off'
      // *** Put end user code here to deal with this event
      //...
    }
  }
  // Finish up, with housekeeping tasks...
  if (interrupt_trigger_type != CHANGE) {
    // not CHANGE, so we need to reset the interrupt pin
    bool level;
    // Deal with the switch control structure data first:
    //   reset interrupt pin (linked output pin) status back to previous value
    //   (ie invert it), ready for next read if interrupt trigger type is
    //   RISING or FALLING. If it is CHANGE, then we leave it as is.
    if (interrupt_trigger_type == RISING)level = LOW; else level = HIGH;
    my_switches.switches[switch_id].switch_out_pin_status = level;

    // Now deal with the physical interrupt pin (linked output pin) level:
    //   if the attachInterrupt function trigger parameter is:
    //     "RISING"  then the common interrupt pin must be reset to LOW
    //     "FALLING" then the common interrupt pin must be reset to HIGH
    //     "CHANGE"  then we DO NOT reset the common interrupt pin
    digitalWrite(my_switches.switches[switch_id].switch_out_pin, level);
  } else {
    // trigger type is CHANGE, which the code here is a bit more involved!
    uint8_t ISR_pin = My_Switches[switch_id][3]; // pin defined for this interrupt
    bool level = my_switches.switches[switch_id].switch_out_pin_status;
    for (uint8_t sw = 0; sw < max_switches; sw++) {
      if (my_switches.switches[sw].switch_out_pin != 0) {
        // linked output pin associated with this switch, but only
        // perform update if it is the same pin number as the one
        // that triggered this interrupt
        if (my_switches.switches[sw].switch_out_pin == ISR_pin) {
          // make this switch (sw) same level as the actual switch
          // that triggered the interrupt
          my_switches.switches[sw].switch_out_pin_status = level;
        }
      }
    }
  }
} // End of switch_ISR
