#include <iostream>
#include <vector>

template <typename T> class Deque {
public:
  [[nodiscard]] bool Empty() const { return _front.empty() && _back.empty(); }

  [[nodiscard]] size_t Size() const { return _front.size() + _back.size(); }

  T &operator[](size_t index) {
    if (index < _front.size()) {
      return _front[_front.size() - index - 1];
    } else {
      return _back[index - _front.size()];
    }
  }

  const T &operator[](size_t index) const {
    if (index < _front.size()) {
      return _front[_front.size() - index - 1];
    } else {
      return _back[index - _front.size()];
    }
  }

  T &At(size_t index) {
    if (Size() <= index) {
      throw std::out_of_range("");
    }
    return this->operator[](index);
  }

  const T &At(size_t index) const {
    if (Size() <= index) {
      throw std::out_of_range("");
    }
    return this->operator[](index);
  }

  T &Front() {
    if (!_front.empty()) {
      return _front.back();
    } else {
      return _back.front();
    }
  }

  const T &Front() const {
    if (!_front.empty()) {
      return _front.back();
    } else {
      return _back.front();
    }
  }

  T &Back() {
    if (!_back.empty()) {
      return _back.back();
    } else {
      return _front.front();
    }
  }

  const T &Back() const {
    if (!_back.empty()) {
      return _back.back();
    } else {
      return _front.front();
    }
  }

  void PushFront(const T &item) { _front.push_back(item); }

  void PushBack(const T &item) { _back.push_back(item); }

private:
  std::vector<T> _front;
  std::vector<T> _back;
};

int main() {
  std::cout << "Hello, World!" << std::endl;
  return 0;
}