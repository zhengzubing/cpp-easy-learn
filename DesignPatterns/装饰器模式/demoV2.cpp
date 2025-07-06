#include <iostream>
#include <memory>
#include <string>

// 抽象组件类
class Component {
   public:
    virtual std::string performTask() const = 0;
    virtual ~Component() = default;
};

// 具体组件类
class ConcreteComponent : public Component {
   public:
    std::string performTask() const override { return "ConcreteComponent"; }
};

// 抽象装饰器类(继承+组合)
class Decorator : public Component {
   protected:
    std::shared_ptr<Component> component;

   public:
    Decorator(std::shared_ptr<Component> comp) : component(comp) {}
    std::string performTask() const override {
        return component->performTask();
    }
};

// 具体装饰器类A
class ConcreteDecoratorA : public Decorator {
   public:
    ConcreteDecoratorA(std::shared_ptr<Component> comp) : Decorator(comp) {}
    std::string performTask() const override {
        return "ConcreteDecoratorA(" + Decorator::performTask() + ")";
    }
};

// 具体装饰器类B
class ConcreteDecoratorB : public Decorator {
   public:
    ConcreteDecoratorB(std::shared_ptr<Component> comp) : Decorator(comp) {}
    std::string performTask() const override {
        return "ConcreteDecoratorB(" + Decorator::performTask() + ")";
    }
};

int main() {
    // 创建具体组件对象
    auto simple = std::make_shared<ConcreteComponent>();
    std::cout << "Client: I've got a simple component: "
              << simple->performTask() << std::endl;

    // 用ConcreteDecoratorA装饰具体组件对象
    auto decorator1 = std::make_shared<ConcreteDecoratorA>(simple);
    std::cout << "Client: Now I've got a decorated component: "
              << decorator1->performTask() << std::endl;

    // 用ConcreteDecoratorB装饰ConcreteDecoratorA装饰后的对象
    auto decorator2 = std::make_shared<ConcreteDecoratorB>(decorator1);
    std::cout << "Client: Now I've got a double decorated component: "
              << decorator2->performTask() << std::endl;

    return 0;
}