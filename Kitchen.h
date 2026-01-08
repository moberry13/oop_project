#ifndef KITCHEN_H
#define KITCHEN_H

#include "Object.h"
#include "Order.h"
#include "Cook.h"
#include <memory>
#include <vector>
#include <queue>           // ← ДОБАВИТЬ ЭТО!
#include <unordered_map>

class Kitchen : public Object {
private:
    std::vector<std::shared_ptr<Cook>> cooks_;
    std::unordered_map<int, std::shared_ptr<Order>> activeOrders_;  // orderID -> Order
    std::unordered_map<int, std::vector<int>> orderToDishes_;       // orderID -> dish indices
    std::queue<std::shared_ptr<Order>> pendingOrders_;              // ← ТЕПЕРЬ queue виден

public:
    Kitchen();

    // Управление поварами
    void AddCook(std::shared_ptr<Cook> cook);
    Cook* FindAvailableCook(DishType dishType) const;

    // Обработка заказов
    void ReceiveOrder(std::shared_ptr<Order> order);
    void DistributeOrder(std::shared_ptr<Order> order);
    void AssignDishToCook(std::shared_ptr<Dish> dish);

    // Отслеживание готовности
    void OnDishReady(std::shared_ptr<Dish> dish);
    void CheckOrderReady(int orderID);
    void NotifyOrderReady(std::shared_ptr<Order> order);

    // Виртуальные методы Object
    void Update() override;
    void OnMsg(const MSG& msg) override;

    // Вспомогательные методы
    void PrintStatus() const;
    size_t GetCookCount() const { return cooks_.size(); }
    size_t GetActiveOrderCount() const { return activeOrders_.size(); }
    size_t GetPendingOrderCount() const { return pendingOrders_.size(); }
};

#endif // KITCHEN_H