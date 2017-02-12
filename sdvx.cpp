
#include "sdvx.h"

#ifdef USE_FASTLED

CRGB crgb[LED_COUNT];
#endif

// TODO: Test poll vs interrupt for buttons
void setup()
{
  for (int i = 0; i < SWITCH_COUNT; i++) {
    pinMode(switches[i].switchPin, INPUT_PULLUP);
  }
  for (int i = 0; i < LED_COUNT; i++) {
    pinMode(leds[i].pin, OUTPUT);
  }
  Serial.begin(57600);
  Arcade.useManualSend(true); // lets reduce the calls
#ifdef USE_FASTLED
  //FIXME: Why does this error?
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(crgb, LED_COUNT);
#endif
}

uint32_t buffer[1];
uint8_t lastSentState[3]; // the state of the last sent buttons

void loop() {
  Arcade.recv(buffer, 0); // 0 timeout = do not wait

  // Read and set all the bytes for the buttons
  for (int i = 0; i < SWITCH_COUNT; i++) {

    if (digitalRead(switches[i].switchPin)) {
      Arcade.button(switches[i].joyId, 0);
    } else {
      Arcade.button(switches[i].joyId, 1);
    }
    // Hack instant write
    // TODO: check if bit-tweaking is more efficient
    // it may compile to the same stuff
    // usb_arcade_data[0] ^= (digitalRead(switches[i].switchPin) ^ usb_arcade_data[0]) & (1 << i);
  }

  for (int i = 0; i < ENCODER_COUNT; i++) {
    long encVal = encoders[i].enc.read();
    if (encVal != 0) {
      if (encoders[i].axis == 'x') {
        Arcade.X((unsigned int) encVal);
      } else {
        Arcade.Y((unsigned int) encVal);
      }
      encoders[i].enc.write(0);
    }
  }

  for (int i = 0; i < LED_COUNT; i++) {
    // we have LEDs and a buffer
    // set the buffer pos to the led pin
    digitalWrite(leds[i].pin, (buffer[0] >> i) & 1);
  }
  if ( memcmp(usb_arcade_data, lastSentState, sizeof(usb_arcade_data)) != 0 ) {
    // the button state has changed, send and reset it.
    Arcade.send_now();
    memcpy(lastSentState, usb_arcade_data, sizeof(lastSentState));
#ifdef DEBUG
    Serial.print("New State: ");
    Serial.printf("%02x", usb_arcade_data[0]);
    Serial.printf("%02x", usb_arcade_data[1]);
    Serial.printf("%02x", usb_arcade_data[2]);
    Serial.println();
#endif
  }
  delay(DELAY);
}