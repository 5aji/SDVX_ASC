# SDVX ASC

A Teensy-powered firmware for a custom SDVX controller.
Features include a HID lighting system for use with games that support it,
reactive lighting that can be enabled, and optimized USB dataflow for faster
response time. Can use any RGB LEDs that support the FastLed library, or
single-color LEDs with direct write.

#### Libaries

This program uses a modified set of the Teensy Core Libraries for 3.2. See the teensy lib
repo.

## Options

There are a variety of options that can be manipulated both in the code
and by using the controller itself.

#### Compile-time Options

By defining the `DEBUG` macro in [sdvx.h](sdvx.h) the Teensy will send
information over the emulated Serial. Use the Teensy toolchain to read the
output.

All mappings are editable in [sdvx.h](sdvx.h). Changing the mappings
according to the structure will edit all references.

Defining `USE_FASTLED` will enable the system to write the LED output
to FastLED supported addressable LEDs. This allows you to define custom colors
as well as use RGB output over HID where supported. (Note that this will
require some modification of the HID report descriptors as well as setup
of the FastLED library)