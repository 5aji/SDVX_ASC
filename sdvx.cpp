
#include "sdvx.h"

#ifdef USE_FASTLED

CRGB crgb[LED_COUNT];
#endif
#define ENCODER_SENSITIVITY (double) 9.375

// TODO: Test poll vs interrupt for buttons
void setup()
{
  for (int i = 0; i < SWITCH_COUNT; i++) {
    pinMode(switches[i].switchPin, INPUT_PULLUP);
  }

  for (int i = 0; i < LED_COUNT; i++) {
    pinMode(LEDs[i].pin, OUTPUT);
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

  /*
   * Read and set all the bytes for the buttons
   * TODO: check if bit-tweaking is more efficient
   */
  for (int i = 0; i < SWITCH_COUNT; i++) {

    if (digitalRead(switches[i].switchPin)) {
      Arcade.button(switches[i].joyId, 0);
    } else {
      Arcade.button(switches[i].joyId, 1);
    }
    // usb_arcade_data[0] ^= (digitalRead(switches[i].switchPin) ^ usb_arcade_data[0]) & (1 << i);
  }


  /*
   * Process the encoders. may not be the most optimal method.
   */
  for (int i = 0; i < ENCODER_COUNT; i++) {
    long encVal = encoders[i].enc.read();
    if (encVal != 0) {
      if (encoders[i].axis == 'x') {
        Arcade.X((uint8_t)((int32_t)(encVal / ENCODER_SENSITIVITY) % 256));
        Serial.print("EncoderVal: ");
        Serial.print(encVal);
      } else {
        Arcade.Y((uint8_t)((int32_t)(encVal / ENCODER_SENSITIVITY) % 256));
      }
      // try encoders[i].enc.write(0);
    }
  }



  /*
   * Receive and set the LED positions
   * TODO: add FastLED support
   * FIXME: different brightness settings from input.
   * Overall this part is pretty hacked together.
   */
  Arcade.recv(buffer, 0); // 0 timeout = do not wait

  for (int i = 0; i < LED_COUNT; i++) {
    // we have LEDs and a buffer
    // set the buffer pos to the led pin
    digitalWrite(LEDs[i].pin, uint8_t ((buffer[0] >> i) & 1));

  }



  /*
   * Check for changes to the button state.
   * if there are changes, send these changes to the computer.
   */
  if ( memcmp(usb_arcade_data, lastSentState, sizeof(usb_arcade_data)) != 0 ) {
    // the button state has changed, send and reset it.
    Arcade.send_now();
    memcpy(lastSentState, usb_arcade_data, sizeof(lastSentState));
    /*
#ifdef DEBUG
    Serial.print("New State: ");
    Serial.printf("%02x", usb_arcade_data[0]);
    Serial.printf("%02x", usb_arcade_data[1]);
    Serial.printf("%02x", usb_arcade_data[2]);
    Serial.println();
#endif
     */
  }

  //delay to save cpu cycles
  delay(DELAY);
}

