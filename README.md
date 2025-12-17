# ESP32ButtonHandler
A C++ ESP32 library for handling buttons with FreeRTOS support. The handler starts its own task to poll the button pin and supports the observer pattern with multiple observers and callback functions.

## Features

- **Non-blocking**: Uses FreeRTOS tasks for background button monitoring
- **Thread-safe**: Mutex-protected operations for safe concurrent access
- **Observer Pattern**: Support for multiple observers per button
- **Lambda Callbacks**: Modern C++ callback support
- **Multi-click Detection**: Detects single, double, and multi-clicks
- **Long Press Support**: Start, during, and end events for long presses
- **Debouncing**: Built-in debounce with configurable delay

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

## Error Handling

Always check if the handler was initialized successfully:

```cpp
ESP32ButtonHandler* button = new ESP32ButtonHandler(PIN_BUTTON_1);
if (!button->isInitialized()) {
    Serial.println("ERROR: Failed to initialize button handler!");
    // Handle error appropriately
}
```

## Thread Safety

This library is thread-safe. You can safely:
- Add/remove observers from any task or the main loop
- Set callbacks from any context
- Multiple button handlers can be used simultaneously

All operations use mutex protection to ensure thread safety.

## Debug Mode

To enable debug output, define `ESP32_BUTTON_DEBUG` before including the library:

```cpp
#define ESP32_BUTTON_DEBUG
#include <ESP32ButtonHandler.h>
```

Note: Debug mode requires Serial to be initialized.

## Configuration Parameters

The constructor accepts the following parameters:

- `pin`: GPIO pin number
- `activeLow`: true if button is active when pin is LOW (default: true)
- `pullUp`: true to enable internal pull-up resistor (default: true)
- `holdThreshold`: time in ms to trigger long press (default: 500)
- `multiClickThreshold`: time in ms to wait for additional clicks (default: 250)
- `debounceDelay`: debounce time in ms (default: 20)

Example with custom configuration:
```cpp
// Button with 1 second long press threshold and 500ms multi-click window
ESP32ButtonHandler* button = new ESP32ButtonHandler(
    PIN_BUTTON_1,     // pin
    true,             // activeLow
    true,             // pullUp
    1000,             // holdThreshold
    500,              // multiClickThreshold
    50                // debounceDelay
);
```

## Resource Management

The library automatically manages FreeRTOS resources (tasks and mutexes). When the button handler is destroyed, all resources are properly cleaned up.

```cpp
// Clean up when done (typically not needed for embedded systems)
delete button;
```

## Troubleshooting

If the callbacks aren't called:
1. Check that `isInitialized()` returns true
2. Check that the correct pin number is specified
3. Verify your button wiring matches the `activeLow` and `pullUp` settings
4. Enable `ESP32_BUTTON_DEBUG` to see state transitions

## Requirements

- ESP32 platform
- Arduino framework
- FreeRTOS (included with ESP32 Arduino core)

