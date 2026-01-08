#ifndef TABLE_H
#define TABLE_H

#include "Object.h"
#include "TableState.h"  // Вместо Hall.h

class Table : public Object {
private:
    int tableID_;
    TableState::State state_;  // Используем TableState::

public:
    Table(int id, int x, int y);
    virtual ~Table() = default;

    int GetID() const { return tableID_; }
    TableState::State GetState() const { return state_; }

    void MarkAsOccupied() { state_ = TableState::OCCUPIED; }
    void MarkAsDirty() { state_ = TableState::DIRTY; }
    void Clean() { state_ = TableState::FREE; }

    void Update() override;
    void OnMsg(const MSG& msg) override;

    // Вспомогательные методы
    bool IsFree() const { return state_ == TableState::FREE; }
    bool IsOccupied() const { return state_ == TableState::OCCUPIED; }
    bool IsDirty() const { return state_ == TableState::DIRTY; }
};

#endif // TABLE_H