# 函数指针、Lambda 与 std::function 的相互赋值

---

## 1. lambda -> 函数指针

- **无捕获的 Lambda** 可以隐式转换为函数指针。

```cpp
#include <iostream>

void callFunction(int (*funcPtr)(int, int)) {
    std::cout << "Result: " << funcPtr(3, 4) << std::endl;
}

int main() {
    auto lambda = [](int a, int b) -> int { return a + b; };
    callFunction(lambda); // 输出 Result: 7
    return 0;
}
```

---

## 2. lambda -> std::function

- Lambda（无论是否捕获变量）都可以赋值给 std::function。

```cpp
#include <iostream>
#include <functional>

int main() {
    int multiplier = 2;
    auto lambda = [multiplier](int a, int b) -> int {
        return (a + b) * multiplier;
    };
    std::function<int(int, int)> func = lambda;
    std::cout << "Result: " << func(3, 4) << std::endl; // 输出 Result: 14
    return 0;
}
```

---

## 3. 函数指针 -> std::function

- 普通函数指针可以直接赋值给 std::function。

```cpp
#include <iostream>
#include <functional>

int add(int a, int b) {
    return a + b;
}

int main() {
    int (*funcPtr)(int, int) = add;
    std::function<int(int, int)> func = funcPtr;
    std::cout << "Result: " << func(3, 4) << std::endl; // 输出 Result: 7
    return 0;
}
```

---

## 4. std::function -> 函数指针

- 只有 std::function 存储的是普通函数/函数指针时，才能通过 `target<T>()` 获取底层函数指针。

```cpp
#include <iostream>
#include <functional>

int add(int a, int b) {
    return a + b;
}

int main() {
    std::function<int(int, int)> func = add;
    auto funcPtr = func.target<int(*)(int, int)>();
    if (funcPtr) {
        std::cout << "Result: " << (*funcPtr)(3, 4) << std::endl; // 输出 Result: 7
    } else {
        std::cout << "Failed to retrieve function pointer!" << std::endl;
    }
    return 0;
}
```

---

## 5. 类的成员函数 -> 函数指针

- **成员函数不能直接赋值给普通函数指针**，因为成员函数需要对象实例。
- 可通过 Lambda、全局辅助函数、静态成员函数等方式间接实现。

### 5.1 Lambda 包装成员函数

```cpp
#include <iostream>

class Calculator {
public:
    int add(int a, int b) {
        return a + b;
    }
};

void callFunction(int (*funcPtr)(int, int, Calculator*), Calculator* calc) {
    std::cout << "Result: " << funcPtr(3, 4, calc) << std::endl;
}

int main() {
    Calculator calc;
    auto funcPtr = [](int a, int b, Calculator* obj) -> int {
        return obj->add(a, b);
    };
    using FuncPtr = int(*)(int, int, Calculator*);
    FuncPtr ptr = funcPtr;
    callFunction(ptr, &calc);
    return 0;
}
```

### 5.2 静态成员函数

```cpp
#include <iostream>

void callFunction(int (*funcPtr)(int, int)) {
    std::cout << "Result: " << funcPtr(3, 4) << std::endl;
}

class Calculator {
public:
    static int addStatic(int a, int b) {
        Calculator calc;
        return calc.add(a, b);
    }
    int add(int a, int b) {
        return a + b;
    }
};

int main() {
    callFunction(&Calculator::addStatic); // 输出 Result: 7
    return 0;
}
```

### 5.3 全局辅助函数

```cpp
#include <iostream>

void callFunction(int (*funcPtr)(int, int)) {
    std::cout << "Result: " << funcPtr(3, 4) << std::endl;
}

class Calculator {
public:
    int add(int a, int b) {
        return a + b;
    }
};

int addWrapper(int a, int b) {
    Calculator calc;
    return calc.add(a, b);
}

int main() {
    callFunction(addWrapper); // 输出 Result: 7
    return 0;
}
```

### 5.4 std::bind + 全局辅助函数

```cpp
#include <iostream>
#include <functional>

void callFunction(int (*funcPtr)(int, int)) {
    std::cout << "Result: " << funcPtr(3, 4) << std::endl;
}

int boundAddWrapper(int a, int b) {
    static Calculator calc;
    static auto boundAdd = std::bind(&Calculator::add, &calc, std::placeholders::_1, std::placeholders::_2);
    return boundAdd(a, b);
}

class Calculator {
public:
    int add(int a, int b) {
        return a + b;
    }
};

int main() {
    callFunction(boundAddWrapper); // 输出 Result: 7
    return 0;
}
```

---

## 6. 类的成员函数 -> std::function

- 通过 std::bind 绑定对象后赋值给 std::function。

```cpp
#include <iostream>
#include <functional>

class Calculator {
public:
    int add(int a, int b) {
        return a + b;
    }
};

int main() {
    Calculator calc;
    std::function<int(int, int)> func = std::bind(&Calculator::add, &calc, std::placeholders::_1, std::placeholders::_2);
    std::cout << "Result: " << func(3, 4) << std::endl; // 输出 Result: 7
    return 0;
}
```