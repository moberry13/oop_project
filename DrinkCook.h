#ifndef DRINKCOOK_H
#define DRINKCOOK_H

#include "Cook.h"

class DrinkCook : public Cook {
public:
    DrinkCook(int id);
    virtual ~DrinkCook() = default;

    // Убираем override или делаем правильно:
    std::string GetSpecializationString() const override { return "Beverage"; }
};

#endif // DRINKCOOK_H