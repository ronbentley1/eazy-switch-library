// Arduino Switch Library for configuring different switch type wired
// in common circuit schemes.
//
// Ron Bentley, Stafford (UK), March 2021, 
// updates:
//   Sept 2022, version 3.00
//     addition of functions 'is_button_pressed' (overloaded) and
//                           'reset_switch' and 'reset_switches'
//   Sept 2022, version 3.01
//     change of library variables to unsigned declarations, generally,
//     eg byte to uint8_t, long unsigned int to uint32_t, etc
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
    Switches(uint8_t max_switches);

#define button_switch         1      // differentiates switch type
#define toggle_switch         2      // ditto

#ifndef INPUT_PULLDOWN
#define INPUT_PULLDOWN    INPUT      // default to INPUT type if not already defined
#endif

#define circuit_C1        INPUT      // switch circuit requires an external pull down 10k ohm resistor
#define circuit_C2  INPUT_PULLUP     // switch circuit requires no other components beyond the switch
#define circuit_C3  INPUT_PULLDOWN   // switch circuit requires no other components beyond the switch, only ESP 32

#define switched           true      // signifies switch has been pressed/switch cycle complete
#define on                 true      // used for toggle switch status
#define not_used           true      // helps self document code
#define bad_params           -2      // invalid add_swith paramters
#define add_failure          -1      // add_swith could not insert a given switch, ie no space left
#define link_success          0      // output successfully linked to a switch
#define link_failure         -1      // output pin could not be linked to a switch
#define none_switched       255      // 'last_switched_id' initialised to this value

    // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    // %                   Switch Control Sruct(ure) Declaration                 %
    // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //
    struct switch_control {
      uint8_t switch_type;         // type of switch connected
      uint8_t switch_pin;          // digital input pin assigned to the switch
      uint8_t switch_circuit_type; // the type of circuit wired to the switch
      bool switch_on_value;        // used for BUTTON SWITCHES only - defines what "on" means
      bool switch_pending;         // records if switch in transition or not
      uint32_t switch_db_start;    // records debounce start time when associated switch starts transition
      bool switch_status;          // used for TOGGLE SWITCHES only - current state of toggle switch.
      uint8_t switch_out_pin;      // the digital pin mapped to this switch, if any
      bool switch_out_pin_status;  // the status of the mapped pin
    } volatile *switches;          // memory will be created when class is instantiated

    volatile uint8_t last_switched_id = none_switched;

    // Functions available to end users
    bool read_switch           (uint8_t switch_id);
    bool read_toggle_switch    (uint8_t switch_id);
    bool read_button_switch    (uint8_t switch_id);
    int  add_switch            (uint8_t sw_type, uint8_t sw_pin, uint8_t circ_type);
    int  link_switch_to_output (uint8_t switch_id, uint8_t output_pin, bool HorL);
    int  num_free_switch_slots ();
    void set_debounce          (uint16_t period);
    void reset_switch          (uint8_t switch_id);
    void reset_switches        ();
    bool button_is_pressed     (uint8_t switch_id, bool process_link);
    bool button_is_pressed     (uint8_t switch_id);
    void print_switch          (uint8_t switch_id);
    void print_switches        ();

  private:
    uint8_t _num_entries  = 0;  // used for adding switches to switch control structure/list
    uint8_t _max_switches = 0;  // max switches user has initialise
    int _debounce         = 10; // 10 millisecs if not specified by user code
};

#endif
