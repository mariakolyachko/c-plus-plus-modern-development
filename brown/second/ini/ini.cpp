//
// Created by mariakolyachko on 13.03.20.
//

#include "ini.h"

namespace Ini {
Document Load(std::istream &input) {
  Document doc;

  std::string section_name;
  Section *section_data = nullptr;
  while (!input.eof()) {
    if (input.peek() == '[') {
      input.ignore(1);
      getline(input, section_name);
      section_name.erase(section_name.size() - 1, 1);
      section_data = &doc.AddSection(section_name);
    } else {
      std::string section_item;
      getline(input, section_item);
      if (section_item.empty()) {
        continue;
      }

      auto iter = section_item.find('=');
      (*section_data)[section_item.substr(0, iter)] =
          section_item.substr(iter + 1);
    }
  }
  return doc;
}
}