#ifndef SWEETCOOK_H
#define SWEETCOOK_H

#include "Cook.h"

class SweetCook : public Cook {
public:
    SweetCook(int id);
    virtual ~SweetCook() = default;

    std::string GetSpecializationString() const override { return "Sweet"; }
};

#endif // SWEETCOOK_H