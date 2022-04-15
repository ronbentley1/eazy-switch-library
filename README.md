# eazy-switch-library for Arduino and ESP 32 Microcontrollers
A repository to file the ez_switch_lib library files for the Arduino Library Manager.

# Overview
The ez_switch_lib library provides support for reading single or multiple swicthes of types button (eg momentary) or toggle.
Any nuber of switches of either or mixed types may be configured and wired in one of two/ different wiring schemes.
For Arduino boards there are:
  two wiring scheme choices - pinMode(..,INPUT) wired with an external 10k ohm pull down resistor and pinMode(..,INPUT_PULLUP) wired without any external resistor.
For ESP 32 boards the choice of switch wiring schemes is as for Arduino boards but there is also a third choice of wiring, this being:
  pinMode(..,INPUT_PULLDOWN) wired without any external resistor.
  
The ez_switch_lib library provides a software solution for switch debounce. Debounce may be set as required, otherwise it will default to 10 milliseconds.

The ez_switch_lib library provides a unique caoablity in that it is possible to link any switch to another digital output pin such that when the switch is actuated the linked out will be flipped/inverted automatically.
This means that no use code is required to 'turn on/turn off' a linked output following a switch actuation.
Such switches may also be processed normally via a 'switched' event.

The linked output concenpt may also be used to trigger external interrupts following switch actuations.

A comprehensive User Guide, Crib Sheet and Quick Start Guide are also available, along with example sketches.

