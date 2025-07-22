# decltype 与 auto 的对比

- **auto**：用于变量声明时推导类型，编译器会根据初始化值推导类型，不会保留引用和 const 限定符。
- **decltype**：直接推导表达式的类型，保留引用和 const 限定符。

## 示例

```cpp
int x = 42;
const int y = 10;

auto a = y;        // a 的类型是 int（去掉了 const）
decltype(y) b = y; // b 的类型是 const int
decltype((x)) c = x; // c 的类型是 int&，因为 (x) 是左值表达式
```

## decltype(auto)

C++14 引入了 `decltype(auto)`，它结合了 auto 和 decltype 的特性，用于更精确地推导返回值的类型。

**示例：decltype(auto) 用于函数返回值**

```cpp
int x = 42;
int& getRef() { return x; }

decltype(auto) ref = getRef(); // ref 的类型是 int&，保持了引用

// 如果使用 auto：
// auto ref = getRef(); // ref 的类型是 int（丢失了引用）
```

## 常见的坑和注意事项

- 左值引用与右值引用
 如果 expression 是一个的括号的左值表达式（如变量、解引用、函数返回左值引用等），decltype(expression) 返回左值引用类型
 如果 expression 是右值，decltype(expression) 会返回非引用类型。
 
```cpp
int x = 42;
decltype(x) a = x;  // a 的类型是 int
decltype((x)) b = x; // b 的类型是 int&，因为 (x) 是左值
```

```cpp
int x = 10;
decltype(x + 1) a = 20;   // x + 1 是右值
decltype((x + 1)) b = 30; // (x + 1) 仍然是右值

    x + 1 是一个右值，表示临时计算结果。
    (x + 1) 仅仅是加了括号改变优先级，但它仍然是右值。
```

左值加括号仍是左值：
```cpp
int x = 10;
decltype(x) a = 20;    // x 是左值，decltype(x) 是 int
decltype((x)) b = x;   // (x) 是左值，decltype((x)) 是 int&
```
