#include <algorithm>
#include <deque>
#include <iostream>
#include <set>
#include <stack>
#include <vector>

template <typename T>
std::vector<T> FindGreaterElements(const std::set<T> &elements,
                                   const T &border);
std::vector<std::string> SplitIntoWords(const std::string &s);

template <typename T>
std::vector<T> FindGreaterElements(const std::set<T> &elements,
                                   const T &border) {
  auto iter = elements.lower_bound(border);
  if (iter != elements.end() && *iter == border) {
    ++iter;
  }

  std::vector<T> answer;
  for (; iter != elements.end(); ++iter) {
    answer.push_back(*iter);
  }
  return answer;
}

std::vector<std::string> SplitIntoWords(const std::string &s) {
  std::vector<std::string> answer;
  int start = 0;
  int finish;
  while ((finish = s.find(' ', start)) != std::string::npos) {
    answer.emplace_back(s.substr(start, finish - start));
    start = finish + 1;
  }
  answer.emplace_back(s.substr(start, s.size() - start));
  return answer;
}

template <typename RandomIt>
std::pair<RandomIt, RandomIt> FindStartsWith(RandomIt range_begin,
                                             RandomIt range_end, char prefix) {
  std::string start, finish;
  start.push_back(prefix);
  finish.push_back(prefix + 1);
  return std::make_pair(std::lower_bound(range_begin, range_end, start),
                        std::lower_bound(range_begin, range_end, finish));
}

template <typename RandomIt>
std::pair<RandomIt, RandomIt> FindStartsWith(RandomIt range_begin,
                                             RandomIt range_end,
                                             const std::string &prefix) {
  auto finish = prefix;
  ++finish[finish.size() - 1];
  return std::make_pair(std::lower_bound(range_begin, range_end, prefix),
                        std::lower_bound(range_begin, range_end, finish));
}

int main() {
  std::string x;
  std::cin >> x;
  int n;
  std::cin >> n;
  std::cin.ignore(1);

  std::deque<std::string> expression;
  expression.emplace_back(x);
  std::stack<int> priorities;

  for (int i = 0; i < n; i++) {
    int priority;
    std::string operation;
    getline(std::cin, operation);
    if (operation[0] == '+' || operation[0] == '-') {
      priority = 0;
    } else {
      priority = 1;
    }
    if (!priorities.empty() && priority > priorities.top()) {
      expression.push_front("(");
      expression.emplace_back(")");
    }
    priorities.push(priority);
    expression.emplace_back(" "  + operation);
  }

  for (const auto &item : expression) {
    std::cout << item;
  }
  return 0;
}
