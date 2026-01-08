#include "Timer.h"
#include <iostream>
#include <iomanip>

Timer::Timer()
    : totalTime_(0), remainingTime_(0), isActive_(false), isFinished_(false) {
}

Timer::Timer(int duration)
    : totalTime_(duration), remainingTime_(duration),
    isActive_(true), isFinished_(false) {
    if (duration <= 0) {
        isActive_ = false;
        isFinished_ = true;
    }
}

void Timer::Start(int duration) {
    totalTime_ = duration;
    remainingTime_ = duration;
    isActive_ = true;
    isFinished_ = false;

    if (duration <= 0) {
        isActive_ = false;
        isFinished_ = true;
    }
}

void Timer::Stop() {
    isActive_ = false;
}

void Timer::Update() {
    if (!isActive_ || isFinished_) return;

    remainingTime_--;

    if (remainingTime_ <= 0) {
        remainingTime_ = 0;
        isActive_ = false;
        isFinished_ = true;
    }
}

void Timer::Reset() {
    remainingTime_ = totalTime_;
    isActive_ = false;
    isFinished_ = false;
}

float Timer::GetProgress() const {
    if (totalTime_ <= 0) return 0.0f;
    return 1.0f - (static_cast<float>(remainingTime_) / totalTime_);
}

void Timer::PrintStatus() const {
    std::cout << "Timer: " << remainingTime_ << "/" << totalTime_
        << " (" << std::fixed << std::setprecision(1)
        << GetProgress() * 100.0f << "%)"
        << " Active: " << (isActive_ ? "Yes" : "No")
        << " Finished: " << (isFinished_ ? "Yes" : "No") << "\n";
}