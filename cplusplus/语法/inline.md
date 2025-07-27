```c++
// constants.h（正确写法，C++14 兼容）
#ifndef CONSTANTS_H
#define CONSTANTS_H

// 正确：C++14 中，头文件里的非模板变量必须加 inline
inline constexpr int MAX_SIZE = 1024;  // 显式 inline
// C++20 中：如果 const 变量的初始化满足 constexpr 要求，则无需显式 inline

#endif // CONSTANTS_H
```

```c++
#ifndef XXXX_H
#define XXXX_H
// C++14 中，头文件中定义静态成员变量的正确写法
class MyClass {
public:
    // 非静态成员变量（不需要 inline）
    double y = 3.14;  // C++11 起支持的就地初始化

    static constexpr int VALUE = 42;  // 声明 + 初始化（constexpr 特例，无需类外定义）
    static int counter;  // 普通静态成员变量：仅声明
};

// 必须在类外显式定义并初始化（C++14 中不能省略）
// 若多个 .cpp 文件包含此头文件，必须加 inline 避免重复定义
inline int MyClass::counter = 0;  // 显式 inline
#endif // XXXX_H
```

```c++
class MyClass {
public:
    inline static int counter = 0;  // 直接在类内定义
};
```


# 总结:

    1. 建议 在头文件里定义非模板变量(=赋值)加 inline;

    2. 使用 inline static 在类内直接定义静态成员变量;

    3. 类内可直接定义普通变量(就地初始化);