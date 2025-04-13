# ESP32ButtonHandler
A C++ ESP32 library for handling buttons with FreeRTOS support.
It implementation a simple class and starts a thread to handle the tick events.

## Getting started

Add the include file to your project.
```cpp
#include <ESP32ButtonHandler.hpp>
```

Create a class that inherits from *ESP32ButtonHandler* and overrides the callback functions.
```cpp
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
```

Create an instance of your custom handler to handle the button events.
```cpp
void setup() {
    Serial.begin(115200);
    ESP32ButtonHandler *button1 = new CustomButtonHandler(PIN_BUTTON_1);
    // Press the buttons to see the callback logs
}
```

## Troubleshooting
If the callbacks aren't called
1. Check the *Serial* log for errors
2. Check that the correct pin number is specified

