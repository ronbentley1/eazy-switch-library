// Arduino Switch Library for configuring different switch type wired
// in common circuit schemes.
//
// Ron Bentley, Stafford (UK), March 2021, version 2.0
//
// This example and code is in the public domain and
// may be used without restriction and without warranty.
//

#ifndef ez_switches_h
#define ez_switches_h

#include <Arduino.h>

class Switches
{
  public:
    Switches(byte max_switches);

#define button_switch         1      // differentiates switch type
#define toggle_switch         2      // ditto

#ifndef INPUT_PULLDOWN
#define INPUT_PULLDOWN        9      // not valid for Arduino boards, but it is for ESP32 boards
#endif

#define circuit_C1        INPUT      // switch circuit requires an external pull down 10k ohm resistor
#define circuit_C2  INPUT_PULLUP     // switch circuit requires no other components beyond the switch
#define circuit_C3  INPUT_PULLDOWN   // switch circuit requires no other components beyond the switch

#define switched           true      // signifies switch has been pressed/switch cycle complete
#define on                 true      // used for toggle switch status
#define not_used           true      // helps self document code
#define bad_params           -2      // invalid add_swith paramters
#define add_failure          -1      // add_swith could not insert a given switch, ie no space left
#define link_success          0      // output successfully linked to a switch
#define link_failure         -1      // output pin could not be linked to a switch
#define none_switched      255      // 'last_switched_id' initialised to this value

    // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    // %                   Switch Control Sruct(ure) Declaration                 %
    // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //
    struct switch_control {
      byte switch_type;                 // type of switch connected
      byte switch_pin;                  // digital input pin assigned to the switch
      byte switch_circuit_type;         // the type of circuit wired to the switch
      bool switch_on_value;             // used for BUTTON SWITCHES only - defines what "on" means
      bool switch_pending;              // records if switch in transition or not
      long unsigned int switch_db_start;// records debounce start time when associated switch starts transition
      bool switch_status;               // used for TOGGLE SWITCHES only - current state of toggle switch.
      byte switch_out_pin;              // the digital pin mapped to this switch, if any
      bool switch_out_pin_status;       // the status of the mapped pin
    } volatile *switches;               // memory will be created when class is initiated

    volatile byte last_switched_id = none_switched;

    // Functions available to end users
    bool read_switch           (byte sw);
    bool read_toggle_switch    (byte sw);
    bool read_button_switch    (byte sw);
    int  add_switch            (byte sw_type, byte sw_pin, byte circ_type);
    int  link_switch_to_output (byte switch_id, byte output_pin, bool HorL);
    int  num_free_switch_slots ();
    void set_debounce          (int period);
    void print_switch          (byte sw);
    void print_switches        ();

  private:
    byte _num_entries  = 0;  // used for adding switches to switch control structure/list
    byte _max_switches = 0;  // max switches user has initialise
    int _debounce      = 10; // 10 millisecs if not specified by user code
};

#endif