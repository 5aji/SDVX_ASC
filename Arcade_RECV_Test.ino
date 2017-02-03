 /* Basic Raw HID Example
   Teensy can send/receive 64 byte packets with a
   dedicated program running on a PC or Mac.

   You must select Raw HID from the "Tools > USB Type" menu

   Optional: LEDs should be connected to pins 0-7,
   and analog signals to the analog inputs.

   This example code is in the public domain.
*/


void setup() {
  Serial.begin(57600);
  Serial.println(F("RawHID Example"));
  for (int i=0; i<7; i++) {
    pinMode(i, OUTPUT);
  }
}

// Arcade packets are always 2 bytes
byte buffer[2];

void loop() {
  int n;
  n = Arcade.recv(buffer, 15); // 0 timeout = do not wait
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
}
