#include "SavoryCook.h"
#include <iostream>

SavoryCook::SavoryCook(int id)
    : Cook(id, DishType::SANDWICH, "SavoryCook") {
    std::cout << "[SavoryCook #" << id << "] Specialized in sandwiches\n";
}