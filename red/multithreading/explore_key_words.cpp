//
// Created by mariakolyachko on 30.01.20.
//

#include "profile.h"
#include "test_runner.h"
#include <iostream>
#include <sstream>

#include <algorithm>
#include <fstream>
#include <future>
#include <map>
#include <mutex>
#include <string>
using namespace std;

template <typename Iterator> class Page {
public:
  Page(const Iterator &begin, const Iterator &end) : _begin(begin), _end(end) {}

  Iterator &begin() { return _begin; }
  [[nodiscard]] const Iterator &begin() const { return _begin; }

  Iterator &end() { return _end; }
  [[nodiscard]] const Iterator &end() const { return _end; }

  [[nodiscard]] size_t size() const { return _end - _begin; }

private:
  Iterator _begin;
  Iterator _end;
};

template <typename Iterator> class Paginator {
public:
  Paginator(const Iterator &begin, const Iterator &end, size_t page_size) {
    for (auto it = begin; it < end; it += page_size) {
      if (end - it < page_size) {
        _pages.push_back(Page(it, end));
        break;
      } else {
        _pages.push_back(Page(it, it + page_size));
      }
    }
  }

  auto begin() { return _pages.begin(); }
  [[nodiscard]] auto begin() const { return _pages.begin(); }

  auto end() { return _pages.end(); }
  [[nodiscard]] auto end() const { return _pages.end(); }

  [[nodiscard]] size_t size() const { return _pages.size(); }

private:
  std::vector<Page<Iterator>> _pages;
};

template <typename Container> auto Paginate(Container &c, size_t page_size) {
  return Paginator(c.begin(), c.end(), page_size);
}

struct Stats {
  map<string, int> word_frequences;

  void operator+=(const Stats &other) {
    for (const auto &it : other.word_frequences) {
      word_frequences[it.first] += it.second;
    }
  }
};

class GetWords {
public:
  mutex m;

  string getWord(istream &input) {
    lock_guard<mutex> g(m);
    string str;
    input >> str;
    return str;
  }

  bool eof(istream &input) { return input.eof(); }
};

Stats Explore(const vector<string> &words, const set<string> &key_words) {
  Stats result;
  for (const auto &word : words) {
    if (key_words.count(word)) {
      ++result.word_frequences[word];
    }
  }
  return result;
}

Stats ExploreLine(const set<string> &key_words, const string &line) {
  string_view view = line;
  Stats result;
  int pos = view.find(' ');
  while (pos != std::string::npos) {
    string str = string(view.substr(0, pos));
    if (key_words.count(str)) {
      ++result.word_frequences[str];
    }
    view.remove_prefix(pos + 1);
    pos = view.find(' ');
  }
  string str = string(view);

  if (key_words.count(str)) {
    ++result.word_frequences[str];
  }
  return result;
}

Stats ExploreKeyWordsSingleThread(const set<string> &key_words,
                                  istream &input) {
  Stats result;
  for (string line; getline(input, line);) {
    result += ExploreLine(key_words, line);
  }
  return result;
}

Stats ExploreKeyWords(const set<string> &key_words, istream &input) {
  LOG_DURATION("ExploreKeyWords")
  const int page_size = 10000;
  vector<stringstream> streams;

  {LOG_DURATION("input")
    while (!input.eof()) {
      int i = 0;
      stringstream ss;
      while (!input.eof() && i < page_size) {
        string line;
        getline(input, line);
        ss << line << '\n';
        ++i;
      }
      streams.push_back(move(ss));
    }
  }

  vector<future<Stats>> futures;
  futures.reserve(streams.size());
  { LOG_DURATION("future")
    for (auto &stream : streams) {
      futures.push_back(
          async(ExploreKeyWordsSingleThread, ref(key_words), ref(stream)));
    }
  }

  Stats result;
  { LOG_DURATION("get")
    for (auto &future : futures) {
      result += future.get();
    }
  }
  return result;
}

void TestBasic() {
  const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

  ofstream stream("input.txt");
  for (int i = 0; i < 1; ++i) {
    for (int j = 0; j < 1; ++j) {
      stream << "this new yangle service really rocks";
    }
    stream << '\n';
    for (int j = 0; j < 1; ++j) {
      stream << "It sucks when yangle isn't available";
    }
    stream << '\n';
    for (int j = 0; j < 1; ++j) {
      stream << "10 reasons why yangle is the best IT company";
    }
    stream << '\n';
    for (int j = 0; j < 1; ++j) {
      stream << "yangle rocks others suck";
    }
    stream << '\n';
    for (int j = 0; j < 1; ++j) {
      stream << "Goondex really sucks, but yangle rocks. Use yangle";
    }
    stream << '\n';
  }
  stream.close();
  ifstream ss("input.txt");
  const auto stats = ExploreKeyWords(key_words, ss);
  const map<string, int> expected = {
      {"yangle", 600000}, {"rocks", 200000}, {"sucks", 100000}};
  ASSERT_EQUAL(stats.word_frequences, expected);
}

int main1() {
  TestRunner tr;
  RUN_TEST(tr, TestBasic);
  return 0;
}