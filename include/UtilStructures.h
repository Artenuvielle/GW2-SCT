#pragma once
#include <vector>
#include <functional>
#include "json.hpp"

template <class T>
class ObservableVector : public std::vector<T>
{
public:

    ObservableVector()
    {}

    ~ObservableVector()
    {}

    void addOnEraseCallback(std::function<void(int)> callback) {
        onEraseCallbacks.push_back(callback);
    }

    constexpr std::vector<T>::iterator erase(std::vector<T>::const_iterator pos) {
        auto it = std::vector<T>::begin();
        int i = 0;
        while (it != std::vector<T>::end()) {
            if (it == pos) {
                for (auto& callback : onEraseCallbacks) callback(i);
                break;
            }
            it++;
            i++;
        }
        return std::vector<T>::erase(pos);
    }

    void addOnPushBackCallback(std::function<void(const T&)> callback) {
        onPushBackCallbacks.push_back(callback);
    }

    constexpr void push_back(const T& val) {
        std::vector<T>::push_back(val);
        size_t s = std::vector<T>::size();
        if (s > 0)
            for (auto& callback : onPushBackCallbacks) callback(val);
    }
private:
    std::vector<std::function<void(int)>> onEraseCallbacks;
    std::vector<std::function<void(const T&)>> onPushBackCallbacks;
};

template <class T>
class ObservableValue {
public:
    ObservableValue(T init) : value(init) {};
    ObservableValue& operator=(const ObservableValue& other) {
        T oldValue = value;
        value = other.value;
        for (auto& callback : onAssignCallbacks) callback.second(oldValue, other.value);
        return *this;
    }

    long onAssign(std::function<void(const T&, const T&)> callback) {
        onAssignCallbacks.insert(std::pair<long, std::function<void(const T&, const T&)>>(nextAssignCallbackIndex, callback));
        nextAssignCallbackIndex++;
        return nextAssignCallbackIndex - 1;
    }

    void removeOnAssign(long id) {
        auto found = onAssignCallbacks.find(id);
        if (found != onAssignCallbacks.end()) {
            onAssignCallbacks.erase(found);
        }
    }

    operator T() const { return value; };

    friend std::ostream& operator<< (std::ostream& stream, const ObservableValue<T>& ov) {
        return stream << ov.value;
    }

private:
    class TemporaryProxy {
    public:
        TemporaryProxy(ObservableValue<T>* proxyCaller, const T& originalValue) : proxyCaller(proxyCaller), originalValue(originalValue), newValue(originalValue) {
        };
        ~TemporaryProxy() {
            if (originalValue != newValue) {
                proxyCaller->operator=(newValue);
            }
        };
        operator T* () {
            return &newValue;
        }
    private:
        ObservableValue<T>* proxyCaller;
        const T originalValue;
        T newValue;
    };

public:
    TemporaryProxy operator&() {
        return TemporaryProxy(this, value);
    }

private:
    T value;
    std::map<long, std::function<void(const T&, const T&)>> onAssignCallbacks = {};
    long nextAssignCallbackIndex = 0;
};

template <class T>
void to_json(nlohmann::json& j, const ObservableValue<T>& p) {
    T t = p;
    j = t;
}

template <class T>
void from_json(const nlohmann::json& j, ObservableValue<T>& p) {
    T t = j;
    p = t;
}
