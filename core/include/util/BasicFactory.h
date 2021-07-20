#pragma once

template<typename T>
class BasicFactory {
  public:
  template<typename... Args>
  T* create(Args... args) {
    return new T(args...);
  };
};
