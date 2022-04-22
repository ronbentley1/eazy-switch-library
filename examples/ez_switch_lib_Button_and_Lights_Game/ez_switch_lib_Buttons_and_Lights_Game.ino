/*
   Ron D Bentley, Stafford, UK
   Feb 2021

   Buttons & Lights Game, using the <ez_switch_lib> library and using button switches
   as pseudo toggle switches, as we want the click capabilty of a button switch
   and want to flash a linked LED on ant then off after actuation complete.
   ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

   This example and code is in the public domain and may be used without restriction and
   without warranty.

   Basic game of Buttons & Lights
   Game flow:
   - game starts by blinking all 4 leds 4 times
   - a sequence of 4, 5 or 6 leds are then illuminated, in turn, at the end
     of which a timer then starts (user variable) within which the sequence must
     be entered on the button switches, each button switch one coresponding to an
     associated led
   - if correct sequence is entered then the green led will flash 5 times
   - if the incorrect sequence is entered them the red led will flash 5 times
   - if the sequence is not fully entered within the given time then the
     blue led will flash 5 times and the game reset for the next round.

   Configurability:
   1. sequence size for illuminated leds - the number of guesess is defined by
      the sequence size definition ('#define max_sequence'). Change this as
      required, but should be > 4 for a reasonable game.
   2. game time - the duration for entering the guess sequence is controlled
      by the variable 'int guess_duration'. This should be set with the time in
      milliseconds given for the guess period.
*/
#include <ez_switch_lib.h>

#define num_switches 4 // number of button switches connected

#define button_1   2
#define button_2   3
#define button_3   4
#define button_4   5
#define red_led    8  // doubles as failed guess indicator
#define amber_led  9  // 
#define green_led 10  // doubles as successful guess indicator
#define blue_led  11  // doubles as timed out indicator, before guess fully made

// layout:
//   [][0] = digital pin assigned to 1st switch
//   [][1] = digital pin to be linked to the switch (LED)
//   [][2] = switch id returned from add_switch function
int pseudo_toggles[num_switches][4] = {
  button_1, red_led,    0, circuit_C1,
  button_2, amber_led,  0, circuit_C1,
  button_3, green_led,  0, circuit_C1,
  button_4, blue_led,   0, circuit_C2
};

#define max_sequence                 6   // must be >= 4 for a reasonable game, change if more wanted
int sequence[max_sequence + 1];          // records sequence to guess.[0] will hold number of entries in array
#define success                   true   // used for verifying guesses sequence

#define number_of_guesses max_sequence
int guesses[number_of_guesses + 1];      // records player's guesses. [0] will hold number of entries in array

long unsigned int start_time =      0;   // used to time start of a game
int guess_duration           =  10000;   // duration time given to a play game, in millisecs

Switches my_switches(num_switches);      // create Switches class

void setup() {
  int sw;
  // initialise digital input switch pins and their associated LEDs
  for (sw = 0; sw < num_switches; sw++) {
    // add switches to the switch library
    pseudo_toggles[sw][2] = my_switches.add_switch(
                              toggle_switch,         // switch type
                              pseudo_toggles[sw][0], // switch digital pin
                              pseudo_toggles[sw][3]);// switch circuit type
    // link switches to outputs
    my_switches.link_switch_to_output(
      pseudo_toggles[sw][2], // switch id to link output to
      pseudo_toggles[sw][1], // output pin to be linked
      LOW);                  // initial output pin setting
  }
  randomSeed(analogRead(A0 + A1));
  reset_game();  // set up first game
}

void loop() {
  if (millis() - start_time > guess_duration) {
    // play duration has timed out
    flash_led(blue_led, 5);  // play is out of time
    delay(2000);
    reset_game();
  } else {
    if (guesses[0] < sequence[0]) {
      // poll switches for a selection
      for (int sw = 0; sw < num_switches; sw++) {
        byte sw_id  = pseudo_toggles[sw][2];  // switch id given by add_switch
        bool sw_status = my_switches.read_switch(sw_id);
        if (sw_status == switched &&
            my_switches.switches[sw_id].switch_status == !on) {
          // this switch was pressed on and now switched to off and the 
          // linked output will have been set to LOW (ie LED is off), 
          // so record it - add to guess list
          guesses[0]++;
          guesses[guesses[0]] = sw;  // record this switch's index
        }
      }
    } else {
      //  all guesses made within the play time - check if correct
      if (check_sequence() == success) {
        // guessed correct
        flash_led(green_led, 5);  // flash success
      } else {
        // guess incorrect
        flash_led(red_led, 5);    // flash failure
      }
      delay(2000);
      reset_game();
    }
  }
}

//
// game reset, clear down and generate a new sequence
//
void reset_game() {
  int i, num_in_sequence;
  guesses[0] = 0;  // clear down previous guesses
  // set up a new sequence for next game
  num_in_sequence = random(4, max_sequence + 1); // pick new sequence length, at least 4 in sequence
  for (i = 1; i <= num_in_sequence; i++) {
    sequence[i] = random(0, num_switches);       // index to switch pin/led array
  }
  sequence[0] = num_in_sequence;
  flash_all_leds(4);
  delay(2000);
  play_sequence();
  start_time = millis();  // set guess timer start time
}

//
// flash leds in the order defined by the sequence list
//
void play_sequence() {
  int i;
  for (i = 1; i <= sequence[0]; i++) {
    digitalWrite(pseudo_toggles[sequence[i]][1], HIGH);
    delay(1000);
    digitalWrite(pseudo_toggles[sequence[i]][1], LOW);
    delay(1000);
  }
}

//
// check off guessed button presses against the sequence
//
bool check_sequence() {
  int i;
  for (i = 1; i <= guesses[0]; i++) {
    if (guesses[i] != sequence[i]) {
      return !success;
    }
  }
  return success;
}

//
// flash given led the given number of times
//
void flash_led(int led, int num) {
  for (int i = 1; i <= num; i++) {
    digitalWrite(led, HIGH);
    delay(250);
    digitalWrite(led, LOW);
    delay(250);
  }
}

//
// flash all leds the given number of times to signify game starting
//
void flash_all_leds(int n) {
  int cycle;
  bool level = LOW;
  n = 2 * n; // because 1 cycle is on then off, hence a x2 factor
  level = 0;
  for (cycle = 1; cycle <= n; cycle++) {
    level = HIGH - level;
    for (byte sw = 0; sw < num_switches; sw++) {
      digitalWrite(pseudo_toggles[sw][1], level);
    }
    delay(250);
  }
}
