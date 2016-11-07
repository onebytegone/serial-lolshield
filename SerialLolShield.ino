/**
 * Serial LoL Shield Test
 *
 * Created by Ethan Smith on 2016/11/4
 */

#define MESSAGE_MAX_LENGTH 7  // e.g. T00001

char serialBuffer[MESSAGE_MAX_LENGTH];
const char NO_MESSAGE = '\0';
int serialBufferIndex;

void setup() {
   Serial.begin(9600);
   Serial.println('Starting...');

   resetSerialBuffer();
}

void loop() {
   const char* message;
   message = fetchSerialMessage();

   if (message[0] != '\0') {
      Serial.print('[');
      Serial.print(message);
      Serial.println(']');
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
