# C++ 中的 `constexpr` 类与成员详解

在 C++ 中，若要使用 `constexpr` 声明类的静态成员或创建 `constexpr` 类对象，类本身及其实例化过程需要满足一系列严格要求。这些要求确保对象能在**编译期完成初始化**，实现常量表达式的特性。

---

## 一、类构造函数的要求

- **C++11**：构造函数体必须为空（`{}`），所有成员初始化在初始化列表完成。
- **C++14 及以上**：允许构造函数体包含简单语句（如 `if`、循环），但不能有动态内存分配、I/O 等运行期操作。

**初始化列表要求**：所有非静态成员必须通过初始化列表初始化，且初始化表达式必须是编译期常量。

**访问控制**：构造函数可为 `public` 或 `private`，如单例模式可用私有构造函数。

---

## 二、类成员的要求

- `constexpr` 不能直接修饰非静态成员变量，必须结合 `static` 使用，即 `static constexpr`。

**示例：**
```cpp
class Circle {
private:
    static constexpr double PI = 3.14159;
public:
    constexpr Circle(double r) : radius(r) {}
    constexpr double area() const { return PI * radius * radius; }
};
```

---

## 三、类成员函数的要求

- **返回类型、参数类型**：必须是字面类型。
- **函数体**：
    - C++11：只能有 `return` 语句。
    - C++14 及以上：允许简单控制流（`if`、`switch`、循环），但不能有运行期操作。
- **const 限定**：`constexpr` 成员函数默认隐含 `const` 属性。

**示例：**
```cpp
class MathUtil {
public:
    constexpr static int pow(int base, int exp) {
        int result = 1;
        for (int i = 0; i < exp; ++i) {
            result *= base;
        }
        return result;
    }
};
constexpr int powResult = MathUtil::pow(2, 3); // 编译期计算
```

---

## 四、派生类的 `constexpr` 构造函数要求

- 派生类的 `constexpr` 构造函数必须通过初始化列表显式调用基类的 `constexpr` 构造函数，确保基类子对象在编译期初始化。

**示例：**
```cpp
class Base {
protected:
    int value;
public:
    constexpr Base(int v) : value(v) {}
};

class Derived : public Base {
private:
    int extra;
public:
    constexpr Derived(int v, int e) : Base(v), extra(e) {}
};
```

- 若基类构造函数不是 `constexpr`，则派生类也不能声明为 `constexpr`。

---

## 五、`constexpr` 类与虚函数/多态的兼容性

- `constexpr` 依赖编译期确定结果，而虚函数的多态性依赖运行期动态绑定，两者存在冲突。
- 虚函数声明为 `constexpr` 语法上允许，但多态调用无法在编译期完成。

**示例：**
```cpp
class Base {
public:
    constexpr virtual int getValue() const { return 0; }
};

class Derived : public Base {
public:
    constexpr int getValue() const override { return 42; }
};

constexpr Derived d;
constexpr int val = d.getValue(); // 有效
// constexpr Base* b = &d;
// constexpr int val2 = b->getValue(); // 错误：多态调用无法编译期确定
```

---

## 总结

- 派生类的 `constexpr` 构造函数必须初始化基类部分，保证整个对象编译期初始化。
- 虚函数与 `constexpr` 通常不兼容，仅在非多态场景下有限支持。
- `constexpr` 适合编译期可预测逻辑，多态则面向运行期动态行为，两者适用场景很少重叠。