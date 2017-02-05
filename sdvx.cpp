#include <Arduino.h>

#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#define DELAY     1  // Delay per loop in ms

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


// TODO: Test poll vs interrupt for buttons
void setup()
{
  for (int i = 0; i < SWITCH_COUNT; i++) {
    pinMode(switches[i].switchPin, INPUT_PULLUP);
    pinMode(switches[i].lightPin, OUTPUT);
  }

  Serial.begin(57600);
  Arcade.useManualSend(true); // lets reduce the calls

}

uint8_t buffer[2];
uint8_t lastSentState[3]; // the state of the last sent buttons

void loop() {
  int n = Arcade.recv(buffer, 0); // 0 timeout = do not wait
  // TODO: Actually get receiving working.
  if (n > 0) {
    Serial.print("Received: ");
    Serial.print(n);
    Serial.print((uint32_t) buffer);
  }

  // Read and set all the bytes for the buttons
  for (int i = 0; i < SWITCH_COUNT; i++) {

    if (digitalRead(switches[i].switchPin)) {
      Arcade.button(switches[i].joyId, 0);
    } else {
      Arcade.button(switches[i].joyId, 1);
    }
    // Hacky instant write
    // TODO: check if bit-banging is more efficient
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
    Serial.print("New State: ");
    Serial.printf("%02x", usb_arcade_data[0]);
    Serial.printf("%02x", usb_arcade_data[1]);
    Serial.printf("%02x", usb_arcade_data[2]);
    Serial.println();
  }

  delay(DELAY);
}


