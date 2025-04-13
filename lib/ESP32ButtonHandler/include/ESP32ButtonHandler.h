#ifndef ESP32_BUTTON_HANDLER_H
#define ESP32_BUTTON_HANDLER_H

#include <Arduino.h>

class ESP32ButtonHandler
{
public:
    ESP32ButtonHandler(uint8_t pin,
                       bool activeLow = true,
                       bool pullUp = true,
                       unsigned long holdThreshold = 500,
                       unsigned long multiClickThreshold = 250,
                       unsigned long debounceDelay = 20);

    virtual ~ESP32ButtonHandler();

    void update();

protected:
    virtual void onClick(int count) {}
    virtual void onLongPressStart() {}
    virtual void onLongPress() {}
    virtual void onLongPressEnd() {}

private:
    TaskHandle_t threadHandle;

    uint8_t pin;
    bool activeLow;
    unsigned long holdThreshold;
    unsigned long multiClickThreshold;
    unsigned long debounceDelay;

    unsigned long pressTime;
    unsigned long releaseTime;
    unsigned long lastHeldTime;
    unsigned long lastDebounceTime;

    bool lastButtonRead;
    bool debouncedState;

    enum ButtonState
    {
        Idle,
        Pressed,
        Released,
        Counting,
        Clicked,
        DoubleClicked,
        MultiClicked,
        LongPressStart,
        LongPress,
        LongPressEnd
    };

    ButtonState currentState;
    uint8_t clickCount;

    bool updateDebounce(bool reading);
    void transition(ButtonState newState);
    const char *stateToString(ButtonState state) const;

    static void thread(void *context);
};

#endif // ESP32_BUTTON_HANDLER_H