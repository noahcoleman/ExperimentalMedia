#include <Servo.h>
Servo servoX;
Servo servoY;
const int laserPin = 11;
const byte PACKET_BOUNDARY_BYTE = 254; 
// PACKET_BOUNDARY_BYTE:  when we read this number from the serial 
// device, we know we have found the boundary between two packets.  
// We MUST NOT send the byte value "254" unless it is a packet boundary.  
// This means that on the sender side, we must make sure our the bytes
// we are sending always meet the following requirement:
// 0 <= BYTE_BEING_SENT <= 254
// That way we can always know that 254 means packet boundary and
// not something else;

const int NUM_BYTES_EXPECTED = 3;
// This is the number of meaningful data bytes we expect to receive.

byte buffer[NUM_BYTES_EXPECTED] = { 
  0, 0 ,0 };  
// This is the buffer where we will store our data byte values 
// until we can do something with them.  We can "do something" with
// Them once we know that we have a complete packet (as indiciated
// by the arrival of the packet boundary marker (254).

byte bufferIndex = 0;
// This tells us where we are currently writing into our buffer;

//----------------

const int NUM_SERVO_PINS = NUM_BYTES_EXPECTED; // we expect one byte for each LED
const byte SERVO_PINS[NUM_SERVO_PINS] = { 
  9, 10, 11 };//array with pin numbers for servos
byte servoValues[NUM_SERVO_PINS] = { 
  0, 0, 0 };

void setup()
{  
  Serial.begin(9600);
  servoX.attach(9);
  servoY.attach(10);
  for(int i = 0; i < NUM_SERVO_PINS; ++i)
  {
    pinMode(SERVO_PINS[i],OUTPUT); 
  }
}


void loop()
{
  while(Serial.available() > 0) 
  {
    byte inByte = Serial.read();

    if(inByte == 254)
    {
      if(bufferIndex == NUM_BYTES_EXPECTED)
      {
        // we know that we have found a packet boundary
        // and the packet is full, so we copy the values
        // to our ledValues array.

        // We could write them directly into that array, 
        // but instead we keep the buffer and the led 
        // values array separate in case a byte is missing
        // or a packet is malformed.  Also, in other cases,
        // more complex packets might be processed 
        // or interpreted in a more complex way when they
        // are finished.
        for(int i = 0; i < NUM_SERVO_PINS; ++i)
        {
          servoValues[i] = buffer[i];
        }
      } 
      else 
      {
        // The bufferIndex was NOT equal to the NUM_BYTES_EXPECTED
        // so we did not receive a full packet.  So, we won't
        // transfer the data.  This could happen because it was
        // the first time data was sent, or it could happen 
        // because there is a bad serial connection.
      }

      // we reset the bufferIndex to zero because we got inByte==254
      bufferIndex = 0;
    } 
    else
    {
      if(bufferIndex < NUM_BYTES_EXPECTED)
      {
        buffer[bufferIndex] = inByte;
        bufferIndex++; // move the buffer index
      }
      else
      {
        // this was some kind of error
        // we had more bytes than expected before
        // we received a packet boundary marker ...
        // this should be investigated.
        // So, we reset the buffer index to 0
        bufferIndex = 0;
      }
    }
  }

  // write our values to the output.
      servoX.write(servoValues[0]);
      servoY.write(servoValues[1]);
      analogWrite(laserPin, servoValues[2]);
      Serial.println(servoValues[0]);
      Serial.println(servoValues[1]);
      Serial.println(servoValues[2]);


}


