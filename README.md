# ESP32ButtonHandler
A C++ ESP32 library for handling buttons with FreeRTOS support. The handler starts its own task to poll the button pin and calls the appropriate callback functions.

## Getting started

Add the include file to your project.
```cpp
#include <ESP32ButtonHandler.h>

// Define your button pins
#define PIN_BUTTON_1 0
#define PIN_BUTTON_2 14
```

Create a class that inherits from *ESP32ButtonHandler* and override the callback functions.
```cpp
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
```

Create an instance of your custom handler for each button to handle the button events.
```cpp
void setup() {
    Serial.begin(115200);
    CustomButtonHandler *button1 = new CustomButtonHandler(PIN_BUTTON_1);
    CustomButtonHandler *button2 = new CustomButtonHandler(PIN_BUTTON_2);
    // Press the buttons to see the callback logs
}

void loop()
{
    // ESP32ButtonHandler runs its own FreeRTOS task, so no need to call anything here for the buttons to work.
}
```

## Troubleshooting
If the callbacks aren't called
1. Check the *Serial* log for errors
2. Check that the correct pin number is specified

