#include "search_server.h"
#include "parse.h"
#include "test_runner.h"
#include "profile.h"

#include <algorithm>
#include <iterator>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <random>
#include <thread>
using namespace std;

void TestFunctionality(
    const vector<string>& docs,
    const vector<string>& queries,
    const vector<string>& expected
) {
  istringstream docs_input(Join('\n', docs));
  istringstream queries_input(Join('\n', queries));
  ostringstream queries_output;
  ostringstream queries_output2;
  ostringstream queries_output3;
  ostringstream queries_output4;
  {
    SearchServer srv;
    {
      LOG_DURATION("update")
      srv.UpdateDocumentBase(docs_input);
    }
    {
      LOG_DURATION("queries")
      srv.AddQueriesStream(queries_input, queries_output);
      srv.AddQueriesStream(queries_input, queries_output2);
      srv.AddQueriesStream(queries_input, queries_output3);
      srv.AddQueriesStream(queries_input, queries_output4);
    }
  }
  const string result = queries_output.str();
  const auto lines = SplitBy(Strip(result), '\n');
  ASSERT_EQUAL(lines.size(), expected.size());
  for (size_t i = 0; i < lines.size(); ++i) {
    ASSERT_EQUAL(lines[i], expected[i]);
  }
}

void TestSerpFormat() {
  const vector<string> docs = {
      "london is the capital of great britain",
      "i am travelling down the river"
  };
  const vector<string> queries = {"london", "the"};
  const vector<string> expected = {
      "london: {docid: 0, hitcount: 1}",
      Join(' ', vector{
          "the:",
          "{docid: 0, hitcount: 1}",
          "{docid: 1, hitcount: 1}"
      })
  };

  TestFunctionality(docs, queries, expected);
}

void TestTop5() {
  const vector<string> docs = {
      "milk a",
      "milk b",
      "milk c",
      "milk d",
      "milk e",
      "milk f",
      "milk g",
      "water a",
      "water b",
      "fire and earth"
  };

  const vector<string> queries = {"milk", "water", "rock"};
  const vector<string> expected = {
      Join(' ', vector{
          "milk:",
          "{docid: 0, hitcount: 1}",
          "{docid: 1, hitcount: 1}",
          "{docid: 2, hitcount: 1}",
          "{docid: 3, hitcount: 1}",
          "{docid: 4, hitcount: 1}"
      }),
      Join(' ', vector{
          "water:",
          "{docid: 7, hitcount: 1}",
          "{docid: 8, hitcount: 1}",
      }),
      "rock:",
  };
  TestFunctionality(docs, queries, expected);
}

void TestHitcount() {
  const vector<string> docs = {
      "the river goes through the entire city there is a house near it",
      "the wall",
      "walle",
      "is is is is",
  };
  const vector<string> queries = {"the", "wall", "all", "is", "the is"};
  const vector<string> expected = {
      Join(' ', vector{
          "the:",
          "{docid: 0, hitcount: 2}",
          "{docid: 1, hitcount: 1}",
      }),
      "wall: {docid: 1, hitcount: 1}",
      "all:",
      Join(' ', vector{
          "is:",
          "{docid: 3, hitcount: 4}",
          "{docid: 0, hitcount: 1}",
      }),
      Join(' ', vector{
          "the is:",
          "{docid: 3, hitcount: 4}",
          "{docid: 0, hitcount: 3}",
          "{docid: 1, hitcount: 1}",
      }),
  };
  TestFunctionality(docs, queries, expected);
}

void TestRanking() {
  const vector<string> docs = {
      "london is the capital of great britain",
      "paris is the capital of france",
      "berlin is the capital of germany",
      "rome is the capital of italy",
      "madrid is the capital of spain",
      "lisboa is the capital of portugal",
      "bern is the capital of switzerland",
      "moscow is the capital of russia",
      "kiev is the capital of ukraine",
      "minsk is the capital of belarus",
      "astana is the capital of kazakhstan",
      "beijing is the capital of china",
      "tokyo is the capital of japan",
      "bangkok is the capital of thailand",
      "welcome to moscow the capital of russia the third rome",
      "amsterdam is the capital of netherlands",
      "helsinki is the capital of finland",
      "oslo is the capital of norway",
      "stockgolm is the capital of sweden",
      "riga is the capital of latvia",
      "tallin is the capital of estonia",
      "warsaw is the capital of poland",
  };

  const vector<string> queries = {"moscow is the capital of russia"};
  const vector<string> expected = {
      Join(' ', vector{
          "moscow is the capital of russia:",
          "{docid: 7, hitcount: 6}",
          "{docid: 14, hitcount: 6}",
          "{docid: 0, hitcount: 4}",
          "{docid: 1, hitcount: 4}",
          "{docid: 2, hitcount: 4}",
      })
  };
  TestFunctionality(docs, queries, expected);
}

void TestBasicSearch() {
  vector<string> docs;

  for (int i = 0; i < 1000; ++i) {
    docs.emplace_back("we are ready to go");
    docs.emplace_back("come on everybody shake you hands");
    docs.emplace_back("i love this game");
    docs.emplace_back("just like exception safety is not about writing try catch everywhere in your code move semantics are not about typing double ampersand everywhere in your code");
    docs.emplace_back("daddy daddy daddy dad dad dad");
    docs.emplace_back("tell me the meaning of being lonely");
    docs.emplace_back("just keep track of it");
    docs.emplace_back("how hard could it be");
    docs.emplace_back("it is going to be legen wait for it dary legendary");
    docs.emplace_back("we dont need no education");
  }

  vector<string> queries;

  for (int i = 0; i < 1000; ++i) {
    queries.emplace_back("we need some help");
    queries.emplace_back("it");
    queries.emplace_back("i love this game");
    queries.emplace_back("dislike");
    queries.emplace_back("about");
  }

  const vector<string> expected = {
      Join(' ', vector{
          "we need some help:",
          "{docid: 9, hitcount: 2}",
          "{docid: 0, hitcount: 1}"
      }),
      Join(' ', vector{
          "it:",
          "{docid: 8, hitcount: 2}",
          "{docid: 6, hitcount: 1}",
          "{docid: 7, hitcount: 1}",
      }),
      "i love this game: {docid: 2, hitcount: 4}",
      "tell me why: {docid: 5, hitcount: 2}",
      "dislike:",
      "about: {docid: 3, hitcount: 2}",
  };
  TestFunctionality(docs, queries, expected);
}

int main() {
  LOG_DURATION("total")
  TestRunner tr;
  RUN_TEST(tr, TestSerpFormat);
  RUN_TEST(tr, TestTop5);
  RUN_TEST(tr, TestHitcount);
  RUN_TEST(tr, TestRanking);
 // RUN_TEST(tr, TestBasicSearch);
}