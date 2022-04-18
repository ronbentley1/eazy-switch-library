# eazy-switch-library, ez_switch_lib, for Arduino and ESP 32 Microcontrollers

Basic, easy (ez) to use but flexible switch library for Arduino and ESP 32 microcontrollers, supporting multiple switchtypes and circuit wiring schemes.

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
- mixing of different switch wiring schemes for both Arduino and ESP 32 boards - switches may be configured as pinMode(..,INPUT/circuit_C1) requiring an external 10k ohm resistor or pinMode(..,INPUT_PULLUP/circuit_21) requiring NO external resistor; for ESP 32 only, pinMode(..,INPUT_PULLDOWN/circuit_C3) requiring NO external resistor

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
- switch control status reporting via serial monitor
- reserved library macro definitions for use by end user, supporting self documenting sketch code
- a comprehensive User Guide, Crib Sheet and Quick Strart Guide.

The User Guide provides a comprehensive exposition of the scope and capabilities of the <ez_switch_lib> library, including working example sketches. For a full appreciation of the <ez_switch_lib> library capabilities download the User Guide from github.

To see the Arduino Project Hub article follow this link.=: 
https://create.arduino.cc/projecthub/ronbentley1/a-switch-library-for-arduino-esp-32-microcontrollers-dfbe40?ref=user&ref_id=1455180&offset=19

<end>
