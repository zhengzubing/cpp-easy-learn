#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

// 主题接口
class Subject {
   public:
    virtual ~Subject() = default;
    virtual void registerObserver(std::shared_ptr<class Observer> observer) = 0;
    virtual void removeObserver(std::shared_ptr<class Observer> observer) = 0;
    virtual void notifyObservers() = 0;
};

// 观察者接口
class Observer {
   public:
    virtual ~Observer() = default;
    virtual void update(float temp, float humidity, float pressure) = 0;
};

// 显示元素接口
class DisplayElement {
   public:
    virtual ~DisplayElement() = default;
    virtual void display() const = 0;
};

// 具体主题：WeatherData
class WeatherData : public Subject {
   private:
    std::vector<std::shared_ptr<Observer>> observers;
    float temperature;
    float humidity;
    float pressure;

   public:
    void registerObserver(std::shared_ptr<Observer> observer) override {
        observers.push_back(observer);
    }

    void removeObserver(std::shared_ptr<Observer> observer) override {
        observers.erase(
            std::remove(observers.begin(), observers.end(), observer),
            observers.end());
    }

    void notifyObservers() override {
        for (const auto& observer : observers) {
            observer->update(temperature, humidity, pressure);
        }
    }

    void measurementsChanged() { notifyObservers(); }

    void setMeasurements(float temp, float humi, float press) {
        temperature = temp;
        humidity = humi;
        pressure = press;
        measurementsChanged();
    }
};

// 具体显示类：CurrentConditionsDisplay
class CurrentConditionsDisplay : public Observer,
                                 public DisplayElement,
                                 public std::enable_shared_from_this<
                                     CurrentConditionsDisplay> {  // 新增继承
   private:
    float temperature;
    float humidity;
    std::shared_ptr<Subject> weatherData;

   public:
    CurrentConditionsDisplay(std::shared_ptr<Subject> wd) : weatherData(wd) {
        weatherData->registerObserver(shared_from_this());  // 现在可用
    }

    void update(float temp, float humidity, float pressure) override {
        this->temperature = temp;
        this->humidity = humidity;
        display();
    }

    void display() const override {
        std::cout << "Current conditions: " << temperature << "F degrees and "
                  << humidity << "% humidity\n";
    }
};

// 其他显示类（如StatisticsDisplay）也需类似修改
class StatisticsDisplay
    : public Observer,
      public DisplayElement,
      public std::enable_shared_from_this<StatisticsDisplay> {  // 新增继承
   private:
    float maxTemp = 0.0f;
    float minTemp = 200.0f;
    float tempSum = 0.0f;
    int numReadings = 0;
    std::shared_ptr<Subject> weatherData;

   public:
    StatisticsDisplay(std::shared_ptr<Subject> wd) : weatherData(wd) {
        weatherData->registerObserver(shared_from_this());  // 现在可用
    }

    void update(float temp, float humidity, float pressure) override {
        tempSum += temp;
        numReadings++;
        if (temp > maxTemp) maxTemp = temp;
        if (temp < minTemp) minTemp = temp;
        display();
    }

    void display() const override {
        std::cout << "Avg/Max/Min temperature = " << (tempSum / numReadings)
                  << "/" << maxTemp << "/" << minTemp << "\n";
    }
};

int main() {
    auto weatherData = std::make_shared<WeatherData>();
    auto currentDisplay =
        std::make_shared<CurrentConditionsDisplay>(weatherData);
    auto statsDisplay = std::make_shared<StatisticsDisplay>(weatherData);

    weatherData->setMeasurements(80, 65, 30.4);
    weatherData->setMeasurements(82, 70, 29.2);
    weatherData->setMeasurements(78, 90, 29.2);

    return 0;
}