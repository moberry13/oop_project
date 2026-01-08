#ifndef OBJECT_H
#define OBJECT_H

struct MSG; // для чего он тут 

class Object {
public:
    virtual ~Object() = default;

    virtual void Update() = 0;

    virtual void OnMsg(const MSG& msg) = 0;

    int x = 0;
    int y = 0;
};

// т.е мы тут создаем виртуальный метод предмета и метод обновления на каждый тик 
// и обработки каждого 

#endif // !OBJECT_H