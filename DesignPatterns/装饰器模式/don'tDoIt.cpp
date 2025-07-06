#include <iostream>
#include <memory>
#include <string>

// 抽象图形类
class Shape {
   public:
    virtual void draw() const = 0;
    virtual ~Shape() = default;
};

// 具体图形类：圆形
class Circle : public Shape {
   public:
    void draw() const override { std::cout << "绘制圆形" << std::endl; }
};

// 具体图形类：矩形
class Rectangle : public Shape {
   public:
    void draw() const override { std::cout << "绘制矩形" << std::endl; }
};

// 颜色装饰器，依赖具体类型
// 可扩展性受限：当代码依赖特定类型时，每添加一种新的具体组件或具体装饰器类型，都需要修改依赖该类型的代码。
// 违反开闭原则：开闭原则强调软件实体（类、模块、函数等）应该对扩展开放，对修改关闭。依赖特定类型会导致在扩展功能时不得不修改已有的代码，破坏了开闭原则。
class ColorDecorator {
   private:
    std::string color;

   public:
    ColorDecorator(const std::string& c) : color(c) {}

    void drawWithColor(Shape* shape) {
        if (dynamic_cast<Circle*>(shape)) {
            std::cout << "用 " << color << " 颜色绘制圆形" << std::endl;
        } else if (dynamic_cast<Rectangle*>(shape)) {
            std::cout << "用 " << color << " 颜色绘制矩形" << std::endl;
        } else {
            std::cout << "不支持的图形类型，无法绘制颜色" << std::endl;
        }
    }
};

// 新的具体图形类：三角形
class Triangle : public Shape {
   public:
    void draw() const override { std::cout << "绘制三角形" << std::endl; }
};

int main() {
    // 创建圆形和矩形
    std::unique_ptr<Shape> circle = std::make_unique<Circle>();
    std::unique_ptr<Shape> rectangle = std::make_unique<Rectangle>();

    // 创建颜色装饰器
    ColorDecorator redDecorator("红色");

    // 用颜色装饰器绘制圆形和矩形
    redDecorator.drawWithColor(circle.get());
    redDecorator.drawWithColor(rectangle.get());

    // 创建三角形
    std::unique_ptr<Shape> triangle = std::make_unique<Triangle>();

    // 尝试用颜色装饰器绘制三角形
    redDecorator.drawWithColor(triangle.get());

    return 0;
}