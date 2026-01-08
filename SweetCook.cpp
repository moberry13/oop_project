#include "SweetCook.h"
#include <iostream>

SweetCook::SweetCook(int id)
    : Cook(id, DishType::SWEET, "SweetCook") {
    std::cout << "[SweetCook #" << id << "] Specialized in sweet dishes\n";
}