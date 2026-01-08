#include "Cook.h"
#include "MSG.h"
#include "Manager.h"
#include <iostream>
#include <iomanip>

Cook::Cook(int id, DishType specialization, const std::string& name)
    : Human(name + "_" + std::to_string(id), 1),
    cookID_(id), specialization_(specialization), isAvailable_(true) {
    std::cout << "[Cook #" << cookID_ << "] " << name
        << " created, specialization: " << GetSpecializationString() << "\n";
}

bool Cook::CanTakeMoreDishes() const {
    return currentDishes_.size() < 5;  // Максимум 5 блюд одновременно
}

bool Cook::AssignDish(std::shared_ptr<Dish> dish) {
    if (!dish) {
        std::cout << "[Cook #" << cookID_ << "] Can't assign null dish\n";
        return false;
    }

    if (dish->GetType() != specialization_) {
        std::cout << "[Cook #" << cookID_ << "] Wrong dish type! I'm a "
            << GetSpecializationString() << " cook, but got: "
            << dish->GetTypeString() << "\n";
        return false;
    }

    if (!CanTakeMoreDishes()) {
        std::cout << "[Cook #" << cookID_ << "] Can't take more dishes (max 5). "
            << "Current: " << currentDishes_.size() << "\n";
        return false;
    }

    // Добавляем блюдо
    currentDishes_.push_back(dish);

    // Создаем таймер для этого блюда
    Timer timer(dish->GetCookingTime());
    dishTimers_.push_back(timer);

    std::cout << "[Cook #" << cookID_ << "] Started cooking: " << dish->GetName()
        << " (" << dish->GetCookingTime() << " ticks)\n";

    isAvailable_ = CanTakeMoreDishes();
    return true;
}

void Cook::UpdateCooking() {
    for (size_t i = 0; i < dishTimers_.size(); ++i) {
        dishTimers_[i].Update();
    }
}

void Cook::CheckFinishedDishes() {
    for (size_t i = 0; i < dishTimers_.size(); ++i) {
        if (dishTimers_[i].IsFinished()) {
            // Блюдо готово
            auto dish = currentDishes_[i];
            OnDishFinished(dish);

            // Удаляем блюдо и таймер
            currentDishes_.erase(currentDishes_.begin() + i);
            dishTimers_.erase(dishTimers_.begin() + i);

            // Возвращаемся, т.к. индексы изменились
            CheckFinishedDishes();
            return;
        }
    }
}

void Cook::OnDishFinished(std::shared_ptr<Dish> dish) {
    std::cout << "[Cook #" << cookID_ << "] Finished cooking: "
        << dish->GetName() << "\n";

    // ★★★★ ИСПРАВЛЕНИЕ: создаём shared_ptr с копией блюда ★★★★
    auto dishCopy = std::make_shared<Dish>(*dish);
    MSG msg(MSGType::Dish_Ready, this, dishCopy);
    Manager::GetInstance().Send(msg);

    // Проверяем, можем ли взять новые блюда
    isAvailable_ = CanTakeMoreDishes();
}
void Cook::PerformAction() {
    std::cout << "[Cook #" << cookID_ << "] Cooking "
        << currentDishes_.size() << " dish(es)\n";
}

void Cook::Update() {
    UpdateCooking();
    CheckFinishedDishes();
}

void Cook::OnMsg(const MSG& msg) {
    switch (msg.type) {
    case MSGType::Prepare_Dish: {
        // ★★★★ ИСПРАВЛЕНИЕ: используем безопасное извлечение shared_ptr ★★★★
        auto dishPtr = MSGHelper::GetPayloadAs<Dish>(msg);
        if (dishPtr) {
            // Проверяем специализацию
            if (dishPtr->GetType() == specialization_) {
                // Создаём копию для повара (чтобы у него была своя версия)
                auto dishCopy = std::make_shared<Dish>(*dishPtr);
                AssignDish(dishCopy);
            }
            else {
                std::cout << "[Cook #" << cookID_ << "] Ignoring dish: "
                    << dishPtr->GetName() << " (not my specialization)\n";
            }
        }
        break;
    }
    default:
        break;
    }
}

std::string Cook::GetSpecializationString() const {
    switch (specialization_) {
    case DishType::BEVERAGE: return "Beverage";
    case DishType::SANDWICH: return "Sandwich";
    case DishType::SWEET:    return "Sweet";
    default: return "Unknown";
    }
}

void Cook::PrintStatus() const {
    std::cout << "\n=== Cook #" << cookID_ << " ==="
        << "\nName: " << GetName()
        << "\nSpecialization: " << GetSpecializationString()
        << "\nAvailable: " << (isAvailable_ ? "Yes" : "No")
        << "\nCurrent dishes: " << currentDishes_.size() << "/5"
        << "\nDishes in progress:\n";

    for (size_t i = 0; i < currentDishes_.size(); ++i) {
        const auto& dish = currentDishes_[i];
        const auto& timer = dishTimers_[i];
        std::cout << "  " << (i + 1) << ". " << dish->GetName()
            << " - " << timer.GetRemainingTime() << "/"
            << timer.GetTotalTime() << " ticks ("
            << std::fixed << std::setprecision(1)
            << timer.GetProgress() * 100.0f << "%)\n";
    }
    std::cout << "====================\n";
}