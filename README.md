# eazy-switch-library for Arduino and ESP 32 Microcontrollers
A repository to file the ez_switch_lib library files for the Arduino Library Manager.


# Overview
The ez_switch_lib library provides support for reading single or multiple swicthes of types button (eg momentary) or toggle.
Any nuber of switches of either or mixed types may be configured and wired in one of two/three different wiring schemes.

For both Arduino and ESP 32 boards switches may be configured as:

* pinMode(..,INPUT/circuit_C1) requiring an external 10k ohm resistor,

* pinMode(..,INPUT_PULLUP/circuit_21) requiring NO external resistor,

and for ESP 32 only:

* pinMode(..,INPUT_PULLDOWN/circuit_C3) requiring NO external resistor.


The ez_switch_lib library provides a software solution for switch debounce. Debounce may be set as required, otherwise it will default to 10 milliseconds.

The ez_switch_lib library provides a unique capoablity in that it is possible to link any switch to another digital output pin such that when the switch is actuated the linked out will be flipped/inverted automatically.
This means that no user code is required to 'turn on/turn off' a linked output following a switch actuation.
Such switches may also be processed normally via a 'switched' event.

The linked output concept may also be used to trigger external interrupts following switch actuations. 

The library is also capable of providing other features, for example, testing if a switch is in transition from one state to another, etc.

A comprehensive User Guide, Crib Sheet and Quick Start Guide are also available, along with example sketches.

# Arduino/ESP 32 ez_switch_lib
Basic, easy (ez) to use but flexible switch library for Arduino and ESP 32 microcontrollers, supporting multiple switch and circuit schemes of different types.

Read the User Guide for a comprehensive appreciation and understanding.

**Setting the Scene**

Switches can be troublesome to the uninitiated, and to those well used to incorporating them into their projects. It would also appear to be the same in the non-Arduino world...

_"...When you mix with the wrong energy, there's bound to be an explosion. Pay attention and switch lanes when the signal changes. What's really real, is ultimately revealed..."_ T.F. Hodge

_"...Nothing is wrong with darkness provided you control the switch..."_ Aniekee Tochukwu Ezekiel

Such a simple device, but with such a myriad of styles, types and characteristics. But if you are happy and content with dealing with switches of all types then perhaps this article is not for you, keep on doing what you are comfortable doing. However, if switches are new to you, you struggle with them or want a different set of tools and methods for incorporating them into your projects then read on - the <ez_switch_lib> library offers support.

**What Does the <ez_switch_lib> Library Offer?**

In a nutshell, <ez_switch_lib> provides a set of enhanced capabilities for reading switches of different types and different connection schemes, and removes from the end user developer issues regarding switching 'noise'. Indeed, <ez_switch_lib> gives the end user developer a choice in the way a switch can be connected, supporting both of the most common wiring schemes without additional components beyond wires and, if wished, 10k ohm pull down resistors. The <ez_switch_lib> library provides a software approach to switch management and control.

**Features**

The following features are provided by the <ez_switch_lib> library:

- dynamic memory allocation, depending on the number of switches you wish to incorporate in your project
- multi-switch type capabilities
- mixing of different switch types:

- For both Arduino and ESP 32 boards switches may be configured as:

- pinMode(..,INPUT/circuit_C1) requiring an external 10k ohm resistor,

-pinMode(..,INPUT_PULLUP/circuit_21) requiring NO external resistor,

- and for ESP 32 only - pinMode(..,INPUT_PULLDOWN/circuit_C3) requiring NO external resistor.

- dual switch circuit wiring scheme support, transparent to the software developer
- support for both button and toggle style switches
- easy switch setup, with or without switch output linking
- ability to link a digital output pin to any switch for automatic output pin switching without end user coding
- configurable and automatic debounce of switching circuits
- generic switch read function (switch type agnostic)
- specific button switch read function
- specific toggle switch read function
- error trapping from read and linking functions
- direct access to all switch control variables
- support for **multiple switches linked to a single interrupt service routine (ISR)**, with switch type
  and circuit wiring scheme independence, plus full debounce handling of all switches 
  (see Corollary section of User Guide for working example)
- switch control status reporting via serial monitor
- reserved library macro definitions for use by end user, supporting self documenting sketch code
- a comprehensive User Guide

The User Guide provides a comprehensive exposition of the scope and capabilities of the <ez_switch_lib> library, including working example sketches. For a full appreciation of the <ez_switch_lib> library capabilities download the User Guide from github.

<end>
