#include "Hall.h"
#include "Table.h"
#include "Waiter.h"
#include "MSG.h"
#include "Manager.h"
#include <iostream>

Hall::Hall() {
    x = 0;
    y = 0;
    std::cout << "[Hall] Created at position (" << x << ", " << y << ")\n";
}

void Hall::AddTable(Table* table) {
    if (table) {
        tables_.push_back(table);
        std::cout << "[Hall] Table #" << table->GetID()
            << " added at (" << table->x << ", " << table->y << ")\n";
    }
}

void Hall::AddWaiter(Waiter* waiter) {
    if (waiter) {
        waiters_.push_back(waiter);
        std::cout << "[Hall] Waiter #" << waiter->GetID() << " registered\n";
    }
}

Table* Hall::FindFreeTable() const {
    for (Table* table : tables_) {
        if (table->GetState() == TableState::FREE) {
            return table;
        }
    }
    return nullptr;
}

Waiter* Hall::FindAvailableWaiter() const {
    for (Waiter* waiter : waiters_) {
        if (!waiter->IsBusy()) {
            return waiter;
        }
    }
    return nullptr;
}

size_t Hall::GetFreeTableCount() const {
    size_t count = 0;
    for (Table* table : tables_) {
        if (table->GetState() == TableState::FREE) {
            ++count;
        }
    }
    return count;
}

void Hall::Update() {
    // Пассивный
}

void Hall::OnMsg(const MSG& msg) {
    switch (msg.type) {
    case MSGType::Client_Entered: {
        std::cout << "[Hall] Received: Client_Entered\n";

        // 1. Ищем свободный стол
        Table* freeTable = FindFreeTable();
        if (!freeTable) {
            std::cout << "[Hall] No free tables! Client leaves.\n";
            return;
        }

        // 2. Помечаем стол занятым
        freeTable->MarkAsOccupied();

        // 3. Отправляем клиенту назначенный стол
        MSG tableMsg = MSGHelper::CreateWithInt(
            MSGType::Table_Assigned,
            this,
            freeTable->GetID()
        );
        Manager::GetInstance().Send(tableMsg);

        std::cout << "[Hall] Assigned Table #" << freeTable->GetID() << "\n";

        // ★★★★ ИСПРАВЛЕНИЕ: Назначаем официанта ★★★★
        Waiter* availableWaiter = FindAvailableWaiter();
        if (availableWaiter) {
            MSG waiterMsg = MSGHelper::CreateWithInt(
                MSGType::Waiter_Assigned,
                this,
                freeTable->GetID()
            );
            Manager::GetInstance().Send(waiterMsg);
            std::cout << "[Hall] Assigned Waiter #" << availableWaiter->GetID()
                << " to table #" << freeTable->GetID() << "\n";
        }
        else {
            std::cout << "[Hall] No available waiters for table #"
                << freeTable->GetID() << "\n";
        }
        break;
    }

    case MSGType::Client_Finished_Eating: {
        int tableID = MSGHelper::GetIntFromPayload(msg);
        std::cout << "[Hall] Client finished at table #" << tableID
            << ", marking as dirty\n";

        // Находим стол и помечаем грязным
        for (Table* table : tables_) {
            if (table->GetID() == tableID) {
                table->MarkAsDirty();
                std::cout << "[Hall] Table #" << tableID << " marked as DIRTY\n";

                // Уведомляем официанта
                MSG cleanMsg = MSGHelper::CreateWithInt(
                    MSGType::Table_Dirty,
                    this,
                    tableID
                );
                Manager::GetInstance().Send(cleanMsg);
                break;
            }
        }
        break;
    }

    case MSGType::Table_Cleaned: {
        int tableID = MSGHelper::GetIntFromPayload(msg);
        std::cout << "[Hall] Table #" << tableID << " has been cleaned\n";

        // Находим стол и отмечаем как свободный
        for (Table* table : tables_) {
            if (table->GetID() == tableID) {
                table->Clean();
                std::cout << "[Hall] Table #" << tableID << " is now FREE\n";
                break;
            }
        }
        break;
    }

    default:
        break;
    }
}