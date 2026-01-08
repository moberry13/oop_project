#ifndef ORDER_H
#define ORDER_H

#include "Dish.h"
#include <vector>
#include <memory>
#include <string>

class Order {
private:
    static int nextOrderID;

    int orderID_;
    int tableID_;
    int clientID_;
    std::vector<std::shared_ptr<Dish>> dishes_;
    std::vector<bool> dishReadyStatus_;
    bool isReady_;

public:
    Order(int tableID, int clientID);
    Order(const Order& other);

    // Геттеры
    int GetID() const { return orderID_; }
    int GetTableID() const { return tableID_; }
    int GetClientID() const { return clientID_; }
    bool IsReady() const { return isReady_; }

    // Управление блюдами
    void AddDish(std::shared_ptr<Dish> dish);
    const std::vector<std::shared_ptr<Dish>>& GetDishes() const { return dishes_; }
    int GetDishCount() const { return dishes_.size(); }

    // Управление готовностью
    void MarkDishReady(int dishIndex);
    bool IsAllReady() const;
    bool IsDishReady(int dishIndex) const;

    // Информация
    std::string ToString() const;
    void PrintOrder() const;

    // Поиск блюд по типу
    std::vector<std::shared_ptr<Dish>> GetDishesByType(DishType type) const;
};

#endif // ORDER_H