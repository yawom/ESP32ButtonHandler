#include <Arduino.h>

#include <ESP32ButtonHandler.h>

class CustomButtonHandler : public ESP32ButtonHandler
{
private:
    int pinNumber;

public:
    CustomButtonHandler(int pinNumber, bool activeLow = true, bool pullupActive = false)
        : ESP32ButtonHandler(pinNumber, activeLow, pullupActive), pinNumber(pinNumber) {}

protected:
    void onClick(int clickCount) override
    {
        Serial.printf("%d click(s), pin %d.\n", clickCount, pinNumber);
    }
    void onLongPressStart(void) override
    {
        Serial.printf("Long press start, pin %d.\n", pinNumber);
    }
    void onLongPress(void) override
    {
        Serial.printf("Long press, pin %d.\n", pinNumber);
    }
    void onLongPressEnd(void) override
    {
        Serial.printf("Long press stop, pin %d.\n", pinNumber);
    }
};

ESP32ButtonHandler *button1;
ESP32ButtonHandler *button2;

void setup()
{
    Serial.begin(115200);
    button1 = new CustomButtonHandler(0);
    button2 = new CustomButtonHandler(14);
}

void loop()
{
    // ESP32ButtonHandler runs its own FreeRTOS task, so no need to call anything here.
}