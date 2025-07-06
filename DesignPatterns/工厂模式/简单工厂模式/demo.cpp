#include <iostream>
#include <memory>

// 抽象产品类：图形
class Shape {
   public:
    virtual void draw() const = 0;
    virtual ~Shape() = default;
};

// 具体产品类：圆形
class Circle : public Shape {
   public:
    void draw() const override { std::cout << "绘制圆形" << std::endl; }
};

// 具体产品类：矩形
class Rectangle : public Shape {
   public:
    void draw() const override { std::cout << "绘制矩形" << std::endl; }
};

// 简单工厂类
class ShapeFactory {
   public:
    static std::unique_ptr<Shape> createShape(const std::string& shapeType) {
        if (shapeType == "Circle") {
            return std::make_unique<Circle>();
        } else if (shapeType == "Rectangle") {
            return std::make_unique<Rectangle>();
        }
        return nullptr;
    }
};

int main() {
    // 使用工厂创建圆形
    std::unique_ptr<Shape> circle = ShapeFactory::createShape("Circle");
    if (circle) {
        circle->draw();
    }

    // 使用工厂创建矩形
    std::unique_ptr<Shape> rectangle = ShapeFactory::createShape("Rectangle");
    if (rectangle) {
        rectangle->draw();
    }

    // 尝试创建不存在的图形
    std::unique_ptr<Shape> unknown = ShapeFactory::createShape("Triangle");
    if (!unknown) {
        std::cout << "无法创建该类型的图形" << std::endl;
    }

    return 0;
}