#include "ESP32ButtonHandler.hpp"

ESP32ButtonHandler::ESP32ButtonHandler(int pinNumber, bool activeLow, bool pullupActive) {
    oneButton = new OneButton(pinNumber, activeLow, pullupActive);
    oneButton->attachClick([](void *context) {
        static_cast<ButtonHandler *>(context)->onClick();
    }, this);
    oneButton->attachDoubleClick([](void *context) {
        static_cast<ButtonHandler *>(context)->onDoubleClick();
    }, this);
    oneButton->attachLongPressStart([](void *context) {
        static_cast<ButtonHandler *>(context)->onLongPressStart();
    }, this);
    oneButton->attachDuringLongPress([](void *context) {
        static_cast<ButtonHandler *>(context)->onLongPress();
    }, this);
    oneButton->attachLongPressStop([](void *context) {
        static_cast<ButtonHandler *>(context)->onLongPressStop();
    }, this);

    if (xTaskCreate(thread, "ButtonThread", 4096, this, 1, &threadHandle) != pdPASS) {
        Serial.println("Failed to create button thread");
    }
}

ESP32ButtonHandler::~ESP32ButtonHandler() {
    vTaskDelete(threadHandle);
    delete oneButton;
    oneButton = nullptr;
}

void ESP32ButtonHandler::loop(void)
{
    oneButton->tick();
}

void ESP32ButtonHandler::thread(void *context)
{
    ButtonHandler *buttonHandler = static_cast<ButtonHandler *>(context);
    while (true) {
        buttonHandler->loop();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
