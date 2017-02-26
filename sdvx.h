//
// Created by kschamplin19 on 2/5/17.
//
#include <Arduino.h>

#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#ifndef SDVX_ASC_SDVX_H
#define SDVX_ASC_SDVX_H
#define DEBUG
#define DELAY     1  // Delay per loop in ms

// #define DEBUG

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
  uint8_t joyId;
} switch_t;

#define SWITCH_COUNT 8

switch_t switches[SWITCH_COUNT] = {
    {4, 1},   // A
    {5, 2},   // B
    {6, 3},   // C
    {7, 4},   // D
    {8, 5},   // FX A
    {9, 6},   // FX B
    {10, 7},  // START
    {11, 8},  // TEST (no led)
};

#ifdef USE_FASTLED
#include <FastLED.h>
#define LED_COUNT 10 // Number of LEDs to add
#define DATA_PIN
#define BT_COLOR 190 // the HUE for the BT buttons
#define FX_COLOR 20 // same for FX
#else
#define LED_COUNT 8 // Number of LEDs to add

typedef struct {
  uint8_t pin;
} led_t;

// some notes about LED lights
// for fast LED we need to use light ID instead of Pin.
led_t LEDs[LED_COUNT] = {
    {13},
    {14},
    {15},
    {16},
    {17},
    {18},
    {19},
    {20},
};
#endif



#endif //SDVX_ASC_SDVX_H
