#ifndef ESP32_BUTTON_HANDLER_H
#define ESP32_BUTTON_HANDLER_H

#include <Arduino.h>
#include <functional>
#include <vector>

// Forward declaration
class ESP32ButtonHandler;

/**
 * @brief Interface for button event observers
 */
class ButtonObserver {
public:
    virtual ~ButtonObserver() = default;

    /**
     * @brief Called when a button click is detected
     * @param handler The button handler that triggered the event
     * @param clickCount Number of clicks (1 for single click, 2 for double click, etc.)
     */
    virtual void onButtonClick(ESP32ButtonHandler* handler, int clickCount) {}

    /**
     * @brief Called when a long press starts
     * @param handler The button handler that triggered the event
     */
    virtual void onButtonLongPressStart(ESP32ButtonHandler* handler) {}

    /**
     * @brief Called periodically during a long press
     * @param handler The button handler that triggered the event
     */
    virtual void onButtonLongPress(ESP32ButtonHandler* handler) {}

    /**
     * @brief Called when a long press ends
     * @param handler The button handler that triggered the event
     */
    virtual void onButtonLongPressEnd(ESP32ButtonHandler* handler) {}
};

/**
 * @brief Button handler for ESP32 with observer pattern support
 */
class ESP32ButtonHandler
{
public:
    /**
     * @brief Construct a new ESP32ButtonHandler
     *
     * @param pin GPIO pin number
     * @param activeLow true if button is active when pin is LOW
     * @param pullUp true to enable internal pull-up resistor
     * @param holdThreshold time in ms to trigger long press
     * @param multiClickThreshold time in ms to wait for additional clicks
     * @param debounceDelay debounce time in ms
     */
    ESP32ButtonHandler(uint8_t pin,
                       bool activeLow = true,
                       bool pullUp = true,
                       unsigned long holdThreshold = 500,
                       unsigned long multiClickThreshold = 250,
                       unsigned long debounceDelay = 20);

    /**
     * @brief Destructor
     */
    ~ESP32ButtonHandler();

    /**
     * @brief Add an observer to receive button events
     * @param observer Pointer to observer object
     */
    void addObserver(ButtonObserver* observer);

    /**
     * @brief Remove an observer
     * @param observer Pointer to observer to remove
     */
    void removeObserver(ButtonObserver* observer);

    /**
     * @brief Set callback for click events
     * @param callback Function to call on click events
     */
    void setOnClickCallback(std::function<void(ESP32ButtonHandler*, int)> callback);

    /**
     * @brief Set callback for long press start events
     * @param callback Function to call on long press start
     */
    void setOnLongPressStartCallback(std::function<void(ESP32ButtonHandler*)> callback);

    /**
     * @brief Set callback for long press events
     * @param callback Function to call during long press
     */
    void setOnLongPressCallback(std::function<void(ESP32ButtonHandler*)> callback);

    /**
     * @brief Set callback for long press end events
     * @param callback Function to call when long press ends
     */
    void setOnLongPressEndCallback(std::function<void(ESP32ButtonHandler*)> callback);

    /**
     * @brief Get the pin number
     * @return uint8_t Pin number
     */
    uint8_t getPin() const { return pin; }

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

    // Observer pattern implementation
    std::vector<ButtonObserver*> observers;

    // Function callbacks
    std::function<void(ESP32ButtonHandler*, int)> onClickCallback;
    std::function<void(ESP32ButtonHandler*)> onLongPressStartCallback;
    std::function<void(ESP32ButtonHandler*)> onLongPressCallback;
    std::function<void(ESP32ButtonHandler*)> onLongPressEndCallback;

    // Notification methods
    void notifyOnClick(int count);
    void notifyOnLongPressStart();
    void notifyOnLongPress();
    void notifyOnLongPressEnd();

    bool updateDebounce(bool reading);
    void transition(ButtonState newState);
    const char *stateToString(ButtonState state) const;
    void update();

    static void thread(void *context);
};

#endif // ESP32_BUTTON_HANDLER_H