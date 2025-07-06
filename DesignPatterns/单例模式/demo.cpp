// C++ 11
#include <iostream>

class Singleton {
   private:
    // 私有构造函数，防止外部实例化
    Singleton() { std::cout << "Singleton instance created." << std::endl; }
    // 禁用拷贝构造函数
    Singleton(const Singleton&) = delete;
    // 禁用赋值运算符
    Singleton& operator=(const Singleton&) = delete;

   public:
    // 静态方法，用于获取单例实例
    static Singleton& getInstance() {
        static Singleton instance;
        return instance;
    }

    // 示例方法
    void doSomething() {
        std::cout << "Singleton is doing something." << std::endl;
    }
};