# eazy-switch-library for Arduino and ESP 32 Microcontrollers
A repository to file the ez_switch_lib library files for the Arduino Library Manager.


# Overview
The ez_switch_lib library provides support for reading single or multiple swicthes of types button (eg momentary) or toggle.
Any nuber of switches of either or mixed types may be configured and wired in one of two/three different wiring schemes.

For both Arduino and ESP 32 boards switches may be configured as:
pinMode(..,INPUT/circuit_C1) requiring an external 10k ohm resistor,
pinMode(..,INPUT_PULLUP/circuit_21) requiring NO external resistor,

and for ESP 32 only:
pinMode(..,INPUT_PULLDOWN/circuit_C3) requiring NO external resistor.


The ez_switch_lib library provides a software solution for switch debounce. Debounce may be set as required, otherwise it will default to 10 milliseconds.

The ez_switch_lib library provides a unique caoablity in that it is possible to link any switch to another digital output pin such that when the switch is actuated the linked out will be flipped/inverted automatically.
This means that no use code is required to 'turn on/turn off' a linked output following a switch actuation.
Such switches may also be processed normally via a 'switched' event.

The linked output concenpt may also be used to trigger external interrupts following switch actuations. 

A comprehensive User Guide, Crib Sheet and Quick Start Guide are also available, along with example sketches.

