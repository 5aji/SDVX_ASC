// You will need the SoftPWM and Encoder libraries, available from
// the teensy website. Google!

// Edit the fade time to your liking.
// Edit the pins and keys if you use different ones.
// ENCODER PINS MUST BE 0, 1, 2, 3 FOR BEST PERFORMANCE
// Everything else can go wherever.

#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#define DELAY     1  // Delay per loop in ms
#define FADE_TIME 200

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

#define SWITCH_COUNT 7

switch_t switches[SWITCH_COUNT] = {
  {4, 12, 1}, // A
  {5, 14, 2}, // B
  {6, 15, 3}, // C
  {7, 16, 4}, // D
  {8, 17, 5}, // FX A
  {9, 18, 6}, // FX B
  {10, 19, 7}, // START
  {11, 0, 8} // TEST (no led)
};

void setup()
{
  for (int i = 0; i < SWITCH_COUNT; i++) {
    pinMode(switches[i].switchPin, INPUT_PULLUP);
    pinMode(switches[i].lightPin, OUTPUT);
  }
  pinMode(5, OUTPUT);
  Serial.begin(57600);
  Arcade.useManualSend(true); // lets reduce the calls
}
byte buffer[2];

void loop() {
  int n = Arcade.recv(buffer, 0); // 0 timeout = do not wait
  if (n > 0) {
    // the computer sent a message.  Display the bits
    // of the first byte on pin 0 to 7.  Ignore the
    // other 63 bytes!
    Serial.print(F("Received packet, first byte: "));
    Serial.println((int)buffer[0]);
    for (int i=0; i<8; i++) {
      int b = buffer[0] & (1 << i);
      digitalWrite(i, b);
    }
  }

  if (n > 0) {
    Serial.print("recieved: ");
    Serial.print(n);
    Serial.print((uint32_t) buffer);
  }
  for (int i = 0; i < ENCODER_COUNT; i++) {
    long encVal = encoders[i].enc.read();
    if (encVal != 0) {
      if (encoders[i].axis == 'x') {
        Arcade.X(encVal);
      } else {
        Arcade.Y(encVal);
      }
      encoders[i].enc.write(0);
    }
  }
  ButtonChange(1);
  delay(DELAY);
}

void ButtonChange(uint id) {
  usb_arcade_data[0] ^= 1 << id; // toggle the state of the button id
  Arcade.send_now(); // the state changed so we send it.
}
