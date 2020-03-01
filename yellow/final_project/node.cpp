//
// Created by mariakolyachko on 6.01.20.
//

#include "node.h"

bool LogicalOperationNode::Evaluate(const Date &date,
                                    const std::string &event) {
  if (_operation == LogicalOperation::And) {
    return _left->Evaluate(date, event) && _right->Evaluate(date, event);
  } else {
    return _left->Evaluate(date, event) || _right->Evaluate(date, event);
  }
}

bool DateComparisonNode::Evaluate(const Date &date, const std::string &event) {
  switch (_cmp) {
  case Comparison::Less:
    return date < _date;
  case Comparison::LessOrEqual:
    return date < _date || date == _date;
  case Comparison::Greater:
    return !(date < _date || date == _date);
  case Comparison::GreaterOrEqual:
    return !(date < _date);
  case Comparison::Equal:
    return date == _date;
  case Comparison::NotEqual:
    return !(date == _date);
  }
}

bool EventComparisonNode::Evaluate(const Date &date,
                                   const std::string &event) {
  switch (_cmp) {
  case Comparison::Less:
    return event < _event;
  case Comparison::LessOrEqual:
    return event < _event || event == _event;
  case Comparison::Greater:
    return !(event < _event || event == _event);
  case Comparison::GreaterOrEqual:
    return !(event < _event);
  case Comparison::Equal:
    return event == _event;
  case Comparison::NotEqual:
    return !(event == _event);
  }
}
