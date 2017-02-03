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
  {1, 5, 7}, // START
  {23, 7, 1}, // A
  {20, 9, 2}, // B
  {10, 16, 3}, // C
  {14, 15, 4}, // D
  {16, 17, 5}, // FX A
  {18, 19, 6}, // FX B
};

void setup()
{
  for (int i = 0; i < SWITCH_COUNT; i++) {
    pinMode(switches[i].switchPin, INPUT_PULLUP);
    pinMode(switches[i].lightPin, OUTPUT);
  }
  pinMode(5, OUTPUT);
  Serial.begin(57600);
}
byte buffer[2];

void loop() {
  int n = Arcade.recv(buffer, 15); // no timeout

  if (n > 0) {
    Serial.print("recieved: ");
    Serial.print(n);
    Serial.print((uint32_t) buffer);
  }
  for (int i = 0; i < SWITCH_COUNT; i++) {
    if (digitalRead(switches[i].switchPin) == LOW) {
      Arcade.button(switches[i].joyId, 1);
    } else {
      Arcade.button(switches[i].joyId, 0);
    }
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
  delay(DELAY);
}
