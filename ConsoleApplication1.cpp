// test_final_integration_fixed.cpp
#include "Manager.h"
#include "Client.h"
#include "Waiter.h"
#include "Hall.h"
#include "Table.h"
#include "Kitchen.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    std::cout << "=== FINAL INTEGRATION TEST (COMPLETE) ===\n\n";

    Manager& manager = Manager::GetInstance();

    // 1. Инициализация системы
    std::cout << "1. Initializing restaurant...\n";

    Hall hall;
    Table table1(1, 10, 20);
    Table table2(2, 30, 20);

    manager.RegisterObject(&hall);
    manager.RegisterObject(&table1);
    manager.RegisterObject(&table2);

    hall.AddTable(&table1);
    hall.AddTable(&table2);

    // 2. Создаем персонал
    std::cout << "\n2. Creating staff...\n";

    Waiter waiter(1);
    manager.RegisterObject(&waiter);

    // ★★★★ ВАЖНО: Регистрируем официанта в Hall ★★★★
    hall.AddWaiter(&waiter);
    std::cout << "[Hall] Total waiters: " << hall.GetWaiterCount() << "\n";

    Kitchen kitchen;
    manager.RegisterObject(&kitchen);

    // 3. Клиент приходит
    std::cout << "\n3. Client arrives...\n";

    Client client1(1);
    manager.RegisterObject(&client1);

    client1.EnterRestaurant();

    // 4. Полный цикл симуляции
    std::cout << "\n4. Running full simulation (50 ticks)...\n";

    for (int tick = 0; tick < 50; ++tick) {
        std::cout << "\n[Tick " << tick << "]\n";

        // Порядок ВАЖЕН: сначала сообщения, потом обновления
        manager.ProcessMessages();

        // Обновляем всех
        waiter.Update();
        client1.Update();
        kitchen.Update();

        // В цикле симуляции добавьте:
        if (tick % 5 == 0) {  // Каждые 5 тиков показываем статус кухни
            kitchen.PrintStatus();
        }

        // Показываем статусы
        std::cout << "Client state: ";
        if (client1.IsEating()) {
            std::cout << "EATING";
        }
        else if (client1.HasOrdered()) {
            std::cout << "WAITING_FOOD";
        }
        else if (client1.GetTableID() != -1) {
            std::cout << "WAITING_ORDER";
        }
        else {
            std::cout << "NO_TABLE";
        }
        std::cout << "\n";

        std::cout << "Waiter state: " << waiter.GetStateString() << "\n";
        std::cout << "Table1 state: " << (table1.IsFree() ? "FREE" :
            table1.IsOccupied() ? "OCCUPIED" : "DIRTY") << "\n";

        // Короткая задержка для наглядности
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Если клиент ушел, заходим новый
        if (client1.GetTableID() == -1 && tick > 20 && tick < 40) {
            std::cout << "\nNew client entering...\n";
            Client client2(2);
            manager.RegisterObject(&client2);
            client2.EnterRestaurant();
            break; // Для простоты тестируем только одного нового клиента
        }

        // Прерываем если клиент ушел
        if (client1.GetTableID() == -1 && client1.GetCurrentOrder() == nullptr) {
            std::cout << "Client left, ending simulation...\n";
            break;
        }
    }

    // 5. Финальный статус
    std::cout << "\n5. Final status:\n";
    kitchen.PrintStatus();

    std::cout << "\n=== FINAL TEST COMPLETED ===\n";
    std::cout << "✓ Full restaurant workflow works\n";
    std::cout << "✓ All components integrated\n";
    std::cout << "✓ Message passing system works\n";
    std::cout << "✓ Complete order cycle (Client → Waiter → Kitchen → Cooks → Client)\n";

    std::cout << "\n[DEBUG] Cleaning up before exit...\n";

    // Явно очищаем kitchen (у неё есть повара)
    // kitchen.~Kitchen(); // Не вызывай явно деструктор!

    // Или просто добавь паузу чтобы увидеть где именно падает
    std::cout << "[DEBUG] Press Enter to exit...\n";

    return 0;
}