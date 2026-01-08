#include "Kitchen.h"
#include "MSG.h"
#include "Manager.h"
#include "DrinkCook.h"
#include "SweetCook.h"
#include "SavoryCook.h"
#include <iostream>

Kitchen::Kitchen() {
    std::cout << "[Kitchen] Created\n";

    // Автоматически создаем поваров при создании кухни
    AddCook(std::make_shared<DrinkCook>(1));
    AddCook(std::make_shared<SweetCook>(2));
    AddCook(std::make_shared<SavoryCook>(3));
}

void Kitchen::AddCook(std::shared_ptr<Cook> cook) {
    if (cook) {
        cooks_.push_back(cook);
        Manager::GetInstance().RegisterObject(cook.get());
        std::cout << "[Kitchen] Cook #" << cook->GetID()
            << " (" << cook->GetSpecializationString() << ") added\n";
    }
}

Cook* Kitchen::FindAvailableCook(DishType dishType) const {
    for (const auto& cook : cooks_) {
        if (cook->GetSpecialization() == dishType && cook->IsAvailable()) {
            return cook.get();
        }
    }
    return nullptr;
}

void Kitchen::ReceiveOrder(std::shared_ptr<Order> order) {
    if (!order) return;

    std::cout << "[Kitchen] Received order #" << order->GetID()
        << " for table #" << order->GetTableID() << "\n";

    pendingOrders_.push(order);
}

void Kitchen::DistributeOrder(std::shared_ptr<Order> order) {
    if (!order) return;

    std::cout << "[Kitchen] Distributing order #" << order->GetID() << "\n";
    order->PrintOrder();

    // Добавляем заказ в активные
    activeOrders_[order->GetID()] = order;

    // Распределяем каждое блюдо
    const auto& dishes = order->GetDishes();
    for (size_t i = 0; i < dishes.size(); ++i) {
        // ★★★★ ИСПРАВЛЕНИЕ: создаём копию блюда для передачи повару ★★★★
        auto dishCopy = std::make_shared<Dish>(*(dishes[i]));
        AssignDishToCook(dishCopy);

        // Сохраняем индекс блюда в заказе
        orderToDishes_[order->GetID()].push_back(static_cast<int>(i));
    }
}

void Kitchen::AssignDishToCook(std::shared_ptr<Dish> dish) {
    if (!dish) return;

    Cook* availableCook = FindAvailableCook(dish->GetType());

    if (availableCook) {
        std::cout << "[Kitchen] Assigning " << dish->GetName()
            << " to Cook #" << availableCook->GetID() << "\n";

        // ★★★★ ИСПРАВЛЕНИЕ: передаём shared_ptr с копией блюда ★★★★
        auto dishForCook = std::make_shared<Dish>(*dish);
        MSG msg(MSGType::Prepare_Dish, this, dishForCook);
        Manager::GetInstance().Send(msg);
    }
    else {
        std::cout << "[Kitchen] No available cook for "
            << dish->GetName() << " [" << dish->GetTypeString() << "]\n";
    }
}

void Kitchen::OnDishReady(std::shared_ptr<Dish> dish) {
    if (!dish) return;

    std::cout << "[Kitchen] Dish ready: " << dish->GetName() << "\n";

    // ★★★★ НУЖНО ДОРАБОТАТЬ: найти заказ и обновить статус ★★★★
    // Пока просто выводим сообщение

    // TODO: 1. Найти заказ, содержащий это блюдо
    // TODO: 2. Обновить order->MarkDishReady(index)
    // TODO: 3. Проверить order->IsAllReady()
    // TODO: 4. Если готов, отправить Order_Ready

    // Временное решение: каждый раз при готовности блюда проверяем все заказы
    for (auto& pair : activeOrders_) {
        auto order = pair.second;
        // Просто для теста: если заказ содержит одно блюдо и оно того же типа
        if (order->GetDishCount() == 1) {
            std::cout << "[Kitchen] Order #" << order->GetID()
                << " has 1 dish, assuming it's ready\n";
            NotifyOrderReady(order);
            activeOrders_.erase(pair.first);
            orderToDishes_.erase(pair.first);
            break;
        }
    }
}
void Kitchen::CheckOrderReady(int orderID) {
    auto it = activeOrders_.find(orderID);
    if (it == activeOrders_.end()) return;

    auto order = it->second;
    if (order->IsAllReady()) {
        std::cout << "[Kitchen] Order #" << orderID << " is COMPLETELY READY!\n";
        NotifyOrderReady(order);

        // Удаляем из активных заказов
        activeOrders_.erase(orderID);
        orderToDishes_.erase(orderID);
    }
}

void Kitchen::NotifyOrderReady(std::shared_ptr<Order> order) {
    if (!order) return;

    std::cout << "[Kitchen] Notifying waiter that order #"
        << order->GetID() << " is ready\n";

    // ★★★★ ИСПРАВЛЕНИЕ: передаём shared_ptr с копией заказа ★★★★
    auto orderCopy = std::make_shared<Order>(*order);
    MSG msg(MSGType::Order_Ready, this, orderCopy);
    Manager::GetInstance().Send(msg);
}

void Kitchen::Update() {
    // Обрабатываем ожидающие заказы
    while (!pendingOrders_.empty()) {
        auto order = pendingOrders_.front();
        pendingOrders_.pop();
        DistributeOrder(order);
    }

    // ★★★★ ОБНОВЛЯЕМ ВСЕХ ПОВАРОВ ★★★★
    for (auto& cook : cooks_) {
        cook->Update();
    }

    // Проверяем готовность всех активных заказов
    for (const auto& pair : activeOrders_) {
        CheckOrderReady(pair.first);
    }
}

void Kitchen::OnMsg(const MSG& msg) {
    switch (msg.type) {
    case MSGType::Order_To_Kitchen: {
        // ★★★★ ИСПРАВЛЕНИЕ: используем безопасное извлечение shared_ptr ★★★★
        auto orderPtr = MSGHelper::GetPayloadAs<Order>(msg);
        if (orderPtr) {
            // Создаём копию заказа для кухни
            auto orderCopy = std::make_shared<Order>(*orderPtr);
            ReceiveOrder(orderCopy);
        }
        break;
    }

    case MSGType::Dish_Ready: {
        // ★★★★ ИСПРАВЛЕНИЕ: используем безопасное извлечение shared_ptr ★★★★
        auto dishPtr = MSGHelper::GetPayloadAs<Dish>(msg);
        if (dishPtr) {
            OnDishReady(dishPtr);
        }
        break;
    }

    default:
        break;
    }
}

void Kitchen::PrintStatus() const {
    std::cout << "\n=== KITCHEN STATUS ===\n";
    std::cout << "Cooks: " << cooks_.size() << "\n";
    std::cout << "Active orders: " << activeOrders_.size() << "\n";
    std::cout << "Pending orders: " << pendingOrders_.size() << "\n";

    for (const auto& cook : cooks_) {
        std::cout << "\nCook #" << cook->GetID()
            << " (" << cook->GetSpecializationString() << "): "
            << cook->GetCurrentDishCount() << "/5 dishes"
            << " - Available: " << (cook->IsAvailable() ? "Yes" : "No") << "\n";
    }
    std::cout << "======================\n";
}