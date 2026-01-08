#ifndef SAVORYCOOK_H
#define SAVORYCOOK_H

#include "Cook.h"

class SavoryCook : public Cook {
public:
    SavoryCook(int id);
    virtual ~SavoryCook() = default;

    std::string GetSpecializationString() const override { return "Savory"; }
};

#endif // SAVORYCOOK_H