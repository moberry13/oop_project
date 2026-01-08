#include "Order.h"
#include <iostream>
#include <sstream>

// »нициализаци€ статической переменной
int Order::nextOrderID = 1;

Order::Order(int tableID, int clientID)
    : orderID_(nextOrderID++), tableID_(tableID), clientID_(clientID), isReady_(false) {
    std::cout << "[Order #" << orderID_ << "] Created for table "
        << tableID_ << ", client " << clientID_ << "\n";
}

//  онструктор копировани€
Order::Order(const Order& other)
    : orderID_(nextOrderID++),  // Ќовый уникальный ID!
    tableID_(other.tableID_),
    clientID_(other.clientID_),
    isReady_(other.isReady_) {

    std::cout << "[Order #" << orderID_ << "] Copied from order #"
        << other.orderID_ << "\n";

    //  опируем блюда (каждое блюдо нужно скопировать)
    for (const auto& dish : other.dishes_) {
        dishes_.push_back(std::make_shared<Dish>(*dish));
        dishReadyStatus_.push_back(false); // ¬се блюда начинаютс€ как "не готово"
    }
}

void Order::AddDish(std::shared_ptr<Dish> dish) {
    if (dish) {
        dishes_.push_back(dish);
        dishReadyStatus_.push_back(false);
        std::cout << "[Order #" << orderID_ << "] Added: " << dish->GetName() << "\n";
    }
}

void Order::MarkDishReady(int dishIndex) {
    if (dishIndex >= 0 && dishIndex < dishes_.size()) {
        dishReadyStatus_[dishIndex] = true;
        std::cout << "[Order #" << orderID_ << "] Dish ready: "
            << dishes_[dishIndex]->GetName() << "\n";

        // ѕровер€ем, готов ли весь заказ
        isReady_ = IsAllReady();
        if (isReady_) {
            std::cout << "[Order #" << orderID_ << "] COMPLETELY READY!\n";
        }
    }
}

bool Order::IsAllReady() const {
    for (bool ready : dishReadyStatus_) {
        if (!ready) return false;
    }
    return !dishReadyStatus_.empty();  // true только если есть блюда и все готовы
}

bool Order::IsDishReady(int dishIndex) const {
    if (dishIndex >= 0 && dishIndex < dishReadyStatus_.size()) {
        return dishReadyStatus_[dishIndex];
    }
    return false;
}

std::string Order::ToString() const {
    std::stringstream ss;
    ss << "Order #" << orderID_
        << " (Table: " << tableID_
        << ", Client: " << clientID_
        << ", Dishes: " << dishes_.size()
        << ", Ready: " << (isReady_ ? "Yes" : "No") << ")";
    return ss.str();
}

void Order::PrintOrder() const {
    std::cout << "\n=== Order #" << orderID_ << " ===\n";
    std::cout << "Table: " << tableID_ << "\n";
    std::cout << "Client: " << clientID_ << "\n";
    std::cout << "Status: " << (isReady_ ? "READY" : "PREPARING") << "\n";
    std::cout << "Dishes (" << dishes_.size() << "):\n";

    for (size_t i = 0; i < dishes_.size(); ++i) {
        const auto& dish = dishes_[i];
        std::cout << "  " << (i + 1) << ". " << dish->GetName()
            << " [" << dish->GetTypeString() << "]"
            << " (" << dish->GetCookingTime() << " ticks)"
            << " - " << (dishReadyStatus_[i] ? "READY" : "COOKING") << "\n";
    }
    std::cout << "=====================\n";
}

std::vector<std::shared_ptr<Dish>> Order::GetDishesByType(DishType type) const {
    std::vector<std::shared_ptr<Dish>> result;
    for (const auto& dish : dishes_) {
        if (dish->GetType() == type) {
            result.push_back(dish);
        }
    }
    return result;
}