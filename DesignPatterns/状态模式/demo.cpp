#include <iostream>
#include <string>

// 前置声明
class NetworkState;
class OpenState;
class CloseState;
class ConnectState;

// 网络处理器类前置声明，以便在NetworkState中使用
class NetworkProcessor;

// 网络状态基类
class NetworkState {
protected:
    std::string stateName;

public:
    NetworkState* pNext;
    virtual void CloseOperation() = 0;
    virtual void ConnectOperation() = 0;
    virtual void OpenOperation() = 0;
    virtual std::string getStateName() const { return stateName; }
    virtual ~NetworkState() = default;
};

// 所有状态类的完整定义
class OpenState;
class CloseState;
class ConnectState;

// 打开状态
class OpenState : public NetworkState {
private:
    OpenState() { stateName = "Open"; }
    OpenState(const OpenState&) = delete;
    OpenState& operator=(const OpenState&) = delete;

public:
    static OpenState& getInstance() {
        static OpenState instance;
        return instance;
    }

    void CloseOperation() override;
    void ConnectOperation() override;
    void OpenOperation() override;
};

// 关闭状态
class CloseState : public NetworkState {
private:
    CloseState() { stateName = "Close"; }
    CloseState(const CloseState&) = delete;
    CloseState& operator=(const CloseState&) = delete;

public:
    static CloseState& getInstance() {
        static CloseState instance;
        return instance;
    }

    void CloseOperation() override;
    void ConnectOperation() override;
    void OpenOperation() override;
};

// 连接状态
class ConnectState : public NetworkState {
private:
    ConnectState() { stateName = "Connect"; }
    ConnectState(const ConnectState&) = delete;
    ConnectState& operator=(const ConnectState&) = delete;

public:
    static ConnectState& getInstance() {
        static ConnectState instance;
        return instance;
    }

    void CloseOperation() override;
    void ConnectOperation() override;
    void OpenOperation() override;
};

/* --------------OpenState start------------------- */
void OpenState::CloseOperation() {
    std::cout << "执行操作1: 从OpenState切换到CloseState" << std::endl;
    pNext = &CloseState::getInstance();
}

void OpenState::ConnectOperation() {
    std::cout << "执行操作2: 从OpenState切换到ConnectState" << std::endl;
    pNext = &ConnectState::getInstance();
}

void OpenState::OpenOperation() {
    std::cout << "执行操作3: OpenState保持不变" << std::endl;
    pNext = &OpenState::getInstance();
}
/* --------------OpenState end------------------- */

/* --------------CloseState start------------------- */
void CloseState::CloseOperation() {
    std::cout << "执行操作1: 从CloseState切换到CloseState" << std::endl;
    pNext = &CloseState::getInstance();
}

void CloseState::ConnectOperation() {
    std::cout << "执行操作2: 从CloseState切换到CloseState" << std::endl;
    pNext = &CloseState::getInstance();
}

void CloseState::OpenOperation() {
    std::cout << "执行操作2: 从CloseState切换到OpenState" << std::endl;
    pNext = &OpenState::getInstance();
}
/* --------------CloseState end------------------- */

/* --------------ConnectState start------------------- */
void ConnectState::CloseOperation() {
    std::cout << "执行操作1: 从ConnectState切换到CloseState" << std::endl;
    pNext = &CloseState::getInstance();
}

void ConnectState::ConnectOperation() {
    std::cout << "执行操作2: 从ConnectState切换到ConnectState" << std::endl;
    pNext = &ConnectState::getInstance();
}

void ConnectState::OpenOperation() {
    std::cout << "执行操作3: ConnectState保持不变" << std::endl;
    pNext = &ConnectState::getInstance();
}
/* --------------ConnectState end------------------- */

// 网络处理器类
class NetworkProcessor {
private:
    NetworkState* pState;

public:
    NetworkProcessor(NetworkState& initialState) : pState(&initialState) {}

    void ShutdownPhoneOperation() {
        std::cout << "\n当前状态: " << pState->getStateName() << std::endl;
        pState->CloseOperation();
        pState = pState->pNext;
        std::cout << "转换后状态: " << pState->getStateName() << std::endl;
    }

    void SurfaceInternetOperation() {
        std::cout << "\n当前状态: " << pState->getStateName() << std::endl;
        pState->ConnectOperation();
        pState = pState->pNext;
        std::cout << "转换后状态: " << pState->getStateName() << std::endl;
    }

    void TurnOnPhoneOperation() {
        std::cout << "\n当前状态: " << pState->getStateName() << std::endl;
        pState->OpenOperation();
        pState = pState->pNext;
        std::cout << "转换后状态: " << pState->getStateName() << std::endl;
    }

    std::string getCurrentState() const {
        return pState->getStateName();
    }
};

// 主函数测试
int main() {
    // 创建网络处理器，初始状态为Open
    NetworkProcessor processor(OpenState::getInstance());

    std::cout << "=== 网络状态模式测试 ===" << std::endl;
    std::cout << "初始状态: " << processor.getCurrentState() << std::endl;

    // 测试状态转换序列
    processor.ShutdownPhoneOperation();
    processor.SurfaceInternetOperation();
    processor.TurnOnPhoneOperation();
    processor.SurfaceInternetOperation();
    processor.SurfaceInternetOperation();
    processor.ShutdownPhoneOperation();

    return 0;
}    