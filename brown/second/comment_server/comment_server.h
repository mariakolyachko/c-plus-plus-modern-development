//
// Created by mariakolyachko on 17.03.20.
//

#ifndef SECOND_COMMENT_SERVER_H
#define SECOND_COMMENT_SERVER_H

#include <map>
#include <optional>
#include <sstream>
#include <unordered_set>
#include <utility>
#include <vector>

enum HttpCode { OK = 200, Found = 302, NotFound = 404 };

struct HttpRequest {
  std::string method, path, body;
  std::map<std::string, std::string> get_params;
};

struct LastCommentInfo {
  size_t user_id, consecutive_count;
};

struct HttpHeader {
  std::string name_, value_;

  HttpHeader(std::string name, std::string value)
      : name_(std::move(name)), value_(std::move(value)) {}
};
std::ostream &operator<<(std::ostream &output, const HttpHeader &h);
bool operator==(const HttpHeader &lhs, const HttpHeader &rhs);

class HttpResponse {
private:
  HttpCode code_;
  std::vector<HttpHeader> headers_;
  std::string content_;

public:
  explicit HttpResponse(HttpCode code) : code_(code) {}

  HttpResponse &AddHeader(const std::string& name, const std::string& value) {
    headers_.emplace_back(name, value);
    return *this;
  }

  HttpResponse &SetContent(std::string a_content) {
    content_ = std::move(a_content);
    return *this;
  }

  HttpResponse &SetCode(HttpCode a_code) {
    code_ = a_code;
    return *this;
  }

  [[nodiscard]] HttpCode GetCode() const {
    return code_;
  }

  [[nodiscard]] std::vector<HttpHeader> GetHeaders() const {
    return headers_;
  }

  [[nodiscard]] std::string GetContent() const {
    return content_;
  }
  friend std::ostream &operator<<(std::ostream &output,
                                  const HttpResponse &resp);
};

std::pair<std::string, std::string> SplitBy(const std::string &what,
                                            const std::string &by);

template <typename T> T FromString(const std::string &s) {
  T x;
  std::istringstream is(s);
  is >> x;
  return x;
}

std::pair<size_t, std::string> ParseIdAndContent(const std::string &body);

class CommentServer {
private:
  std::vector<std::vector<std::string>> comments_;
  std::optional<LastCommentInfo> last_comment;
  std::unordered_set<size_t> banned_users;

  HttpResponse ServePostRequest(const HttpRequest &req);
  HttpResponse ServeGetRequest(const HttpRequest &req);

public:
  HttpResponse ServeRequest(const HttpRequest &req);
};

struct ParsedResponse {
  int code;
  std::vector<HttpHeader> headers;
  std::string content;
};

std::istream &operator>>(std::istream &input, ParsedResponse &r);

#endif // SECOND_COMMENT_SERVER_H
