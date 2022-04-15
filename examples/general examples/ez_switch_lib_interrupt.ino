// Ron Bentley, Stafford UK
// March 2021
//
// This example and code is in the public domain and may be used without restriction and
// without warranty.
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//           Example sketch - Multiple switches handled by a single interrupt
//           '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
// This sketch demonstrates how the ez_switch_lib may be used to handle multiple
// switches (button & toggle switches in this example) with a single interrupt routine.
//
// The use of the ez_switch_lib library for switches provides:
//   * switch type independence
//   * switch circuit type independence
//   * automatic multiple switch debounce handling
//   * parallel switching capabilities, and
//   * automatic interrupt handling for all switches
//
// The sketch is designed such that when a toggle switch is switched to the 'on'
// position, or a button switch is pressed AND released a linked output connected
// to a common interrupt pin will cause the associated interrupt handler to be
// fired to process the switch 'on' event.
// NB, and to recap:
//   1. a toggle switch will fire the interrupt when set to 'on'.  Setting it back
//      off does not fire the interrupt
//   2. a button switch will ONLY fire the interrupt when pressed 'on' AND then released
//      to off.  The interrupt fires on completion of the button switch cycle.
//
// Note that:
//   1. error checking on switch set ups has been removed post development.
//   2. the serial monitor is used to confirm the correct operation of the sketch.
//
// The sketch will use digital pin 2 as the common interrupt pin and
// pins 3, 4, 5 and 6 as the switch pins.
//
// For an understanding of the capabilities of the 'ez_switch_lib' library see the USER GUIDE:
// https://github.com/ronbentley1/ez_switch_lib-Arduino-Library/blob/main/ez_switch_lib_user_guide%2C%20v1.03.pdf
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include <ez_switch_lib.h>

int     interrupt_pin =  2;  // external interrupt pin

#define num_switches     4
//
// 'my_switches' layout.
// one row of data for each switch to be configured, as follows:
// [][0] = switch type
// [][1] = digital pin connected to switch
// [][2] = the switch_id provided by the add_switch function for the switch declared
// [][3] = the circuit type connecting the switch, here the switches
//         will have 10k ohm pull down resistors wired
byte my_switches[num_switches][4] =
{
  button_switch,  3, 0, circuit_C1,
  button_switch,  4, 0, circuit_C1,
  toggle_switch,  5, 0, circuit_C1,
  toggle_switch,  6, 0, circuit_C1
};

// Create the 'Switches' instance (ms) for the given number of switches
Switches ms(num_switches); 

void setup() {
  // Add all switches to library switch control structure
  // and link all to same interrupt pin as a linked output
  for (byte sw = 0; sw < num_switches; sw++) {
    my_switches[sw][2] = ms.add_switch(
                           my_switches[sw][0],  // switch type
                           my_switches[sw][1],  // digital pin switch is wired to
                           my_switches[sw][3]); // type of circuit switch is wired as
    ms.link_switch_to_output(
      my_switches[sw][2], // switch id
      interrupt_pin,      // digital pin to link to for interrupt
      LOW);               // start with interrupt pin LOW, as interrupt will be triggered on RISING
  }
  // Now establish the common interrupt service routine (ISR) that
  // will be used for all declared switches
  attachInterrupt(
    digitalPinToInterrupt(interrupt_pin),
    switch_ISR, // name of the sketch's ISR function to handle switch interrupts
    RISING);    // trigger on a rising pin value
  Serial.begin(115200);
} // end of setup function

void loop() {
  // Keep testing switch, and let the interrupt handler do its thing
  // once a switch is switched to 'on'
  for (byte sw = 0; sw < num_switches; sw++) {
    ms.read_switch(my_switches[sw][2]); // my_switches[sw][2] is the switch id for switch sw
  }
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// ISR for handling interrupt triggers arising from associated switches
// when they transition to on.  The routine knows which switch has generated
// the interrupt because the ez_switch_lib switch read functions record the
// actuated switch in the library variable 'last_switched_id'.
//
// The routine does nothing more than demonstrate the effectiveness of the 
// use of a single ISR handling multiple switches by using the serial monitor
// to confirm correct operation.
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void switch_ISR()
{
  // Reset the interrupt pin to LOW, so that any other switch will fire the interrupt
  // whist one or more switches in transition stage
  byte switch_id = ms.last_switched_id;  // switch id of switch currently switched to on
  digitalWrite(ms.switches[switch_id].switch_out_pin, LOW);
  // For button switches only, reset the linked output pin status to LOW so that
  // it will trigger the interrupt at every press/release cycle.
  if (ms.switches[switch_id].switch_type == button_switch) {
    ms.switches[switch_id].switch_out_pin_status = LOW;
  }
  Serial.print("** Interrupt triggered for switch id ");
  Serial.println(switch_id); // 'this_switch_id' is the id of the triggering switch
  Serial.flush();
} // end of switch_ISR
