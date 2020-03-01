#include "test_runner.h"

void TestSortBy();
void TestUpdate();

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestUpdate);
}