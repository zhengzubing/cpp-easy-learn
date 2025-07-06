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

// 抽象工厂类
class ShapeFactory {
   public:
    virtual std::unique_ptr<Shape> createShape() = 0;
    virtual ~ShapeFactory() = default;
};

// 具体工厂类：圆形工厂
class CircleFactory : public ShapeFactory {
   public:
    std::unique_ptr<Shape> createShape() override {
        return std::make_unique<Circle>();
    }
};

// 具体工厂类：矩形工厂
class RectangleFactory : public ShapeFactory {
   public:
    std::unique_ptr<Shape> createShape() override {
        return std::make_unique<Rectangle>();
    }
};

int main() {
    // 使用圆形工厂创建圆形
    std::unique_ptr<ShapeFactory> circleFactory =
        std::make_unique<CircleFactory>();
    std::unique_ptr<Shape> circle = circleFactory->createShape();
    if (circle) {
        circle->draw();
    }

    // 使用矩形工厂创建矩形
    std::unique_ptr<ShapeFactory> rectangleFactory =
        std::make_unique<RectangleFactory>();
    std::unique_ptr<Shape> rectangle = rectangleFactory->createShape();
    if (rectangle) {
        rectangle->draw();
    }

    return 0;
}