
#include "sdvx.h"

#ifdef USE_FASTLED

CRGB crgb[LED_COUNT];
#endif

// TODO: Test poll vs interrupt for buttons
void setup()
{
  for (int i = 0; i < SWITCH_COUNT; i++) {
    pinMode(switches[i].switchPin, INPUT_PULLUP);
    pinMode(switches[i].lightPin, OUTPUT);
  }

  Serial.begin(57600);
  Arcade.useManualSend(true); // lets reduce the calls
#ifdef USE_FASTLED
  //FIXME: Why does this error?
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(crgb, LED_COUNT);
#endif
}

uint8_t buffer[2];
uint8_t lastSentState[3]; // the state of the last sent buttons

void loop() {
  int n = Arcade.recv(buffer, 0); // 0 timeout = do not wait
  // TODO: Actually get receiving working.
  if (n > 0) {
#ifdef DEBUG
    Serial.print("Received: ");
    Serial.print(n);
    Serial.print((uint32_t) buffer);
#endif
  }

  // Read and set all the bytes for the buttons
  for (int i = 0; i < SWITCH_COUNT; i++) {

    if (digitalRead(switches[i].switchPin)) {
      Arcade.button(switches[i].joyId, 0);
    } else {
      Arcade.button(switches[i].joyId, 1);
    }
    // Hacky instant write
    // TODO: check if bit-tweaking is more efficient
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
  for (int i = 0; i < LED_COUNT; i++) {

#ifdef USE_FASTLED
    // FASTLED mode!
#endif

  }
  delay(DELAY);
}