//
// Created by mariakolyachko on 28.01.20.
//

#ifndef LINEAR_CONTAINERS_STATS_H
#define LINEAR_CONTAINERS_STATS_H

#include "http_request.h"

#include <algorithm>
#include <map>
#include <string_view>
#include <vector>
using namespace std;

class Stats {
public:
  Stats();
  void AddMethod(string_view method);

  void AddUri(string_view uri);
  [[nodiscard]] const map<string_view, int> &GetMethodStats() const;

  [[nodiscard]] const map<string_view, int> &GetUriStats() const;
private:
  const vector<string> METHODS = {"GET", "POST", "PUT", "DELETE", "UNKNOWN"};
  const vector<string> URIS = {"/",       "/order", "/product",
                               "/basket", "/help",  "unknown"};
  map<string_view, int> methods_;
  map<string_view, int> uris_;
};

HttpRequest ParseRequest(string_view line);


#endif // LINEAR_CONTAINERS_STATS_H
