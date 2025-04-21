#include "ESP32ButtonHandler.h"

ESP32ButtonHandler::ESP32ButtonHandler(uint8_t pin,
                                       bool activeLow,
                                       bool pullUp,
                                       unsigned long holdThreshold,
                                       unsigned long multiClickThreshold,
                                       unsigned long debounceDelay)
    : pin(pin), activeLow(activeLow), holdThreshold(holdThreshold),
      multiClickThreshold(multiClickThreshold), debounceDelay(debounceDelay),
      pressTime(0), releaseTime(0), lastHeldTime(0), lastDebounceTime(0),
      lastButtonRead(false), debouncedState(false),
      currentState(Idle), clickCount(0),
      onClickCallback(nullptr), onLongPressStartCallback(nullptr),
      onLongPressCallback(nullptr), onLongPressEndCallback(nullptr)
{
    pinMode(pin, pullUp ? INPUT_PULLUP : INPUT);
    if (xTaskCreate(thread, "ButtonThread", 4096, this, 1, &threadHandle) != pdPASS)
    {
        Serial.println("Failed to create button thread");
        vTaskDelete(threadHandle);
    }
}

ESP32ButtonHandler::~ESP32ButtonHandler() {
    // Clean up task
    if (threadHandle != nullptr) {
        vTaskDelete(threadHandle);
        threadHandle = nullptr;
    }
}

void ESP32ButtonHandler::addObserver(ButtonObserver* observer) {
    if (observer) {
        // Check if observer already exists to avoid duplicates
        for (auto existingObserver : observers) {
            if (existingObserver == observer) {
                return; // Observer already added
            }
        }
        observers.push_back(observer);
    }
}

void ESP32ButtonHandler::removeObserver(ButtonObserver* observer) {
    if (observer) {
        auto it = std::find(observers.begin(), observers.end(), observer);
        if (it != observers.end()) {
            observers.erase(it);
        }
    }
}

void ESP32ButtonHandler::setOnClickCallback(std::function<void(ESP32ButtonHandler*, int)> callback) {
    onClickCallback = callback;
}

void ESP32ButtonHandler::setOnLongPressStartCallback(std::function<void(ESP32ButtonHandler*)> callback) {
    onLongPressStartCallback = callback;
}

void ESP32ButtonHandler::setOnLongPressCallback(std::function<void(ESP32ButtonHandler*)> callback) {
    onLongPressCallback = callback;
}

void ESP32ButtonHandler::setOnLongPressEndCallback(std::function<void(ESP32ButtonHandler*)> callback) {
    onLongPressEndCallback = callback;
}

void ESP32ButtonHandler::update()
{
    bool rawRead = digitalRead(pin) == (activeLow ? LOW : HIGH);
    bool isActive = updateDebounce(rawRead);

    switch (currentState)
    {
    case Idle:
        if (isActive)
        {
            pressTime = millis();
            clickCount = 0;
            transition(Pressed);
        }
        break;

    case Pressed:
        if (!isActive)
        {
            releaseTime = millis();
            clickCount++;
            transition(Released);
        }
        else if (millis() - pressTime > holdThreshold)
        {
            if (clickCount == 0)
                transition(LongPressStart);
            else
                if (clickCount == 1)
                    transition(Clicked);
                else if (clickCount == 2)
                    transition(DoubleClicked);
                else if (clickCount > 2)
                    transition(MultiClicked);
        }
        break;

    case LongPressStart:
        lastHeldTime = millis();
        notifyOnLongPressStart();
        transition(LongPress);
        break;

    case LongPress:
        if (!isActive)
        {
            releaseTime = millis();
            transition(LongPressEnd);
        }
        else if (millis() - lastHeldTime > holdThreshold)
        {
            lastHeldTime = millis();
            notifyOnLongPress();
        }
        break;

    case LongPressEnd:
        notifyOnLongPressEnd();
        transition(Released);
        break;

    case Released:
        if (millis() - releaseTime > debounceDelay)
            transition(Counting);
        break;

    case Counting:
        if (isActive)
        {
            pressTime = millis();
            transition(Pressed);
        }
        else if (millis() - releaseTime >= multiClickThreshold)
        {
            if (clickCount == 1)
                transition(Clicked);
            else if (clickCount == 2)
                transition(DoubleClicked);
            else if (clickCount > 2)
                transition(MultiClicked);
        }
        break;

    case Clicked:
        notifyOnClick(clickCount);
        transition(Idle);
        break;

    case DoubleClicked:
        notifyOnClick(clickCount);
        transition(Idle);
        break;

    case MultiClicked:
        notifyOnClick(clickCount);
        transition(Idle);
        break;
    }
}

bool ESP32ButtonHandler::updateDebounce(bool reading)
{
    if (reading != lastButtonRead)
    {
        lastDebounceTime = millis();
    }

    if (millis() - lastDebounceTime > debounceDelay)
    {
        debouncedState = reading;
    }

    lastButtonRead = reading;
    return debouncedState;
}

void ESP32ButtonHandler::transition(ButtonState newState)
{
#ifdef DEBUG
    Serial.print("[");
    Serial.print(millis());
    Serial.print(" ms] Transitioning from ");
    Serial.print(stateToString(currentState));
    Serial.print(" to ");
    Serial.print(stateToString(newState));
    Serial.print(" clickCount = ");
    Serial.println(clickCount);
#endif // DEBUG

    currentState = newState;
}

const char *ESP32ButtonHandler::stateToString(ButtonState state) const
{
    switch (state)
    {
    case Idle:
        return "Idle";
    case Pressed:
        return "Pressed";
    case Released:
        return "Released";
    case Counting:
        return "Counting";
    case Clicked:
        return "Clicked";
    case DoubleClicked:
        return "DoubleClicked";
    case MultiClicked:
        return "MultiClicked";
    case LongPressStart:
        return "LongPressStart";
    case LongPress:
        return "LongPress";
    case LongPressEnd:
        return "LongPressEnd";
    default:
        return "Unknown";
    }
}

void ESP32ButtonHandler::thread(void *context)
{
    ESP32ButtonHandler *buttonHandler = static_cast<ESP32ButtonHandler *>(context);
    while (true)
    {
        buttonHandler->update();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void ESP32ButtonHandler::notifyOnClick(int count) {
    // Notify all observers
    for (auto observer : observers) {
        observer->onButtonClick(this, count);
    }

    // Call function callback if set
    if (onClickCallback) {
        onClickCallback(this, count);
    }
}

void ESP32ButtonHandler::notifyOnLongPressStart() {
    // Notify all observers
    for (auto observer : observers) {
        observer->onButtonLongPressStart(this);
    }

    // Call function callback if set
    if (onLongPressStartCallback) {
        onLongPressStartCallback(this);
    }
}

void ESP32ButtonHandler::notifyOnLongPress() {
    // Notify all observers
    for (auto observer : observers) {
        observer->onButtonLongPress(this);
    }

    // Call function callback if set
    if (onLongPressCallback) {
        onLongPressCallback(this);
    }
}

void ESP32ButtonHandler::notifyOnLongPressEnd() {
    // Notify all observers
    for (auto observer : observers) {
        observer->onButtonLongPressEnd(this);
    }

    // Call function callback if set
    if (onLongPressEndCallback) {
        onLongPressEndCallback(this);
    }
}

