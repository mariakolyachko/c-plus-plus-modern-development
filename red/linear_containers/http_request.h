//
// Created by mariakolyachko on 28.01.20.
//

#ifndef LINEAR_CONTAINERS_HTTP_REQUEST_H
#define LINEAR_CONTAINERS_HTTP_REQUEST_H

#include <string_view>
using namespace std;

struct HttpRequest {
  string_view method, uri, protocol;
};


#endif // LINEAR_CONTAINERS_HTTP_REQUEST_H
