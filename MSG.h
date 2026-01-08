#ifndef MSG_H
#define MSG_H

#include <memory>
#include <cstddef>

// Все типы сообщений для ресторанной симуляции
enum class MSGType {
    // ========== КЛИЕНТ ==========
    Client_Entered,          // Клиент вошел в ресторан
    Client_Ready_To_Order,   // Клиент готов заказывать
    Client_Order_Choice,     // Клиент сделал выбор блюд
    Client_Finished_Eating,  // Клиент закончил есть
    Client_Leaving,          // Клиент уходит

    // ========== СТОЛЫ И ЗАЛ ==========
    Table_Assigned,          // Стол назначен клиенту
    Table_Occupied,          // Стол занят
    Table_Dirty,            // Стол грязный (нужна уборка)
    Table_Cleaned,          // Стол убран

    // ========== ОФИЦИАНТ ==========
    Waiter_Assigned,        // Официант назначен к столу
    Waiter_At_Table,        // Официант подошел к столу
    Order_Taken,            // Официант принял заказ
    Order_To_Kitchen,       // Официант отнес заказ на кухню
    Order_Delivered,        // Официант принес заказ клиенту

    // ========== КУХНЯ ==========
    Kitchen_Order_Received, // Кухня получила заказ
    Prepare_Dish,           // Готовить конкретное блюдо
    Dish_Started_Cooking,   // Блюдо начало готовиться
    Dish_Ready,             // Блюдо готово
    All_Dishes_Ready,       // Все блюда в заказе готовы
    Order_Ready,            // Весь заказ готов

    // ========== ПОВАРА ==========
    Chef_Available,         // Повар свободен
    Chef_Busy,              // Повар занят
    Chef_Took_Dish,         // Повар взял блюдо в работу
    Chef_Finished_Dish,     // Повар закончил блюдо

    // ========== ОБЩИЕ ==========
    Clean_Table,            // Приказ убрать стол
    Simulation_Start,       // Начало симуляции
    Simulation_End,         // Конец симуляции

    // ========== ТАЙМЕРЫ ==========
    Timer_Started,          // Таймер запущен
    Timer_Finished,         // Таймер завершился
    Timer_Cancelled         // Таймер отменен
};

// Предварительное объявление
class Object;

// Структура сообщения с безопасным управлением памятью
struct MSG {
    MSGType type;
    Object* sender;
    std::shared_ptr<void> payload;  // ★★★★ ИЗМЕНЕНИЕ: shared_ptr вместо void* ★★★★

    // Конструкторы
    MSG() : type(MSGType::Simulation_Start), sender(nullptr), payload(nullptr) {}

    MSG(MSGType t, Object* s = nullptr, std::shared_ptr<void> p = nullptr)
        : type(t), sender(s), payload(p) {
    }

    // Конструктор для простых типов (int, bool, etc) - используем reinterpret_cast
    MSG(MSGType t, Object* s, int value)
        : type(t), sender(s),
        payload(reinterpret_cast<void*>(static_cast<intptr_t>(value)), [](void*) {}) {
    }

    // Конструктор для shared_ptr любых типов
    template<typename T>
    MSG(MSGType t, Object* s, std::shared_ptr<T> p)
        : type(t), sender(s), payload(std::static_pointer_cast<void>(p)) {
    }
};

// Вспомогательные функции для работы с payload
namespace MSGHelper {
    // Безопасное извлечение shared_ptr из сообщения
    template<typename T>
    inline std::shared_ptr<T> GetPayloadAs(const MSG& msg) {
        if (!msg.payload) return nullptr;
        return std::static_pointer_cast<T>(msg.payload);
    }

    // Для извлечения сырых указателей (для обратной совместимости)
    template<typename T>
    inline T* GetRawPayload(const MSG& msg) {
        if (!msg.payload) return nullptr;
        return static_cast<T*>(msg.payload.get());
    }

    // Создание сообщения с простым целочисленным значением
    inline MSG CreateWithInt(MSGType type, Object* sender, int value) {
        return MSG(type, sender, value);
    }

    // Извлечение целого числа из payload
    inline int GetIntFromPayload(const MSG& msg) {
        if (!msg.payload) return -1;
        return static_cast<int>(reinterpret_cast<intptr_t>(msg.payload.get()));
    }
}

#endif // MSG_H