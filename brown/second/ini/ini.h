//
// Created by mariakolyachko on 13.03.20.
//

#ifndef SECOND_INI_H
#define SECOND_INI_H

#include <unordered_map>
#include <iostream>

namespace Ini {
using Section = std::unordered_map<std::string, std::string>;

class Document {
public:
  Section &AddSection(const std::string &name) { return sections[name]; }

  const Section &GetSection(const std::string &name) const {
    return sections.at(name);
  }

  size_t SectionCount() const { return sections.size(); }

private:
  std::unordered_map<std::string, Section> sections;
};

Document Load(std::istream &input);
}

#endif // SECOND_INI_H
