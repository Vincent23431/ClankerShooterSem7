#ifndef IBUTTON_H
#define IBUTTON_H
#include <Arduino.h>

class IButton {
public:
    virtual ~IButton() {};
    virtual bool readButton() = 0;
};

#endif
