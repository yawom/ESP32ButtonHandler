#include <Arduino.h>
#include <ESP32ButtonHandler.h>

// Define your button pins
#define PIN_BUTTON_1 0
#define PIN_BUTTON_2 14

// Example of a class implementing the ButtonObserver interface
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

// Global variables
ESP32ButtonHandler* button1;
ESP32ButtonHandler* button2;
ButtonLogger* logger1;
ButtonLogger* logger2;

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("ESP32ButtonHandler Observer Pattern Example");
    
    // Create button handlers
    button1 = new ESP32ButtonHandler(PIN_BUTTON_1);
    button2 = new ESP32ButtonHandler(PIN_BUTTON_2);
    
    // Create observers
    logger1 = new ButtonLogger("Logger1");
    logger2 = new ButtonLogger("Logger2");
    
    // Add multiple observers to button1
    button1->addObserver(logger1);
    button1->addObserver(logger2);
    
    // Add one observer to button2
    button2->addObserver(logger1);
    
    // Example of using lambda callbacks
    button2->setOnClickCallback([](ESP32ButtonHandler* handler, int clickCount) {
        Serial.printf("Lambda callback: Button on pin %d clicked %d time(s)\n", 
                     handler->getPin(), clickCount);
    });
    
    // Example of using std::bind with a member function
    auto longPressCallback = std::bind(&ButtonLogger::onButtonLongPress, logger2, 
                                      std::placeholders::_1);
    button2->setOnLongPressCallback(longPressCallback);
    
    Serial.println("Press buttons to see the callbacks in action");
}

void loop() {
    // ESP32ButtonHandler runs its own FreeRTOS task, so no need to call anything here.
    delay(1000);
}
