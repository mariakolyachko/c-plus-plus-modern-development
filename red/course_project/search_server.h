//
// Created by mariakolyachko on 3.02.20.
//

#ifndef COURSE_PROJECT_SEARCH_SERVER_H
#define COURSE_PROJECT_SEARCH_SERVER_H

#include <deque>
#include <future>
#include <istream>
#include <list>
#include <map>
#include <unordered_map>
#include <mutex>
#include <ostream>
#include <set>
#include <string>
#include <vector>
using namespace std;

#include <mutex>
using namespace std;

template <typename T> class Synchronized {
public:
  explicit Synchronized(T initial = T()) : value(move(initial)) {}

  struct Access {
    lock_guard<mutex> guard;
    T &ref_to_value;
  };

  Access GetAccess() { return {lock_guard(m), value}; }

private:
  T value;
  mutex m;
};

class InvertedIndex {
public:
  void Add(const string &document);
  [[nodiscard]] const vector<pair<size_t, size_t>> &
  Lookup(const string &word) const;

  [[nodiscard]] const string &GetDocument(size_t id) const { return docs[id]; }

  [[nodiscard]] size_t GetDocumentCount() const { return docs.size(); }

private:
  vector<vector<pair<size_t, size_t>>> index;
  unordered_map<string, size_t> ids;
  vector<string> docs;

  vector<pair<size_t, size_t>> empty;
};

class SearchServer {
public:
  SearchServer() { is_first_update = true; }

  explicit SearchServer(istream &document_input);
  void UpdateDocumentBase(istream &document_input);
  void UpdateDocumentBaseAsync(istream &document_input);
  void AddQueriesStream(istream &query_input, ostream &search_results_output);
  void AddQueriesStreamAsync(istream &query_input, ostream &search_results_output);

private:
  Synchronized<InvertedIndex> index;
  vector<future<void>> queries;
  bool is_first_update;
  mutex server;
};

#endif // COURSE_PROJECT_SEARCH_SERVER_H
