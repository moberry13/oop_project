#include "Waiter.h"
#include "MSG.h"
#include "Manager.h"
#include <iostream>

Waiter::Waiter(int id)
    : Human("Waiter_" + std::to_string(id), 3),
    waiterID_(id), isBusy_(false), currentTableID_(-1),
    currentState_(WaiterState::IDLE), actionTimer_(0) {
    std::cout << "[Waiter #" << waiterID_ << "] Created\n";
}

std::string Waiter::GetStateString() const {
    return StateToString(currentState_);
}

void Waiter::AssignToTable(int tableID) {
    if (currentState_ == WaiterState::IDLE) {
        currentTableID_ = tableID;
        std::cout << "[Waiter #" << waiterID_
            << "] Assigned to table #" << tableID << "\n";

        // Начинаем идти к столу (симулируем 1 тик)
        SetState(WaiterState::GOING_TO_TABLE, 1);
    }
    else {
        std::cout << "[Waiter #" << waiterID_
            << "] Can't assign to table #" << tableID
            << " - current state: " << StateToString(currentState_) << "\n";
    }
}

void Waiter::TakeOrder(std::shared_ptr<Order> order) {
    if (!order) {
        std::cout << "[Waiter #" << waiterID_ << "] Can't take null order\n";
        return;
    }

    if (currentState_ != WaiterState::IDLE &&
        currentState_ != WaiterState::GOING_TO_TABLE) {
        std::cout << "[Waiter #" << waiterID_
            << "] Can't take order - current state: "
            << StateToString(currentState_) << "\n";
        return;
    }

    currentOrder_ = order;
    currentTableID_ = order->GetTableID();

    std::cout << "[Waiter #" << waiterID_
        << "] Took order #" << order->GetID()
        << " from table #" << currentTableID_ << "\n";

    // Идет на кухню (симулируем 1 тик)
    SetState(WaiterState::GOING_TO_KITCHEN, 1);
}

void Waiter::DeliverOrder() {
    if (!currentOrder_) {
        std::cout << "[Waiter #" << waiterID_ << "] No order to deliver\n";
        return;
    }

    std::cout << "[Waiter #" << waiterID_
        << "] Delivering order #" << currentOrder_->GetID()
        << " to table #" << currentTableID_ << "\n";

    // Несет заказ клиенту (симулируем 1 тик)
    SetState(WaiterState::DELIVERING, 1);
}

void Waiter::CleanTable(int tableID) {
    std::cout << "[Waiter #" << waiterID_
        << "] Cleaning table #" << tableID << "\n";

    currentTableID_ = tableID;
    SetState(WaiterState::CLEANING, 2);  // Уборка занимает 2 тика
}

void Waiter::CompleteCurrentTask() {
    switch (currentState_) {
    case WaiterState::GOING_TO_TABLE: {
        // Достиг стола, уведомляем клиента
        MSG msg = MSGHelper::CreateWithInt(
            MSGType::Waiter_At_Table,
            this,
            currentTableID_
        );
        Manager::GetInstance().Send(msg);
        std::cout << "[Waiter #" << waiterID_
            << "] Reached table #" << currentTableID_
            << ", notified client\n";
        break;
    }

    case WaiterState::GOING_TO_KITCHEN: {
        // Достиг кухни, отправляет заказ
        SendOrderToKitchen();
        break;
    }

    case WaiterState::DELIVERING: {
        // Достиг стола, отдает заказ
        MSG msg = MSGHelper::CreateWithInt(
            MSGType::Order_Delivered,
            this,
            currentTableID_
        );
        Manager::GetInstance().Send(msg);
        std::cout << "[Waiter #" << waiterID_
            << "] Delivered order #" << currentOrder_->GetID()
            << " to table #" << currentTableID_ << "\n";

        // Освобождаем заказ
        currentOrder_.reset();
        break;
    }

    case WaiterState::CLEANING: {
        // Завершил уборку
        MSG msg = MSGHelper::CreateWithInt(
            MSGType::Table_Cleaned,
            this,
            currentTableID_
        );
        Manager::GetInstance().Send(msg);
        std::cout << "[Waiter #" << waiterID_
            << "] Finished cleaning table #" << currentTableID_ << "\n";
        break;
    }

    default:
        break;
    }

    // Возвращаемся в свободное состояние
    SetState(WaiterState::IDLE);
    ProcessNextCleaning();
}

void Waiter::SendOrderToKitchen() {
    if (!currentOrder_) {
        std::cout << "[Waiter #" << waiterID_ << "] No order to send to kitchen\n";
        return;
    }

    std::cout << "[Waiter #" << waiterID_
        << "] Sending order #" << currentOrder_->GetID()
        << " to kitchen\n";

    auto orderCopy = std::make_shared<Order>(*currentOrder_);
    MSG msg(MSGType::Order_To_Kitchen, this, orderCopy);

    Manager::GetInstance().Send(msg);

    std::cout << "[Waiter #" << waiterID_
        << "] Order #" << currentOrder_->GetID()
        << " sent to kitchen\n";

    // После отправки заказа можем взять новую задачу
    SetState(WaiterState::IDLE);
}

void Waiter::ProcessNextCleaning() {
    if (!tablesToClean_.empty() && currentState_ == WaiterState::IDLE) {
        int tableID = tablesToClean_.front();
        tablesToClean_.pop();

        std::cout << "[Waiter #" << waiterID_
            << "] Starting cleaning for table #" << tableID
            << " from queue\n";

        // Начинаем уборку
        CleanTable(tableID);
    }
}

void Waiter::SetState(WaiterState newState, int timer) {
    std::cout << "[Waiter #" << waiterID_
        << "] State: " << StateToString(currentState_)
        << " -> " << StateToString(newState) << "\n";

    currentState_ = newState;
    isBusy_ = (newState != WaiterState::IDLE);
    actionTimer_ = timer;
}

std::string Waiter::StateToString(WaiterState state) const {
    switch (state) {
    case WaiterState::IDLE: return "IDLE";
    case WaiterState::GOING_TO_TABLE: return "GOING_TO_TABLE";
    case WaiterState::TAKING_ORDER: return "TAKING_ORDER";
    case WaiterState::GOING_TO_KITCHEN: return "GOING_TO_KITCHEN";
    case WaiterState::DELIVERING: return "DELIVERING";
    case WaiterState::CLEANING: return "CLEANING";
    default: return "UNKNOWN";
    }
}

void Waiter::PerformAction() {
    std::cout << "[Waiter #" << waiterID_
        << "] Action: " << StateToString(currentState_)
        << ", Timer: " << actionTimer_ << "\n";
}

void Waiter::Update() {
    // Обновляем положение
    Human::UpdatePosition();

    // Обновляем таймер текущего действия
    if (currentState_ != WaiterState::IDLE && actionTimer_ > 0) {
        actionTimer_--;

        if (actionTimer_ == 0) {
            // Действие завершено
            CompleteCurrentTask();
        }
    }
}

// ... остальной код Waiter.cpp остаётся тем же ...

void Waiter::OnMsg(const MSG& msg) {
    switch (msg.type) {
    case MSGType::Waiter_Assigned: {
        int tableID = MSGHelper::GetIntFromPayload(msg);
        std::cout << "[Waiter #" << waiterID_
            << "] Received assignment to table #" << tableID << "\n";
        AssignToTable(tableID);
        break;
    }

    case MSGType::Client_Order_Choice: {
        std::cout << "[Waiter #" << waiterID_
            << "] Received order from client\n";

        // ★★★★ ИСПРАВЛЕНИЕ: используем безопасное извлечение shared_ptr ★★★★
        auto orderPtr = MSGHelper::GetPayloadAs<Order>(msg);
        if (orderPtr) {
            std::cout << "[Waiter #" << waiterID_
                << "] Order #" << orderPtr->GetID() << " details:\n";
            orderPtr->PrintOrder();

            // Берем заказ, если свободен или идет к столу
            if (currentState_ == WaiterState::IDLE ||
                currentState_ == WaiterState::GOING_TO_TABLE) {
                // Создаём копию заказа для официанта
                currentOrder_ = std::make_shared<Order>(*orderPtr);
                TakeOrder(currentOrder_);
            }
            else {
                std::cout << "[Waiter #" << waiterID_
                    << "] Can't take order now - state: "
                    << StateToString(currentState_) << "\n";
            }
        }
        else {
            std::cout << "[Waiter #" << waiterID_
                << "] Received null order\n";
        }
        break;
    }

    case MSGType::Order_Ready: {
        // ★★★★ ИСПРАВЛЕНИЕ: используем безопасное извлечение shared_ptr ★★★★
        auto orderPtr = MSGHelper::GetPayloadAs<Order>(msg);
        if (orderPtr) {
            std::cout << "[Waiter #" << waiterID_
                << "] Received notification: order #"
                << orderPtr->GetID() << " is ready\n";

            if (currentState_ == WaiterState::IDLE) {
                // Принимаем заказ для доставки (создаём копию)
                currentOrder_ = std::make_shared<Order>(*orderPtr);
                currentTableID_ = orderPtr->GetTableID();
                DeliverOrder();
            }
            else {
                std::cout << "[Waiter #" << waiterID_
                    << "] Can't deliver now - state: "
                    << StateToString(currentState_) << "\n";
            }
        }
        break;
    }

    case MSGType::Table_Dirty: {
        int tableID = MSGHelper::GetIntFromPayload(msg);
        std::cout << "[Waiter #" << waiterID_
            << "] Received cleaning request for table #" << tableID << "\n";
        if (currentState_ == WaiterState::IDLE) {
            // Если свободен, начинаем уборку
            CleanTable(tableID);
        }
        else {
            // Если занят, ставим в очередь
            tablesToClean_.push(tableID);
            std::cout << "[Waiter #" << waiterID_
                << "] Added table #" << tableID << " to cleaning queue\n";
        }
        break;
    }

    default:
        break;
    }
}