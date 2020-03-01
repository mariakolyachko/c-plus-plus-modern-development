//
// Created by mariakolyachko on 3.02.20.
//

#ifndef COURSE_PROJECT_ITERATOR_RANGE_H
#define COURSE_PROJECT_ITERATOR_RANGE_H

#include <algorithm>
using namespace std;

template <typename It> class IteratorRange {
public:
  IteratorRange(It first, It last) : first(first), last(last) {}

  It begin() const { return first; }

  It end() const { return last; }

  [[nodiscard]] size_t size() const { return last - first; }

private:
  It first, last;
};

template <typename Container> auto Head(Container &c, int top) {
  return IteratorRange(begin(c),
                       next(begin(c), min<size_t>(max(top, 0), c.size())));
}

#endif // COURSE_PROJECT_ITERATOR_RANGE_H
