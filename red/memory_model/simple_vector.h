//
// Created by mariakolyachko on 24.01.20.
//

#ifndef MEMORY_MODEL_SIMPLE_VECTOR_H
#define MEMORY_MODEL_SIMPLE_VECTOR_H

#include <cstdlib>

// Реализуйте шаблон SimpleVector
template <typename T> class SimpleVector {
public:
  SimpleVector() {
    data_ = end_ = 0;
    size_ = capacity_ = 0;
  }
  explicit SimpleVector(size_t size) {
    data_ = new T[size];
    end_ = &data_[size];
    capacity_ = size_ = size;
  }
  ~SimpleVector() { delete[] data_; }

  T &operator[](size_t index) { return data_[index]; }

  T *begin() { return data_; }
  T *end() { return end_; }

  [[nodiscard]] size_t Size() const {
    return size_;
  }

  [[nodiscard]] size_t Capacity() const {
    return capacity_;
  }

  void PushBack(const T &value) {
    if (size_ == capacity_) {
      T *temp;
      if (capacity_ != 0) {
        temp = new T[2 * capacity_];
        capacity_ *= 2;
      } else {
        temp = new T[1];
        ++capacity_;
      }

      for (size_t i = 0; i < size_; ++i) {
        temp[i] = data_[i];
      }
      delete[] data_;
      data_ = temp;
    }
    data_[size_] = value;
    ++size_;
    end_ = &data_[size_];
  }

private:
  T *data_;
  T *end_;
  size_t size_;
  size_t capacity_;
};

#endif // MEMORY_MODEL_SIMPLE_VECTOR_H
