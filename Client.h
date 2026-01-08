#ifndef CLIENT_H
#define CLIENT_H

#include "Human.h"
#include "Order.h"
#include "Menu.h"
#include <memory>
#include <vector>

class Client : public Human {
private:
    int clientID_;
    int tableID_;
    std::shared_ptr<Order> currentOrder_;
    bool hasOrdered_;
    bool isEating_;
    int eatingTimer_;
    int patienceTimer_;  // Таймер терпения (сколько ждет)

public:
    Client(int id);

    // Основные действия
    void EnterRestaurant();
    void ChooseDishes();
    void StartEating(int eatingTime);
    void Leave();

    // Геттеры
    int GetClientID() const { return clientID_; }
    int GetTableID() const { return tableID_; }
    bool HasOrdered() const { return hasOrdered_; }
    bool IsEating() const { return isEating_; }
    std::shared_ptr<Order> GetCurrentOrder() const { return currentOrder_; }

    // Создание заказа
    std::shared_ptr<Order> CreateOrder();

    // Виртуальные методы Human
    void PerformAction() override;
    void Update() override;
    void OnMsg(const MSG& msg) override;

private:
    void SendOrderToWaiter();
};

#endif // CLIENT_H