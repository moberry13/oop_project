#include "DrinkCook.h"
#include <iostream>

DrinkCook::DrinkCook(int id)
    : Cook(id, DishType::BEVERAGE, "DrinkCook") {
    std::cout << "[DrinkCook #" << id << "] Specialized in beverages\n";
}