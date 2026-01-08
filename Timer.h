#ifndef TIMER_H
#define TIMER_H

class Timer {
private:
    int totalTime_;      // Общее время (тиков)
    int remainingTime_;  // Оставшееся время
    bool isActive_;      // Активен ли таймер
    bool isFinished_;    // Завершен ли

public:
    Timer();
    Timer(int duration);

    // Основные методы
    void Start(int duration);
    void Stop();
    void Update();  // Вызывается каждый тик
    void Reset();

    // Геттеры
    bool IsActive() const { return isActive_; }
    bool IsFinished() const { return isFinished_; }
    int GetRemainingTime() const { return remainingTime_; }
    int GetTotalTime() const { return totalTime_; }
    float GetProgress() const;  // Прогресс 0.0 до 1.0

    // Проверки
    bool HasTime() const { return remainingTime_ > 0; }

    // Информация
    void PrintStatus() const;
};

#endif // TIMER_H