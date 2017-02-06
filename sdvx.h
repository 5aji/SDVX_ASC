//
// Created by kschamplin19 on 2/5/17.
//

#ifndef SDVX_ASC_SDVX_H
#define SDVX_ASC_SDVX_H

#define DELAY     1  // Delay per loop in ms
#define DEBUG // enable serial out

#ifdef USE_FASTLED
#include "FastLED.h"
#define LED_COUNT 10 // Number of LEDs to add
#define BT_COLOR 190 // the HUE for the BT buttons
#define FX_COLOR 20 // same for FX
#else

#endif
typedef struct {
  Encoder enc;
  char axis;
} encoder_t;

#define ENCODER_COUNT 2

encoder_t encoders[ENCODER_COUNT] = {
    {Encoder(1, 0), 'x'},
    {Encoder(2, 3), 'y'}
};

typedef struct {
  uint8_t switchPin;
  uint8_t lightPin;
  uint8_t joyId;
} switch_t;

#define SWITCH_COUNT 8


// some notes about LED lights
// for fast LED we need to use light ID instead of Pin.
switch_t switches[SWITCH_COUNT] = {
    {4, 12, 1},   // A
    {5, 14, 2},   // B
    {6, 15, 3},   // C
    {7, 16, 4},   // D
    {8, 17, 5},   // FX A
    {9, 18, 6},   // FX B
    {10, 13, 7},  // START
    {11, 19, 8},  // TEST (no led)
};

#ifdef DEBUG
#define BREAK(x) Serial.print("Break Called at: "); \
  Serial.print(__FILE__); \
  Serial.print(' '); \
  Serial.print(__LINE__); \
  Serial.print(' '); \
  Serial.println(__FUNCTION__); \
  while(!Serial.available()) {}
#endif

#endif //SDVX_ASC_SDVX_H
