#ifndef DISH_H
#define DISH_H

#include <string>

// Типы блюд
enum class DishType {
    BEVERAGE,   // Напитки (кофе, чай, сок)
    SANDWICH,   // Сэндвичи (несладкие)
    SWEET       // Сладкие блюда (десерты)
};

class Dish {
private:
    int id_;
    std::string name_;
    DishType type_;
    int cookingTime_;  // Время приготовления в тиках

public:
    Dish(int id, const std::string& name, DishType type, int cookingTime);

    // Геттеры
    int GetID() const { return id_; }
    std::string GetName() const { return name_; }
    DishType GetType() const { return type_; }
    int GetCookingTime() const { return cookingTime_; }

    // Вспомогательные методы
    std::string GetTypeString() const;

    // Статические методы для создания типовых блюд
    static Dish* CreateBeverage(int id, const std::string& name, int time = 3);
    static Dish* CreateSandwich(int id, const std::string& name, int time = 10);
    static Dish* CreateSweet(int id, const std::string& name, int time = 15);
};

#endif // DISH_H