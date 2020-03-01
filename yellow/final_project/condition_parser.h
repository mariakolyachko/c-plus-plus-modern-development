//
// Created by mariakolyachko on 5.01.20.
//

#ifndef FINAL_PROJECT_CONDITION_PARSER_H
#define FINAL_PROJECT_CONDITION_PARSER_H

#include "node.h"

#include <memory>
#include <iostream>

std::shared_ptr<Node> ParseCondition(std::istream& is);

void TestParseCondition();


#endif // FINAL_PROJECT_CONDITION_PARSER_H
