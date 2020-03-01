//
// Created by mariakolyachko on 30.01.20.
//

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

template <typename RandomIt>
void MergeSort(RandomIt first, RandomIt last) {
  if (std::distance(first, last) > 1) {
    std::vector<typename RandomIt::value_type> data(
        std::make_move_iterator(first),
        std::make_move_iterator(last)
    );

    auto range_middle_first = next(first, std::distance(first, last) / 3);
    auto range_middle_second = prev(last, std::distance(first, last) / 3);

    MergeSort(first, range_middle_first);
    MergeSort(range_middle_first, range_middle_second);
    MergeSort(range_middle_second, last);

    std::vector<typename RandomIt::value_type> temp;
    std::merge(
        std::make_move_iterator(first),
        std::make_move_iterator(range_middle_first),
        std::make_move_iterator(range_middle_first),
        std::make_move_iterator(range_middle_second),
        std::back_inserter(temp)
    );
    std::merge(
        std::make_move_iterator(temp.begin()),
        std::make_move_iterator(temp.end()),
        std::make_move_iterator(range_middle_second),
        std::make_move_iterator(last),
        first
    );
  }
}

int main3() {
  std::vector<int> numbers = {1, 6, 3, 1, 3, 2, 6, 9, 3};
  MergeSort(numbers.begin(), numbers.end());
  for (auto number : numbers) {
    std::cout << number << ' ';
  }
  return 0;
}
