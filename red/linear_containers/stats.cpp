#include "stats.h"
//
// Created by mariakolyachko on 28.01.20.
//

Stats::Stats() {
  for (const auto &method : METHODS) {
    methods_[method] = 0;
  }

  for (const auto &uri : URIS) {
    uris_[uri] = 0;
  }
}
void Stats::AddMethod(string_view method) {
  auto it = find(METHODS.begin(), METHODS.end(), method);
  if (it != METHODS.end()) {
    ++methods_[*it];
  } else {
    ++methods_[*find(METHODS.begin(), METHODS.end(), "UNKNOWN")];
  }
}

void Stats::AddUri(string_view uri) {
  auto it = find(URIS.begin(), URIS.end(), uri);
  if (it != URIS.end()) {
    ++uris_[*it];
  } else {
    ++uris_[*find(URIS.begin(), URIS.end(), "unknown")];
  }
}

[[nodiscard]] const map<string_view, int> &Stats::GetMethodStats() const {
  return methods_;
}

[[nodiscard]] const map<string_view, int> &Stats::GetUriStats() const {
  return uris_;
}

HttpRequest ParseRequest(string_view line) {
  while (!line.empty() && line[0] == ' ') {
    line.remove_prefix(1);
  }
  HttpRequest httpRequest;
  int position = line.find(' ');
  httpRequest.method = line.substr(0, position);
  line.remove_prefix(position + 1);
  position = line.find(' ');
  httpRequest.uri = line.substr(0, position);
  line.remove_prefix(position + 1);
  httpRequest.protocol = line;
  return httpRequest;
}