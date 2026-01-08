#include "Dish.h"
#include <iostream>

Dish::Dish(int id, const std::string& name, DishType type, int cookingTime)
    : id_(id), name_(name), type_(type), cookingTime_(cookingTime) {
    std::cout << "[Dish] Created: " << name_ << " (ID: " << id_
        << ", Type: " << GetTypeString()
        << ", Time: " << cookingTime_ << " ticks)\n";
}

std::string Dish::GetTypeString() const {
    switch (type_) {
    case DishType::BEVERAGE: return "Beverage";
    case DishType::SANDWICH: return "Sandwich";
    case DishType::SWEET:    return "Sweet";
    default: return "Unknown";
    }
}

// Статические фабричные методы
Dish* Dish::CreateBeverage(int id, const std::string& name, int time) {
    return new Dish(id, name, DishType::BEVERAGE, time);
}

Dish* Dish::CreateSandwich(int id, const std::string& name, int time) {
    return new Dish(id, name, DishType::SANDWICH, time);
}

Dish* Dish::CreateSweet(int id, const std::string& name, int time) {
    return new Dish(id, name, DishType::SWEET, time);
}