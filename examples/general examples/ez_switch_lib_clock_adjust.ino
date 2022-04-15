/*   Ron D Bentley, Stafford, UK
   Mar 2021

   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   -            Example of use of the switch_lib library                -
   Example 5 - time adjustment sketch
   Example of use of button switches masquerading as toggle switches so
   that we can use the button's as switches to adjust the hours/minutes
   of an external clock/display.  Buttons can be pressed incrementally
   for advancing by a single hour/minute or kept pressed for autoadvancement.
   The adjustment buttons will not work unless the adjust switch is on.
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

   This example and code is in the public domain and
   may be used without restriction and without warranty.
*/

#include <ez_switch_lib.h>

#define adjust_switch          2  // toggle switch input pin
#define hour_adjust_switch     3  // a physical button switch input pin, masquerading as a toggle switch
#define min_adjust_switch      4  // physical button switch input pin, masquerading as a toggle switch
#define adjust_led             8  // LED illuminated when adjust switch on

#define debounce_period       10  // debounce period for general switch reading
#define sensitivity           250 // used to provide a short delay between switch reading during adjustments

int hour_id, min_id, adjust_id;   // used to record switch ids when declared to ez_switch_lib

int hour      = 0; // initial hour setting
int min       = 0; // initial minute setting
int now_time  = 0; // to decide if there has been a time adjustment change
int prev_time = 0; // ditto

Switches my_switches(3); // only 3 switches to be declared

void setup() {
  Serial.begin(9600);    // we will use the serial monitor to demonstrate adjustment process
  
  // declare the switches we wish to use
  adjust_id = my_switches.add_switch(toggle_switch, adjust_switch, circuit_C1);
  // link adjust switch to LED
  my_switches.link_switch_to_output(adjust_id, adjust_led, LOW);
  hour_id    = my_switches.add_switch(toggle_switch, hour_adjust_switch,circuit_C1);
  min_id     = my_switches.add_switch(toggle_switch, min_adjust_switch, circuit_C1);
  
  // set standard debounce period to required value for this sketch
  my_switches.set_debounce(debounce_period);
}

void loop() {
  do {
    my_switches.read_switch(adjust_id);  // read/test adjust switch being on/off
    if (my_switches.switches[adjust_id].switch_status == on) {
      // adjust switch is on, so process any button hour/minute changes
      adjust_time();
    }
  } while (true);
}

void adjust_time() {
  my_switches.set_debounce(0); // reduced debounce to 0 for this application
  do {
    // while the time adjust switch is set, adjust time according to hour/min switches
    my_switches.read_switch(hour_id);
    my_switches.read_switch(min_id);
    if (my_switches.switches[hour_id].switch_status == on ||
        my_switches.switches[min_id].switch_status == on)
    {
      if (my_switches.switches[hour_id].switch_status == on) {
        hour = (hour + 1) % 24;
      }
      if (my_switches.switches[min_id].switch_status == on) {
        min = (min + 1) % 60;
      }
      now_time = hour * 100 + min;
      if (now_time != prev_time) {
        // Update any external display here with hours/mins
        // In the absence of an external display, we use the
        // serial monitor to show te adjustments
        if (hour < 10) {
          Serial.print("0");
        }
        Serial.print(hour);
        Serial.print(":");
        if (min < 10) {
          Serial.print("0");
        }
        Serial.println(min);
        prev_time = now_time;
        delay(sensitivity);
      }
    }
    my_switches.read_switch(adjust_id);// check adjust switch still on
  } while (my_switches.switches[adjust_id].switch_status == on ); // keep goig until deselected
  my_switches.set_debounce(debounce_period);  // reset debounce period
}
