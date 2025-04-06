#include <Arduino.h>

#include <ESP32ButtonHandler.hpp>

class CustomButtonHandler : public ESP32ButtonHandler
{
    private:
        int pinNumber;

    public:
        CustomButtonHandler(int pinNumber, bool activeLow = true, bool pullupActive = true)
            : ESP32ButtonHandler(pinNumber, activeLow, pullupActive), pinNumber(pinNumber) {}

    protected:
        void onClick(void) override
        {
            Serial.printf("Button click, pin %d.\n",  pinNumber);
        }
        void onDoubleClick(void) override
        {
            Serial.printf("Button double click, pin %d.\n",  pinNumber);
        }
        void onLongPressStart(void) override
        {
            Serial.printf("Button long press start, pin %d.\n",  pinNumber);
        }
        void onLongPress(void) override
        {
            Serial.printf("Button long press, pin %d.\n",  pinNumber);
        }
        void onLongPressStop(void) override
        {
            Serial.printf("Button long press stop, pin %d.\n",  pinNumber);
        }
};

ESP32ButtonHandler *button1;
ESP32ButtonHandler *button2;

void setup() {
    Serial.begin(115200);
    button1 = new CustomButtonHandler(0);
    button2 = new CustomButtonHandler(14);
}

void loop() {
    // ESP32ButtonHandler runs its own FreeRTOS task, so no need to call anything here.
}