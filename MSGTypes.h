#ifndef MESSAGE_TYPES_H
#define MESSAGE_TYPES_H

// Структура для передачи ID блюда
struct DishIDInfo {
    int dishID;

    DishIDInfo(int dID = -1) : dishID(dID) {}
};

// Структура для передачи заказа
struct OrderTransfer {
    int orderID;
    int tableID;
    int clientID;

    OrderTransfer(int oID = -1, int tID = -1, int cID = -1)
        : orderID(oID), tableID(tID), clientID(cID) {
    }
};

#endif // MESSAGE_TYPES_H