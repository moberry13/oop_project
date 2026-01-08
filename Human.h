#ifndef HUMAN_H
#define HUMAN_H

#include "Object.h"
#include <string>

class Human : public Object {
protected:
    std::string name_;
    int speed_;           // скорость перемещения (пикселей/тик)
    bool isMoving_;
    int targetX_, targetY_;

public:
    Human(const std::string& name = "Unknown", int speed = 1);
    virtual ~Human() = default;

    // Методы перемещения
    virtual void MoveTo(int x, int y);
    virtual void StopMoving();
    virtual bool HasReachedTarget() const;

    // Геттеры
    std::string GetName() const { return name_; }
    int GetSpeed() const { return speed_; }
    bool IsMoving() const { return isMoving_; }

    // Обновление положения
    void UpdatePosition();

    // Виртуальные методы Object
    void Update() override;
    void OnMsg(const MSG& msg) override;

    // Абстрактный метод (для специализации в дочерних классах)
    virtual void PerformAction() = 0;
};

#endif // HUMAN_H