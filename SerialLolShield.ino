/**
 * Serial LoL Shield Test
 *
 * Dependencies:
 * - https://github.com/jprodgers/LoLshield
 *
 * Created by Ethan Smith on 2016/11/4
 */

#define MESSAGE_MAX_LENGTH 9  // e.g. T 10 15 1

#include <Charliplexing.h>  // The LoLShield library

char serialBuffer[MESSAGE_MAX_LENGTH];
const char NO_MESSAGE = '\0';
int serialBufferIndex;

void setup() {
   Serial.begin(9600);
   Serial.println("powering up...");

   resetSerialBuffer();

   LedSign::Init(GRAYSCALE);
}

void loop() {
   const char* message;
   message = fetchSerialMessage();

   if (message[0] != '\0') {
      Serial.print("processing [");
      Serial.print(message);
      Serial.println("]");

      processCommand(message);
   }
}

const char* fetchSerialMessage() {
   if (Serial.available() > 0) {
      char received = Serial.read();

      if (received == '\n') {
         resetSerialBuffer();
         return serialBuffer;
      } else {
         serialBuffer[serialBufferIndex] = received;
         serialBufferIndex ++;
         serialBuffer[serialBufferIndex] = '\0';

         // If message is longer than expected max message length,
         // return the message we have and reset the buffer.
         if (serialBufferIndex >= MESSAGE_MAX_LENGTH) {
            resetSerialBuffer();
            return serialBuffer;
         }
      }
   }

   return &NO_MESSAGE;
}

void resetSerialBuffer() {
   serialBufferIndex = 0;
}

void processCommand(const char* command) {
   switch (command[0]) {
      case 't':
      case 'T':
         handleToggleLEDCommand(&command[1]);
         break;
      default:
         Serial.println("invalid command");
         break;
   }
}

void handleToggleLEDCommand(const char* buffer) {
   char *fieldPointer;
   uint8_t x, y, level;

   x = (uint8_t)strtol(buffer, &fieldPointer, 10);
   y = (uint8_t)strtol(fieldPointer, &fieldPointer, 10);
   level = (uint8_t)strtol(fieldPointer, &fieldPointer, 10);

   toggleLED(x, y, level);
}

void toggleLED(uint8_t x, uint8_t y, uint8_t level) {
   x = bound(x, 0, DISPLAY_COLS - 1);
   y = bound(y, 0, DISPLAY_ROWS - 1);
   level = bound(level, 0, SHADES - 1);

   Serial.print("setting led (");
   Serial.print(x);
   Serial.print(",");
   Serial.print(y);
   Serial.print(") to: ");
   Serial.println(level);

   LedSign::Set(x, y, level);
}

uint8_t bound(uint8_t value, uint8_t min, uint8_t max) {
   return max(min(value, max), min);
}
