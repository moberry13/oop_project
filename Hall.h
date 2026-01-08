#ifndef HALL_H
#define HALL_H

#include "Object.h"
#include "TableState.h"
#include <vector>

class Table;
class Waiter;  // Forward declaration
struct MSG;

class Hall : public Object {
private:
    std::vector<Table*> tables_;
    std::vector<Waiter*> waiters_;  // Список официантов

public:
    Hall();
    virtual ~Hall() = default;

    void AddTable(Table* table);
    void AddWaiter(Waiter* waiter);  // Регистрация официантов

    Table* FindFreeTable() const;
    Waiter* FindAvailableWaiter() const;  // Поиск свободного официанта

    void Update() override;
    void OnMsg(const MSG& msg) override;

    size_t GetTableCount() const { return tables_.size(); }
    size_t GetWaiterCount() const { return waiters_.size(); }
    size_t GetFreeTableCount() const;
};

#endif // HALL_H