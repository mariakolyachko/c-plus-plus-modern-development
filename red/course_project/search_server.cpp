//
// Created by mariakolyachko on 3.02.20.
//

#include "search_server.h"
#include "iterator_range.h"
#include "profile.h"

#include <algorithm>
#include <future>
#include <iostream>
#include <iterator>
#include <sstream>
#include <unordered_map>

vector<string> SplitIntoWords(const string &line) {
  istringstream words_input(line);
  return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

SearchServer::SearchServer(istream &document_input) {
  is_first_update = true;
  UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBaseAsync(istream &document_input) {
  lock_guard<mutex> guard(server);
  InvertedIndex new_index;
  for (string current_document; getline(document_input, current_document);) {
    new_index.Add(current_document);
  }

  swap(index.GetAccess().ref_to_value, new_index);
}

using SearchResult = pair<string, vector<pair<size_t, size_t>>>;

void SearchServer::AddQueriesStreamAsync(istream &query_input,
                                         ostream &search_results_output) {
  while(is_first_update) {}
  TotalDuration ind("ind");
  TotalDuration sortt("sort");

  vector<pair<size_t, size_t>> docid_count(
      index.GetAccess().ref_to_value.GetDocumentCount());

  for (string current_query; getline(query_input, current_query);) {
    const auto words = SplitIntoWords(current_query);
    {
      ADD_DURATION(ind) {
        for (size_t id = 0; id < docid_count.size(); ++id) {
          docid_count[id] = pair<size_t, size_t>(id, 0);
        }
      }
      for (const auto &word : words) {
        for (const auto docid : index.GetAccess().ref_to_value.Lookup(word)) {
          docid_count[docid.first].second += docid.second;
        }
      }
    }
    {
      ADD_DURATION(sortt)
      partial_sort(begin(docid_count),
                   next(begin(docid_count), min(size_t(5), docid_count.size())),
                   end(docid_count),
                   [](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
                     int64_t lhs_docid = lhs.first;
                     int64_t rhs_docid = rhs.first;
                     return make_pair(lhs.second, -lhs_docid) >
                            make_pair(rhs.second, -rhs_docid);
                   });
    }
    search_results_output << current_query << ':';
    for (auto [docid, hitcount] : Head(docid_count, 5)) {
      if (hitcount == 0) {
        break;
      }
      search_results_output << " {"
                            << "docid: " << docid << ", "
                            << "hitcount: " << hitcount << '}';
    }
    search_results_output << endl;
  }
}
void SearchServer::UpdateDocumentBase(istream &document_input) {
  if (is_first_update) {
    UpdateDocumentBaseAsync(document_input);
    is_first_update = false;
  } else {
    queries.push_back(async(launch::async,
                            &SearchServer::UpdateDocumentBaseAsync, this,
                            ref(document_input)));
  }
}
void SearchServer::AddQueriesStream(istream &query_input,
                                    ostream &search_results_output) {
  queries.push_back(async(launch::async, &SearchServer::AddQueriesStreamAsync,
                          this, ref(query_input), ref(search_results_output)));
}

void InvertedIndex::Add(const string &document) {
  docs.push_back(document);

  const size_t docid = docs.size() - 1;
  unordered_map<size_t, size_t> words_count;

  for (const auto &word : SplitIntoWords(document)) {
    if (auto it = ids.find(word); it != ids.end()) {
      ++words_count[it->second];
    } else {
      index.emplace_back();
      size_t id = ids.size();
      ids[word] = id;
      ++words_count[id];
    }
  }

  for (auto it : words_count) {
    index[it.first].push_back({docid, it.second});
  }
}

const vector<pair<size_t, size_t>> &
InvertedIndex::Lookup(const string &word) const {
  if (auto it = ids.find(word); it != ids.end()) {
    return index[it->second];
  } else {
    return empty;
  }
}