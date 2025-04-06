#ifndef __BUTTON_HANDLER_HPP__
#define __BUTTON_HANDLER_HPP__

#include <OneButton.h>

class ESP32ButtonHandler
{
    OneButton *oneButton;
    TaskHandle_t threadHandle;

    public:
        ESP32ButtonHandler(int pinNumber, bool activeLow = true, bool pullupActive = true);
        virtual ~ESP32ButtonHandler();

        static void thread(void *context);

    protected:
        virtual void onClick(void) {}
        virtual void onDoubleClick(void) {}
        virtual void onLongPressStart(void) {}
        virtual void onLongPress(void) {}
        virtual void onLongPressStop(void) {}

    private:
        void loop(void);
};

#endif // __BUTTON_HANDLER_HPP__