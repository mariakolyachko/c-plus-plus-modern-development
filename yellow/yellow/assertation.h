//
// Created by mariakolyachko on 19.12.19.
//

#ifndef YELLOW_ASSERTATION_H
#define YELLOW_ASSERTATION_H

#include <sstream>

void Assert(bool b, const std::string& hint);
template<class T, class U>
void AssertEqual(const T& t, const U& u,
                 const std::string& hint);


template<class T, class U>
void AssertEqual(const T& t, const U& u,
                 const std::string& hint)
{
    if (t != u) {
        std::ostringstream os;
        os << "Assertion failed: " << t << " != " << u
           << " hint: " << hint;
        throw std::runtime_error(os.str());
    }
}

#endif //YELLOW_ASSERTATION_H
