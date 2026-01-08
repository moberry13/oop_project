#include "Manager.h"
#include <iostream>
#include <iomanip>

Manager& Manager::GetInstance() {
    static Manager instance;
    return instance;
}

void Manager::RegisterObject(Object* obj) {
    if (obj) {
        objects_.push_back(obj);
        std::cout << "[Manager] Object registered at ("
            << obj->x << ", " << obj->y << ")\n";
    }
}

// ÏÐÎÑÒÀß ÂÅÐÑÈß:
void Manager::Send(const MSG& msg) {
    messageQueue_.push(msg);  // Ïðîñòî êîïèðóåì
}

void Manager::ProcessMessages() {
    size_t messagesProcessed = 0;

    while (!messageQueue_.empty() && messagesProcessed < 100) {
        MSG msg = messageQueue_.front();  // Áåç std::move
        messageQueue_.pop();

        for (auto* obj : objects_) {
            if (obj) {
                obj->OnMsg(msg);
            }
        }

        ++messagesProcessed;
    }
}

void Manager::UpdateAll() {
    for (auto* obj : objects_) {
        if (obj) {
            obj->Update();
        }
    }
}

void Manager::DebugPrint(int tick) const {
    std::cout << "[Tick " << std::setw(3) << tick
        << "] Objects: " << objects_.size()
        << ", Messages in queue: " << messageQueue_.size() << "\n";
}

void Manager::PrintStats() const {
    std::cout << "\n=== Simulation Statistics ===\n";
    std::cout << "Total ticks: " << currentTick_ << "\n";
    std::cout << "Total objects: " << objects_.size() << "\n";
    std::cout << "=============================\n";
}

void Manager::ClearAllObjects() {
    objects_.clear();
    std::cout << "[Manager] All objects cleared\n";
}

void Manager::RunMainLoop(int maxTicks) {
    maxTicks_ = maxTicks;
    currentTick_ = 0;
    running_ = true;

    std::cout << "\n=== Cafe Simulator Started ===\n";
    std::cout << "Running for " << maxTicks_ << " ticks\n";
    std::cout << "Registered objects: " << objects_.size() << "\n";
    std::cout << "================================\n\n";

    while (running_ && currentTick_ < maxTicks_) {
        ProcessMessages();
        UpdateAll();

        if (currentTick_ % 10 == 0) {
            DebugPrint(currentTick_);
        }

        ++currentTick_;
    }

    PrintStats();
    std::cout << "\n=== Simulation finished ===\n";
}