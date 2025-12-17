#include <Arduino.h>
#include <ESP32ButtonHandler.h>

// Define your button pins
#define PIN_BUTTON_1 0
#define PIN_BUTTON_2 14

// Simple example using lambda callbacks

void setup()
{
    Serial.begin(115200);
    delay(1000);
    Serial.println("ESP32ButtonHandler Simple Example");

    // Create button handlers
    ESP32ButtonHandler* button1 = new ESP32ButtonHandler(PIN_BUTTON_1);
    ESP32ButtonHandler* button2 = new ESP32ButtonHandler(PIN_BUTTON_2);

    // Check initialization
    if (!button1->isInitialized() || !button2->isInitialized()) {
        Serial.println("ERROR: Failed to initialize button handlers!");
        while(1) { delay(1000); }
    }

    // Set up callbacks for button1
    button1->setOnClickCallback([](ESP32ButtonHandler* handler, int clickCount) {
        Serial.printf("Button on pin %d clicked %d time(s)\n",
                     handler->getPin(), clickCount);
    });

    button1->setOnLongPressStartCallback([](ESP32ButtonHandler* handler) {
        Serial.printf("Button on pin %d long press started\n", handler->getPin());
    });

    button1->setOnLongPressCallback([](ESP32ButtonHandler* handler) {
        Serial.printf("Button on pin %d long press continues\n", handler->getPin());
    });

    button1->setOnLongPressEndCallback([](ESP32ButtonHandler* handler) {
        Serial.printf("Button on pin %d long press ended\n", handler->getPin());
    });

    // Set up callbacks for button2
    button2->setOnClickCallback([](ESP32ButtonHandler* handler, int clickCount) {
        Serial.printf("Button on pin %d clicked %d time(s)\n",
                     handler->getPin(), clickCount);
    });

    Serial.println("Press buttons to see the callbacks in action");
}

void loop()
{
    // ESP32ButtonHandler runs its own FreeRTOS task, so no need to call anything here.
    delay(1000);
}