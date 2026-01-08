#include "Human.h"
#include <iostream>
#include <cmath>  // для std::abs

Human::Human(const std::string& name, int speed)
    : name_(name), speed_(speed), isMoving_(false), targetX_(0), targetY_(0) {
    std::cout << "[Human] " << name_ << " created at (" << x << ", " << y << ")\n";
}

void Human::MoveTo(int x, int y) {
    targetX_ = x;
    targetY_ = y;
    isMoving_ = true;
    std::cout << "[Human] " << name_ << " moving to (" << x << ", " << y << ")\n";
}

void Human::StopMoving() {
    isMoving_ = false;
    std::cout << "[Human] " << name_ << " stopped\n";
}

bool Human::HasReachedTarget() const {
    return (x == targetX_ && y == targetY_) || !isMoving_;
}

void Human::UpdatePosition() {
    if (!isMoving_) return;

    // Двигаемся по X
    if (x < targetX_) {
        x += speed_;
        if (x > targetX_) x = targetX_;
    }
    else if (x > targetX_) {
        x -= speed_;
        if (x < targetX_) x = targetX_;
    }

    // Двигаемся по Y
    if (y < targetY_) {
        y += speed_;
        if (y > targetY_) y = targetY_;
    }
    else if (y > targetY_) {
        y -= speed_;
        if (y < targetY_) y = targetY_;
    }

    // Проверяем, достигли ли цели
    if (x == targetX_ && y == targetY_) {
        isMoving_ = false;
        std::cout << "[Human] " << name_ << " reached target (" << x << ", " << y << ")\n";
    }
}

void Human::Update() {
    UpdatePosition();
}

void Human::OnMsg(const MSG& msg) {
    // Базовый класс обрабатывает только перемещение
    // Конкретная логика - в дочерних классах
}

// PerformAction() - чисто виртуальный, реализуется в наследниках