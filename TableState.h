#ifndef TABLESTATE_H
#define TABLESTATE_H

// Выносим enum в отдельное пространство имен
namespace TableState {
    enum State {
        FREE,      // Свободен
        OCCUPIED,  // Занят
        DIRTY      // Грязный
    };
}

#endif // TABLESTATE_H