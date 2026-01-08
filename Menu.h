#ifndef MENU_H
#define MENU_H

#include "Dish.h"
#include <vector>
#include <memory>

class Menu {
private:
    static Menu* instance_;
    std::vector<std::shared_ptr<Dish>> dishes_;
    bool isInitialized_;

    // Приватный конструктор для синглтона
    Menu();

public:
    // Запрещаем копирование
    Menu(const Menu&) = delete;
    Menu& operator=(const Menu&) = delete;

    // Получение экземпляра
    static Menu& GetInstance();

    // Инициализация меню (только если не инициализировано)
    void Initialize();

    // Получение блюд
    std::shared_ptr<Dish> GetDishByID(int id) const;
    std::vector<std::shared_ptr<Dish>> GetDishesByType(DishType type) const;
    std::shared_ptr<Dish> GetRandomDishByType(DishType type) const;

    // Создание случайного набора блюд (1-4 шт)
    std::vector<std::shared_ptr<Dish>> CreateRandomOrder() const;

    // Информация
    size_t GetDishCount() const { return dishes_.size(); }
    bool IsInitialized() const { return isInitialized_; }
    void PrintMenu() const;
};

#endif // MENU_H