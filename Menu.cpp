#include "Menu.h"
#include <iostream>
#include <random>
#include <algorithm>

// Инициализация статического члена
Menu* Menu::instance_ = nullptr;

Menu::Menu() : isInitialized_(false) {
    // Конструктор пустой, инициализация по требованию
}

Menu& Menu::GetInstance() {
    if (!instance_) {
        instance_ = new Menu();
    }
    // Инициализируем при первом обращении
    if (!instance_->isInitialized_) {
        instance_->Initialize();
    }
    return *instance_;
}

void Menu::Initialize() {
    if (isInitialized_) {
        std::cout << "[Menu] Already initialized with " << dishes_.size() << " dishes\n";
        return;
    }

    std::cout << "[Menu] Initializing...\n";
    isInitialized_ = true;

    int dishID = 1;

    // === 10 НАПИТКОВ ===
    dishes_.push_back(std::make_shared<Dish>(dishID++, "Coffee", DishType::BEVERAGE, 3));
    dishes_.push_back(std::make_shared<Dish>(dishID++, "Tea", DishType::BEVERAGE, 2));
    dishes_.push_back(std::make_shared<Dish>(dishID++, "Orange Juice", DishType::BEVERAGE, 2));
    dishes_.push_back(std::make_shared<Dish>(dishID++, "Coca-Cola", DishType::BEVERAGE, 1));
    dishes_.push_back(std::make_shared<Dish>(dishID++, "Latte", DishType::BEVERAGE, 4));
    dishes_.push_back(std::make_shared<Dish>(dishID++, "Espresso", DishType::BEVERAGE, 3));
    dishes_.push_back(std::make_shared<Dish>(dishID++, "Cappuccino", DishType::BEVERAGE, 4));
    dishes_.push_back(std::make_shared<Dish>(dishID++, "Milk", DishType::BEVERAGE, 1));
    dishes_.push_back(std::make_shared<Dish>(dishID++, "Hot Chocolate", DishType::BEVERAGE, 5));
    dishes_.push_back(std::make_shared<Dish>(dishID++, "Lemonade", DishType::BEVERAGE, 2));

    // === 8 СЭНДВИЧЕЙ ===
    dishes_.push_back(std::make_shared<Dish>(dishID++, "Club Sandwich", DishType::SANDWICH, 12));
    dishes_.push_back(std::make_shared<Dish>(dishID++, "Cheese Sandwich", DishType::SANDWICH, 10));
    dishes_.push_back(std::make_shared<Dish>(dishID++, "Chicken Sandwich", DishType::SANDWICH, 15));
    dishes_.push_back(std::make_shared<Dish>(dishID++, "Vegetarian Sandwich", DishType::SANDWICH, 8));
    dishes_.push_back(std::make_shared<Dish>(dishID++, "Ham Sandwich", DishType::SANDWICH, 10));
    dishes_.push_back(std::make_shared<Dish>(dishID++, "Tuna Sandwich", DishType::SANDWICH, 12));
    dishes_.push_back(std::make_shared<Dish>(dishID++, "Turkey Sandwich", DishType::SANDWICH, 13));
    dishes_.push_back(std::make_shared<Dish>(dishID++, "BLT Sandwich", DishType::SANDWICH, 11));

    // === 10 СЛАДКИХ БЛЮД ===
    dishes_.push_back(std::make_shared<Dish>(dishID++, "Chocolate Cake", DishType::SWEET, 20));
    dishes_.push_back(std::make_shared<Dish>(dishID++, "Ice Cream", DishType::SWEET, 5));
    dishes_.push_back(std::make_shared<Dish>(dishID++, "Apple Pie", DishType::SWEET, 15));
    dishes_.push_back(std::make_shared<Dish>(dishID++, "Pancakes", DishType::SWEET, 12));
    dishes_.push_back(std::make_shared<Dish>(dishID++, "Cheesecake", DishType::SWEET, 18));
    dishes_.push_back(std::make_shared<Dish>(dishID++, "Brownie", DishType::SWEET, 10));
    dishes_.push_back(std::make_shared<Dish>(dishID++, "Fruit Salad", DishType::SWEET, 8));
    dishes_.push_back(std::make_shared<Dish>(dishID++, "Tiramisu", DishType::SWEET, 22));
    dishes_.push_back(std::make_shared<Dish>(dishID++, "Chocolate Mousse", DishType::SWEET, 14));
    dishes_.push_back(std::make_shared<Dish>(dishID++, "Creme Brulee", DishType::SWEET, 16));

    std::cout << "[Menu] Initialized with " << dishes_.size() << " dishes\n";
    std::cout << "[Menu] Dish IDs: 1 to " << dishID - 1 << " (all sequential)\n";
}

std::shared_ptr<Dish> Menu::GetDishByID(int id) const {
    for (const auto& dish : dishes_) {
        if (dish->GetID() == id) {
            return dish;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<Dish>> Menu::GetDishesByType(DishType type) const {
    std::vector<std::shared_ptr<Dish>> result;
    for (const auto& dish : dishes_) {
        if (dish->GetType() == type) {
            result.push_back(dish);
        }
    }
    return result;
}

std::shared_ptr<Dish> Menu::GetRandomDishByType(DishType type) const {
    auto dishesOfType = GetDishesByType(type);
    if (dishesOfType.empty()) return nullptr;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, dishesOfType.size() - 1);

    return dishesOfType[dis(gen)];
}

std::vector<std::shared_ptr<Dish>> Menu::CreateRandomOrder() const {
    std::vector<std::shared_ptr<Dish>> order;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> numDishesDist(1, 4);
    std::uniform_int_distribution<> dishTypeDist(0, 2);

    int numDishes = numDishesDist(gen);

    for (int i = 0; i < numDishes; ++i) {
        DishType type = static_cast<DishType>(dishTypeDist(gen));
        auto dish = GetRandomDishByType(type);
        if (dish) {
            order.push_back(dish);
        }
    }

    return order;
}

void Menu::PrintMenu() const {
    std::cout << "\n=== RESTAURANT MENU ===\n";

    std::cout << "\nBEVERAGES (10):\n";
    for (const auto& dish : dishes_) {
        if (dish->GetType() == DishType::BEVERAGE) {
            std::cout << "  " << dish->GetID() << ". " << dish->GetName()
                << " (" << dish->GetCookingTime() << " ticks)\n";
        }
    }

    std::cout << "\nSANDWICHES (8):\n";
    for (const auto& dish : dishes_) {
        if (dish->GetType() == DishType::SANDWICH) {
            std::cout << "  " << dish->GetID() << ". " << dish->GetName()
                << " (" << dish->GetCookingTime() << " ticks)\n";
        }
    }

    std::cout << "\nSWEET DISHES (10):\n";
    for (const auto& dish : dishes_) {
        if (dish->GetType() == DishType::SWEET) {
            std::cout << "  " << dish->GetID() << ". " << dish->GetName()
                << " (" << dish->GetCookingTime() << " ticks)\n";
        }
    }

    std::cout << "========================\n";
}