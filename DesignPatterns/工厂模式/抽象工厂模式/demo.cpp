#include <iostream>
#include <memory>

// 抽象产品类：按钮
class Button {
   public:
    virtual void paint() = 0;
    virtual ~Button() = default;
};

// 抽象产品类：复选框
class Checkbox {
   public:
    virtual void paint() = 0;
    virtual ~Checkbox() = default;
};

// 具体产品类：Windows风格按钮
class WindowsButton : public Button {
   public:
    void paint() override { std::cout << "Painting a Windows style button.\n"; }
};

// 具体产品类：Windows风格复选框
class WindowsCheckbox : public Checkbox {
   public:
    void paint() override {
        std::cout << "Painting a Windows style checkbox.\n";
    }
};

// 具体产品类：MacOS风格按钮
class MacOSButton : public Button {
   public:
    void paint() override { std::cout << "Painting a MacOS style button.\n"; }
};

// 具体产品类：MacOS风格复选框
class MacOSCheckbox : public Checkbox {
   public:
    void paint() override { std::cout << "Painting a MacOS style checkbox.\n"; }
};

// 抽象工厂类：定义创建UI组件的接口
class GuiFactory {
   public:
    virtual std::unique_ptr<Button> createButton() = 0;
    virtual std::unique_ptr<Checkbox> createCheckbox() = 0;
    virtual ~GuiFactory() = default;
};

// 具体工厂类：Windows平台的UI组件工厂
class WindowsGuiFactory : public GuiFactory {
   public:
    std::unique_ptr<Button> createButton() override {
        return std::make_unique<WindowsButton>();
    }
    std::unique_ptr<Checkbox> createCheckbox() override {
        return std::make_unique<WindowsCheckbox>();
    }
};

// 具体工厂类：MacOS平台的UI组件工厂
class MacOSGuiFactory : public GuiFactory {
   public:
    std::unique_ptr<Button> createButton() override {
        return std::make_unique<MacOSButton>();
    }
    std::unique_ptr<Checkbox> createCheckbox() override {
        return std::make_unique<MacOSCheckbox>();
    }
};

int main() {
    // 使用Windows工厂创建组件
    std::unique_ptr<GuiFactory> windowsFactory =
        std::make_unique<WindowsGuiFactory>();
    std::unique_ptr<Button> winButton = windowsFactory->createButton();
    std::unique_ptr<Checkbox> winCheckbox = windowsFactory->createCheckbox();

    // 使用MacOS工厂创建组件
    std::unique_ptr<GuiFactory> macosFactory =
        std::make_unique<MacOSGuiFactory>();
    std::unique_ptr<Button> macButton = macosFactory->createButton();
    std::unique_ptr<Checkbox> macCheckbox = macosFactory->createCheckbox();

    // 绘制组件
    winButton->paint();
    winCheckbox->paint();
    macButton->paint();
    macCheckbox->paint();

    return 0;
}