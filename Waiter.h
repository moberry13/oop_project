#ifndef WAITER_H
#define WAITER_H

#include "Human.h"
#include "Order.h"
#include <memory>
#include <queue>
#include <string>  // ДОБАВЬТЕ ЭТУ СТРОКУ!

class Waiter : public Human {
private:
    int waiterID_;
    bool isBusy_;  // Занят физическим перемещением/действием
    int currentTableID_;
    std::shared_ptr<Order> currentOrder_;
    std::queue<int> tablesToClean_;

    // Состояния для отслеживания задач
    enum class WaiterState {
        IDLE,           // Свободен, ждет задание
        GOING_TO_TABLE, // Идет к столу
        TAKING_ORDER,   // Принимает заказ
        GOING_TO_KITCHEN, // Идет на кухню
        DELIVERING,     // Несет заказ клиенту
        CLEANING        // Убирает стол
    };

    WaiterState currentState_;
    int actionTimer_;  // Таймер для текущего действия

public:
    Waiter(int id);

    // Геттеры
    int GetID() const { return waiterID_; }
    bool IsBusy() const { return isBusy_; }
    int GetCurrentTable() const { return currentTableID_; }
    WaiterState GetState() const { return currentState_; }
    std::string GetStateString() const;  // ДОБАВЬТЕ ЭТУ СТРОКУ!

    // Основные действия
    void AssignToTable(int tableID);
    void TakeOrder(std::shared_ptr<Order> order);
    void DeliverOrder();
    void CleanTable(int tableID);
    void CompleteCurrentTask();

    // Виртуальные методы Human
    void PerformAction() override;
    void Update() override;
    void OnMsg(const MSG& msg) override;

private:
    void SendOrderToKitchen();
    void ProcessNextCleaning();
    void SetState(WaiterState newState, int timer = 1);
    std::string StateToString(WaiterState state) const;
};

#endif // WAITER_H