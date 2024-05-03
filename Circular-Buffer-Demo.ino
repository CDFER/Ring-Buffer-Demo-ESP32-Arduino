// File name: Circular-Buffer-Demo.ino

// MIT License
// Copyright (c) 2024 Chris Dirks
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
#include <Arduino.h>
#include "freertos/ringbuf.h"


void ringBufferDemo() {
    // 1. Define a static string to send to the ring buffer
    static const char messageToSend[] = "Hello, From the Ring Buffer!\r\n";

    // 2. Create a ring buffer with a size of 1028 bytes
    RingbufHandle_t bufferHandle = xRingbufferCreate(1028, RINGBUF_TYPE_BYTEBUF);

    // 3. Check if the ring buffer creation was successful
    if (bufferHandle == NULL) {
        Serial.println("Failed to create ring buffer!");
        return;
    }

    // 4. Send the message to the ring buffer
    if (xRingbufferSend(bufferHandle, messageToSend, sizeof(messageToSend), pdMS_TO_TICKS(1000)) != pdTRUE) {
        Serial.println("Failed to send message to ring buffer!");
        return;
    }

    // 5. Receive data from the ring buffer
    size_t receivedMessageSize;
    char* receivedMessage = (char*)xRingbufferReceiveUpTo(bufferHandle, &receivedMessageSize, pdMS_TO_TICKS(1000), sizeof(messageToSend));

    // 6. Check if the receive operation was successful
    if (receivedMessage != NULL) {
        // 7. Print the received message
        Serial.write(receivedMessage, receivedMessageSize);

        // 8. Return the received message to the ring buffer
        vRingbufferReturnItem(bufferHandle, (void*)receivedMessage);
    } else {
        Serial.println("Failed to receive message from ring buffer!");
    }
}

void setup() {
  Serial.begin(115200);   // Initialize the serial monitor

}

void loop() {
  ringBufferDemo();
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  
}
