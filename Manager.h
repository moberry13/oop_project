#ifndef MANAGER_H
#define MANAGER_H

#include <vector>
#include <queue>
#include "Object.h"
#include "MSG.h"

class Manager {
public:
    static Manager& GetInstance();

    Manager(const Manager&) = delete;
    Manager& operator=(const Manager&) = delete;

    // === PUBLIC МЕТОДЫ ===
    void RunMainLoop(int maxTicks = 100);

    // ВОЗВРАЩАЕМ ПРОСТУЮ ВЕРСИЮ:
    void Send(const MSG& msg);  // Только одна версия!

    void RegisterObject(Object* obj);
    void DebugPrint(int tick) const;
    void PrintStats() const;
    void ProcessMessages();

    int GetCurrentTick() const { return currentTick_; }
    bool IsRunning() const { return running_; }
    void Stop() { running_ = false; }
    size_t GetObjectCount() const { return objects_.size(); }
    void ClearAllObjects();

private:
    Manager() = default;
    ~Manager() = default;

    std::vector<Object*> objects_;
    std::queue<MSG> messageQueue_;
    int currentTick_ = 0;
    bool running_ = true;
    int maxTicks_ = 100;

    void UpdateAll();
};

#endif // MANAGER_H