#include "Table.h"
#include "MSG.h"
#include "Manager.h"
#include <iostream>

Table::Table(int id, int x, int y)
    : tableID_(id), state_(TableState::FREE) {
    this->x = x;
    this->y = y;
    std::cout << "[Table #" << tableID_ << "] Created at ("
        << x << ", " << y << "), state: FREE\n";
}

void Table::Update() {
    // Пока пассивный
}

void Table::OnMsg(const MSG& msg) {
    switch (msg.type) {
    case MSGType::Clean_Table: {
        if (IsDirty()) {
            Clean();
            std::cout << "[Table #" << tableID_ << "] Cleaned by waiter\n";
        }
        break;
    }
    default:
        // Игнорируем другие сообщения
        break;
    }
}