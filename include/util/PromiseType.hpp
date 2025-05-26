#pragma once
#include <coroutine>
#include <exception>
struct Void {};
template <class T> struct PromiseType {
  T value;
  std::exception_ptr exception;
  auto get_return_object() {
    return std::coroutine_handle<PromiseType<T>>::from_promise(*this);
  }
  std::suspend_always initial_suspend() noexcept { return {}; }
  std::suspend_always final_suspend() noexcept { return {}; }
  std::suspend_always yield_value(T value) {
    this->value = value;
    return {};
  }
  void unhandled_exception() { exception = std::current_exception(); }
  void return_value(T value) { this->value = value; }
};

template <> struct PromiseType<void> {
  Void value;
  std::exception_ptr exception;
  auto get_return_object() {
    return std::coroutine_handle<PromiseType<void>>::from_promise(*this);
  }
  std::suspend_always initial_suspend() noexcept { return {}; }
  std::suspend_always final_suspend() noexcept { return {}; }
  void unhandled_exception() { exception = std::current_exception(); }
  std::suspend_always yield_value(Void) { return {}; }
  void return_void() {}
};