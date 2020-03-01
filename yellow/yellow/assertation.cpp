//
// Created by mariakolyachko on 19.12.19.
//

#include "assertation.h"

void Assert(bool b, const std::string& hint) {
    AssertEqual(b, true, hint);
}