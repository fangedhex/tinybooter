#pragma once

template<typename T>
class BasicFactory {
  public:
  template<typename ...Args>
  virtual T* create(Args... args) {
    return new T(...args);
  };
};
