#include <algorithm>
#include <iostream>
#include <vector>

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end);

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
  if (range_end - range_begin < 3) {
    return;
  }

  std::vector<typename RandomIt::value_type> value(range_begin, range_end);

  auto range_middle_first = value.begin() + (value.end() - value.begin()) / 3;
  auto range_middle_second = value.end() - (value.end() - value.begin()) / 3;
  MergeSort(value.begin(), range_middle_first);
  MergeSort(range_middle_first, range_middle_second);
  MergeSort(range_middle_second, value.end());

  std::vector<typename RandomIt::value_type> temp;
  std::merge(value.begin(), range_middle_first, range_middle_first,
             range_middle_second, std::back_inserter(temp));

  std::merge(temp.begin(), temp.end(), range_middle_second, value.end(),
             range_begin);
}

int main() {
  std::vector<int> v = {6, 4, 7, 6, 4, 4, 0, 1, 5};
  MergeSort(begin(v), end(v));
  for (int x : v) {
    std::cout << x << " ";
  }
  std::cout << std::endl;
  return 0;
}