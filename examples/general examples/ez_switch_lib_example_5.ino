/*
   Ron D Bentley, Stafford, UK
   Mar 2021

   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   -            Example of use of the switch_lib library                -
   Example 5 - time adjustment sketch
   This example shows the use of button and toggle switches to
   adjust an external time display.

   To implement this successfully we need to use button switches
   to adjust hours and minutes individually.  We implement this as
   follows:

   Toggle switch - when on, this will activate the time
   adjustment process bringing the two button switches active.
   When active,
      1 button switch will advance the hours
      1 button switch will advance minutes.

   In this implementation, we wish to advance the times as follows:
      a. if a button is pressed and released immediately, then
         the hour/minute will be advanced by 1, respectively, for
         each press/release
      b. if a button is pressed and kept pressed, then the hour/minute
         will be continually advanced, respectively, automatically
         until it is released.

   To accomplish a)and b) we use the standard read_switch function
   for the button switches, but we examine their transition
   status, as we are not interested in a return of 'switched' or not.
   This is an internal flag that is set to true whem a button
   switch is pressed until the time it is released.  The specific
   switch flag is 'switches[switch_id].switch_pending'.
   This is true when in transition (pending), false otherwise.
   By using this flag we are able to utilise the button switches
   in either single 'shot mode' or continuous mode for time advancement.

   This is an additional feature of the capabilities of the
   switch_lib library, and is one that can be used in  many
   similar applications.

   As a final feature, the design links the timer adjust toggle switch
   to a LED such that the LED is illuminated when the timer adjust
   mode is active, ie the toggle switch actuated. This linking is
   configured using the switch_lib function 'link_switch_to_output'.
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

   This example and code is in the public domain and
   may be used without restriction and without warranty.

*/
#include <Arduino.h>
#include <ez_switch_lib.h>

#define adjust_led             8  // LED illuminated when adjust switch on
#define hour_adjust_switch     3  // a physical button switch, masquerading as a toggle switch
#define min_adjust_switch      4  // physical button switch, masquerading as a toggle switch
#define adjust_switch          2  // actual toggle switch

int hour_id, min_id, adjust_id; // used to record switch ids when declared to switch_lib

int hour      = 0; // initial hour setting
int min       = 0; // initial minute setting
int now_time  = 0; // to decide if there has been a time adjustment change
int prev_time = 0; // ditto

#define sensitivity 250  // msecs - used to provide a short delay between switch reading during adjustments

Switches my_switches(3); // only 3 switches to be declared

void setup() {
  Serial.begin(9600);    // we will use the serial monitor to demonstrate adjustment process
  // declare the switches we wish to use
  adjust_id = my_switches.add_switch(toggle_switch, adjust_switch, circuit_C1);
  // link adjust switch to LED for auto flipping to show switch is on/off
  my_switches.link_switch_to_output(adjust_id, adjust_led, LOW);
  hour_id    = my_switches.add_switch(button_switch, hour_adjust_switch, circuit_C1);
  min_id     = my_switches.add_switch(button_switch, min_adjust_switch,  circuit_C1);
}

void loop() {
  // keep polling the adjust switch and action if on
  do {
    my_switches.read_switch(adjust_id);  // establish switch status
    if (my_switches.switches[adjust_id].switch_status == on) {
      adjust_time();  // adjust switch is on so process any time adjustments
    }
  } while (true);
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Adjust the hours and minutes settings whilst
// the time adjust switch is on
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void adjust_time() {
  do {
    // While the time adjust switch is set,
    // adjust time according to hour/min switches
    my_switches.read_switch(hour_id);
    if (my_switches.switches[hour_id].switch_pending == true) {
      // hour switch is pressed and in transition
      hour = (hour + 1) % 24;
      now_time = hour * 60 + min;  // minutes since 00:00 hours
    }
    my_switches.read_switch(min_id);
    if (my_switches.switches[min_id].switch_pending == true) {
      // minute switch is pressed and in transition
      min = (min + 1) % 60;
      now_time = hour * 60 + min;  // minutes since 00:00 hours
    }
    if (now_time != prev_time) {
      // Either hour button or minute button, or both,
      // have been pressed, so update any external display
      // here with hours/mins.
      // In the absence of an external display, we use the
      // serial monitor to show te adjustments
      if (hour < 10) {
        Serial.print("0"); // leading 0
      }
      Serial.print(hour);
      Serial.print(":");
      if (min < 10) {
        Serial.print("0"); // leading 0
      }
      Serial.println(min);
      prev_time = now_time;
      delay(sensitivity);  // wait a short time between switch presses
    }
    my_switches.read_switch(adjust_id);       // establish current adjust switch status
  } while (my_switches.switches[adjust_id].switch_status == on ); // keep going until deselected
}
