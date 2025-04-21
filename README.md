# ESP32ButtonHandler
A C++ ESP32 library for handling buttons with FreeRTOS support. The handler starts its own task to poll the button pin and supports the observer pattern with multiple observers and callback functions.

## Getting started

Add the include file to your project.
```cpp
#include <ESP32ButtonHandler.h>

// Define your button pins
#define PIN_BUTTON_1 0
#define PIN_BUTTON_2 14
```

## Using the Observer Pattern

Create a class that implements the ButtonObserver interface:

```cpp
class ButtonLogger : public ButtonObserver {
private:
    String name;

public:
    ButtonLogger(const String& name) : name(name) {}

    void onButtonClick(ESP32ButtonHandler* handler, int clickCount) override {
        Serial.printf("[%s] Button on pin %d clicked %d time(s)\n",
                     name.c_str(), handler->getPin(), clickCount);
    }

    void onButtonLongPressStart(ESP32ButtonHandler* handler) override {
        Serial.printf("[%s] Button on pin %d long press started\n",
                     name.c_str(), handler->getPin());
    }

    void onButtonLongPress(ESP32ButtonHandler* handler) override {
        Serial.printf("[%s] Button on pin %d long press continues\n",
                     name.c_str(), handler->getPin());
    }

    void onButtonLongPressEnd(ESP32ButtonHandler* handler) override {
        Serial.printf("[%s] Button on pin %d long press ended\n",
                     name.c_str(), handler->getPin());
    }
};
```

Create button handlers and register observers:

```cpp
void setup() {
    Serial.begin(115200);

    // Create button handlers
    ESP32ButtonHandler* button1 = new ESP32ButtonHandler(PIN_BUTTON_1);
    ESP32ButtonHandler* button2 = new ESP32ButtonHandler(PIN_BUTTON_2);

    // Create observers
    ButtonLogger* logger1 = new ButtonLogger("Logger1");
    ButtonLogger* logger2 = new ButtonLogger("Logger2");

    // Add multiple observers to button1
    button1->addObserver(logger1);
    button1->addObserver(logger2);

    // Add one observer to button2
    button2->addObserver(logger1);
}

void loop() {
    // ESP32ButtonHandler runs its own FreeRTOS task, so no need to call anything here.
}
```

## Using Lambda Callbacks

You can also use lambda functions for callbacks:

```cpp
ESP32ButtonHandler* button = new ESP32ButtonHandler(PIN_BUTTON_1);

// Set click callback using lambda
button->setOnClickCallback([](ESP32ButtonHandler* handler, int clickCount) {
    Serial.printf("Button on pin %d clicked %d time(s)\n",
                 handler->getPin(), clickCount);
});

// Set long press callback
button->setOnLongPressCallback([](ESP32ButtonHandler* handler) {
    Serial.printf("Button on pin %d is being held\n", handler->getPin());
});
```

## Using std::bind

You can use std::bind to connect member functions as callbacks:

```cpp
class MyClass {
public:
    void handleButtonPress(ESP32ButtonHandler* handler) {
        Serial.printf("Button on pin %d pressed\n", handler->getPin());
    }
};

MyClass myObject;
ESP32ButtonHandler* button = new ESP32ButtonHandler(PIN_BUTTON_1);

// Bind member function as callback
auto callback = std::bind(&MyClass::handleButtonPress, &myObject, std::placeholders::_1);
button->setOnLongPressStartCallback(callback);
```

## Troubleshooting
If the callbacks aren't called
1. Check the *Serial* log for errors
2. Check that the correct pin number is specified

