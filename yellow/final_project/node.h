//
// Created by mariakolyachko on 3.01.20.
//

#ifndef FINAL_PROJECT_NODE_H
#define FINAL_PROJECT_NODE_H

#include "date.h"
#include "memory"

enum class LogicalOperation { And, Or };

enum class Comparison {
  Less,
  LessOrEqual,
  Greater,
  GreaterOrEqual,
  Equal,
  NotEqual
};

class Node {
public:

  virtual bool Evaluate(const Date &date, const std::string &event) = 0;

  const std::string _type;
};

class LogicalOperationNode : public Node {
public:
  LogicalOperationNode(const LogicalOperation &operation,
                       const std::shared_ptr<Node> &left,
                       const std::shared_ptr<Node> &next)
      : _operation(operation), _left(left), _right(next) {}

  bool Evaluate(const Date &date, const std::string &event) override;

  const LogicalOperation _operation;
  const std::shared_ptr<Node> _left;
  const std::shared_ptr<Node> _right;
};

class DateComparisonNode : public Node {
public:
  DateComparisonNode(const Comparison &cmp, const Date &date)
      : _cmp(cmp), _date(date) {}

  bool Evaluate(const Date &date, const std::string &event) override;

  const Comparison _cmp;
  const Date _date;
};

class EventComparisonNode : public Node {
public:
  EventComparisonNode(const Comparison &cmp, const std::string &event)
      : _cmp(cmp), _event(event) {}

  bool Evaluate(const Date &date, const std::string &event) override;

  const Comparison _cmp;
  const std::string _event;
};

class EmptyNode : public Node {
public:

  bool Evaluate(const Date &date, const std::string &event) override {
    return true;
  }
};

class AlwaysFalseNode : public Node {
  bool Evaluate(const Date&, const std::string& event) override {
    return false;
  }
};

#endif // FINAL_PROJECT_NODE_H
