#ifndef COOK_H
#define COOK_H

#include "Human.h"
#include "Dish.h"
#include "Timer.h"
#include "MSGTypes.h"
#include <memory>
#include <vector>
#include <string>

class Cook : public Human {
protected:
    int cookID_;
    DishType specialization_;
    std::vector<std::shared_ptr<Dish>> currentDishes_;
    std::vector<Timer> dishTimers_;
    bool isAvailable_;

public:
    Cook(int id, DishType specialization, const std::string& name);
    virtual ~Cook() = default;

    int GetID() const { return cookID_; }
    DishType GetSpecialization() const { return specialization_; }
    bool IsAvailable() const { return isAvailable_; }
    int GetCurrentDishCount() const { return currentDishes_.size(); }

    virtual bool CanTakeMoreDishes() const;
    virtual bool AssignDish(std::shared_ptr<Dish> dish);
    virtual void UpdateCooking();
    virtual void CheckFinishedDishes();

    void PerformAction() override;
    void Update() override;
    void OnMsg(const MSG& msg) override;

    virtual std::string GetSpecializationString() const;
    void PrintStatus() const;

protected:
    virtual void OnDishFinished(std::shared_ptr<Dish> dish);
};

#endif // COOK_H