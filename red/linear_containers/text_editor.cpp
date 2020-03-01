//
// Created by mariakolyachko on 27.01.20.
//

#include "test_runner.h"
#include <list>
#include <string>
using namespace std;

class Editor {
public:
  // Реализуйте конструктор по умолчанию и объявленные методы
  Editor() {
    cursor_ = data_.begin();
  }

  void Left() {
    if (cursor_ != data_.begin()) {
      cursor_ = prev(cursor_);
    }
  }

  void Right() {
    if (cursor_ != data_.end()) {
      cursor_ = next(cursor_);
    }
  }

  void Insert(char token) {
    if (cursor_ != data_.end()) {
      data_.insert(cursor_, token);
    } else {
      data_.push_back(token);
    }
  }

  void Cut(size_t tokens = 1) {
    buffer_.clear();
    auto temp_cursor = cursor_;
    for (size_t i = 0; i < tokens; ++i) {
      if (temp_cursor == data_.end()) {
        break;
      }
      buffer_ += *temp_cursor;
      data_.erase(temp_cursor++);
    }
    cursor_ = temp_cursor;
  }

  void Copy(size_t tokens = 1) {
    buffer_.clear();
    auto temp_cursor = cursor_;
    for (size_t i = 0; i < tokens; ++i) {
      if (temp_cursor == data_.end()) {
        break;
      }
      buffer_ += *temp_cursor;
      temp_cursor = next(temp_cursor);
    }
  }

  void Paste() {
    for (char & it : buffer_) {
      Insert(it);
    }
  }

  [[nodiscard]] string GetText() const {
    string text;
    for (char it : data_) {
      text += it;
    }
    return text;
  }

private:
  list<char> data_;
  list<char>::iterator cursor_;
  string buffer_;
};

void TypeText(Editor &editor, const string &text) {
  for (char c : text) {
    editor.Insert(c);
  }
}

void TestEditing() {
  {
    Editor editor;

    const size_t text_len = 12;
    const size_t first_part_len = 7;
    TypeText(editor, "hello, world");
    for (size_t i = 0; i < text_len; ++i) {
      editor.Left();
    }
    editor.Cut(first_part_len);
    for (size_t i = 0; i < text_len - first_part_len; ++i) {
      editor.Right();
    }

    TypeText(editor, ", ");
    editor.Paste();
    editor.Left();
    editor.Left();
    editor.Cut(3);
    ASSERT_EQUAL(editor.GetText(), "world, hello");
  }
  {
    Editor editor;

    TypeText(editor, "misprnit");
    editor.Left();
    editor.Left();
    editor.Left();
    editor.Cut(1);
    editor.Right();
    editor.Paste();

    ASSERT_EQUAL(editor.GetText(), "misprint");
  }
}

void TestReverse() {
  Editor editor;

  const string text = "esreveR";
  string s;
  for (char c : text) {
    editor.Insert(c);
    s = editor.GetText();
    editor.Left();
  }

  ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
  Editor editor;
  ASSERT_EQUAL(editor.GetText(), "");

  editor.Left();
  editor.Left();
  editor.Right();
  editor.Right();
  editor.Copy(0);
  editor.Cut(0);
  editor.Paste();

  ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
  Editor editor;

  editor.Paste();
  TypeText(editor, "example");
  editor.Left();
  editor.Left();
  editor.Paste();
  editor.Right();
  editor.Paste();
  editor.Copy(0);
  editor.Paste();
  editor.Left();
  editor.Cut(0);
  editor.Paste();

  ASSERT_EQUAL(editor.GetText(), "example");
}

void TestCut()
{
  Editor editor;
  //1
  editor.Cut(10);
  editor.Insert('a');
  editor.Left();
  //2
  editor.Cut(1);
  ASSERT_EQUAL(editor.GetText(), "");
  editor.Paste();
  ASSERT_EQUAL(editor.GetText(), "a");
  //3
  editor.Cut(0);
  editor.Paste();
  ASSERT_EQUAL(editor.GetText(), "a");
  TypeText(editor, "bcde");
  editor.Left();editor.Left();editor.Left();editor.Left();editor.Left();
  //4
  editor.Cut(10);
  ASSERT_EQUAL(editor.GetText(), "");
  editor.Paste();
  ASSERT_EQUAL(editor.GetText(), "abcde");
  editor.Left();editor.Left();editor.Left();editor.Left();editor.Left();
  //5
  editor.Cut(5);
  ASSERT_EQUAL(editor.GetText(), "");
  editor.Paste();
  //6
  editor.Left();editor.Left();editor.Left();editor.Left();editor.Left();
  editor.Cut(1);
  ASSERT_EQUAL(editor.GetText(), "bcde");
  editor.Right();
  editor.Cut(1);
  ASSERT_EQUAL(editor.GetText(), "bde");
  editor.Cut(1);
  editor.Cut(1);
  ASSERT_EQUAL(editor.GetText(), "b");
}

void TestCopy()
{
  Editor editor;
  string s;
  //1
  editor.Copy(10);
  editor.Insert('a');
  s = editor.GetText();
  editor.Paste();
  s = editor.GetText();
  editor.Left();
  ASSERT_EQUAL(editor.GetText(), "a");
  //2
  editor.Copy(1);
  editor.Paste();
  s = editor.GetText();
  ASSERT_EQUAL(editor.GetText(), "aa");//between a
  //3
  editor.Copy(0);
  editor.Paste();
  s = editor.GetText();
  ASSERT_EQUAL(editor.GetText(), "aa");
  TypeText(editor, "bcde");
  s = editor.GetText();
  editor.Left();editor.Left();editor.Left();editor.Left();editor.Left();editor.Left();
  s = editor.GetText();
  //4
  editor.Cut(10);
  s = editor.GetText();
  ASSERT_EQUAL(editor.GetText(), "");
  editor.Paste();
  s = editor.GetText();
  ASSERT_EQUAL(editor.GetText(), "abcdea");
}

int main2() {
  TestRunner tr;
  RUN_TEST(tr, TestEditing);
  RUN_TEST(tr, TestReverse);
  RUN_TEST(tr, TestNoText);
  RUN_TEST(tr, TestEmptyBuffer);
  RUN_TEST(tr, TestCut);
  RUN_TEST(tr, TestCopy);
  return 0;
}