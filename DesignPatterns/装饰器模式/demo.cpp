#include <iostream>
#include <string>

// 抽象组件类
class Component {
   public:
    virtual std::string performTask() const = 0;
    virtual ~Component() {}
};

// 具体组件类
class ConcreteComponent : public Component {
   public:
    // 基础能力
    std::string performTask() const override { return "ConcreteComponent"; }
};

// 抽象装饰器类(继承+组合)
class Decorator : public Component {
   protected:
    Component* component;

   public:
    Decorator(Component* comp) : component(comp) {}
    // 扩展能力
    std::string performTask() const override {
        return component->performTask();
    }
};

// 具体装饰器类A
class ConcreteDecoratorA : public Decorator {
   public:
    ConcreteDecoratorA(Component* comp) : Decorator(comp) {}
    // 扩展能力
    std::string performTask() const override {
        return "ConcreteDecoratorA(" + Decorator::performTask() + ")";
    }
};

// 具体装饰器类B
class ConcreteDecoratorB : public Decorator {
   public:
    ConcreteDecoratorB(Component* comp) : Decorator(comp) {}
    // 扩展能力
    std::string performTask() const override {
        return "ConcreteDecoratorB(" + Decorator::performTask() + ")";
    }
};

int main() {
    // 创建具体组件对象
    Component* simple = new ConcreteComponent;
    std::cout << "Client: I've got a simple component: "
              << simple->performTask() << std::endl;

    // 用ConcreteDecoratorA装饰具体组件对象
    Component* decorator1 = new ConcreteDecoratorA(simple);
    std::cout << "Client: Now I've got a decorated component: "
              << decorator1->performTask() << std::endl;

    // 用ConcreteDecoratorB装饰ConcreteDecoratorA装饰后的对象
    Component* decorator2 = new ConcreteDecoratorB(decorator1);
    std::cout << "Client: Now I've got a double decorated component: "
              << decorator2->performTask() << std::endl;

    delete simple;
    delete decorator1;
    delete decorator2;

    return 0;
}