#include <iostream>

// 策略接口
class Strategy {
   public:
    virtual ~Strategy() {}
    virtual void execute() const = 0;
};

// 具体策略类 A
class ConcreteStrategyA : public Strategy {
   public:
    void execute() const override {
        std::cout << "Executing strategy A" << std::endl;
    }
};

// 具体策略类 B
class ConcreteStrategyB : public Strategy {
   public:
    void execute() const override {
        std::cout << "Executing strategy B" << std::endl;
    }
};

// 上下文类
class Context {
   private:
    Strategy *strategy;

   public:
    Context(Strategy *strategy) : strategy(strategy) {}
    ~Context() { delete strategy; }
    void setStrategy(Strategy *newStrategy) {
        delete strategy;
        strategy = newStrategy;
    }
    void executeStrategy() const { strategy->execute(); }
};

int main() {
    // 创建具体策略对象
    Strategy *strategyA = new ConcreteStrategyA();
    Strategy *strategyB = new ConcreteStrategyB();

    // 创建上下文对象并设置初始策略
    Context context(strategyA);

    // 执行策略 A
    context.executeStrategy();

    // 切换到策略 B
    context.setStrategy(strategyB);

    // 执行策略 B
    context.executeStrategy();

    return 0;
}