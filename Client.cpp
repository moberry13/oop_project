#include "Client.h"aqqqq
#include "MSG.h"
#include "Manager.h"
#include <iostream>
#include <random>

Client::Client(int id)
    : Human("Client_" + std::to_string(id), 2),
    clientID_(id), tableID_(-1), hasOrdered_(false),
    isEating_(false), eatingTimer_(0), patienceTimer_(50) {
    std::cout << "[Client #" << clientID_ << "] Created\n";
}

void Client::EnterRestaurant() {
    std::cout << "[Client #" << clientID_ << "] Entering restaurant\n";

    // Отправляем сообщение Hall'у
    MSG msg(MSGType::Client_Entered, this);
    Manager::GetInstance().Send(msg);
}

void Client::ChooseDishes() {
    std::cout << "[Client #" << clientID_ << "] Looking at menu...\n";

    // Создаем заказ
    currentOrder_ = CreateOrder();
    if (currentOrder_) {
        hasOrdered_ = true;
        std::cout << "[Client #" << clientID_ << "] Created order #"
            << currentOrder_->GetID() << " with "
            << currentOrder_->GetDishCount() << " dishes\n";

        // Отправляем заказ официанту
        SendOrderToWaiter();
    }
}

std::shared_ptr<Order> Client::CreateOrder() {
    if (tableID_ == -1) {
        std::cout << "[Client #" << clientID_ << "] Can't order: no table assigned\n";
        return nullptr;
    }

    // Создаем заказ для этого стола и клиента
    auto order = std::make_shared<Order>(tableID_, clientID_);

    // Получаем случайные блюда из Menu
    Menu& menu = Menu::GetInstance();
    auto randomDishes = menu.CreateRandomOrder();

    // Добавляем блюда в заказ
    for (const auto& dish : randomDishes) {
        order->AddDish(dish);
    }

    return order;
}

void Client::SendOrderToWaiter() {
    if (!currentOrder_) return;

    std::cout << "[Client #" << clientID_ << "] Sending order to waiter\n";

    // ★★★★ ИСПРАВЛЕНИЕ: передаём shared_ptr с копией заказа ★★★★
    auto orderCopy = std::make_shared<Order>(*currentOrder_);
    MSG msg(MSGType::Client_Order_Choice, this, orderCopy);
    Manager::GetInstance().Send(msg);
}

void Client::StartEating(int eatingTime) {
    isEating_ = true;
    eatingTimer_ = eatingTime;
    std::cout << "[Client #" << clientID_ << "] Started eating for "
        << eatingTimer_ << " ticks\n";
}

void Client::Leave() {
    std::cout << "[Client #" << clientID_ << "] Leaving restaurant\n";

    if (tableID_ != -1) {
        // Уведомляем Hall, что стол освободился (но грязный)
        MSG msg = MSGHelper::CreateWithInt(
            MSGType::Client_Finished_Eating,
            this,
            tableID_
        );
        Manager::GetInstance().Send(msg);
    }

    // Сбрасываем состояние
    tableID_ = -1;
    currentOrder_.reset();
    hasOrdered_ = false;
    isEating_ = false;
}

void Client::PerformAction() {
    // Клиент может выполнять различные действия в зависимости от состояния
    if (!hasOrdered_ && tableID_ != -1) {
        std::cout << "[Client #" << clientID_ << "] Is ready to order\n";
    }
    else if (isEating_) {
        std::cout << "[Client #" << clientID_ << "] Is eating\n";
    }
    else {
        std::cout << "[Client #" << clientID_ << "] Is waiting\n";
    }
}

void Client::Update() {
    // Обновляем положение (наследуем от Human)
    Human::UpdatePosition();

    // Обновляем таймеры
    if (isEating_ && eatingTimer_ > 0) {
        eatingTimer_--;
        if (eatingTimer_ == 0) {
            isEating_ = false;
            std::cout << "[Client #" << clientID_ << "] Finished eating\n";
            Leave();
        }
    }

    // Таймер терпения (если ждет слишком долго)
    if (!hasOrdered_ && tableID_ != -1 && patienceTimer_ > 0) {
        patienceTimer_--;
        if (patienceTimer_ == 0) {
            std::cout << "[Client #" << clientID_ << "] Lost patience, leaving\n";
            Leave();
        }
    }
}

void Client::OnMsg(const MSG& msg) {
    switch (msg.type) {
    case MSGType::Table_Assigned: {
        tableID_ = MSGHelper::GetIntFromPayload(msg);
        std::cout << "[Client #" << clientID_
            << "] Assigned to table #" << tableID_ << "\n";

        // Через 2 тика начинает выбирать блюда
        // В реальности - через таймер, но для простоты:
        patienceTimer_ = 30;  // Даем 30 тиков на заказ
        break;
    }

    case MSGType::Waiter_At_Table: {
        int msgTableID = MSGHelper::GetIntFromPayload(msg);
        if (msgTableID == tableID_ && !hasOrdered_) {
            std::cout << "[Client #" << clientID_
                << "] Waiter arrived, choosing dishes...\n";
            ChooseDishes();
        }
        break;
    }

    case MSGType::Order_Delivered: {
        if (!isEating_) {
            std::cout << "[Client #" << clientID_
                << "] Received order, starting to eat\n";

            // Рандомное время еды: 20-40 тиков
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_int_distribution<> eatingTimeDist(20, 40);

            StartEating(eatingTimeDist(gen));
        }
        break;
    }

    default:
        break;
    }
}