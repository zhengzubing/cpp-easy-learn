td::queue本身不是容器，是对底层容器的包装。
STL中std::queue默认底层容器是 std::deque，也可以指定为 std::list 等只要满足接口要求的容器（但不能用 std::vector，因为 vector 不支持 pop_front）。

```
std::queue<int> q1;                // 默认使用 deque
std::queue<int, std::deque<int>> q2; // 显式指定 deque
std::queue<int, std::list<int>> q3;  // 也可以用 list
```

