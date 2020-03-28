//
// Created by mariakolyachko on 17.03.20.
//

#include "comment_server.h"

std::istream &operator>>(std::istream &input, ParsedResponse &r) {
  std::string line;
  getline(input, line);

  {
    std::istringstream code_input(line);
    std::string dummy;
    code_input >> dummy >> r.code;
  }

  size_t content_length = 0;

  r.headers.clear();
  while (getline(input, line) && !line.empty()) {
    if (auto [name, value] = SplitBy(line, ": "); name == "Content-Length") {
      std::istringstream length_input(value);
      length_input >> content_length;
    } else {
      r.headers.emplace_back(std::move(name), std::move(value));
    }
  }

  r.content.resize(content_length);
  input.read(r.content.data(), r.content.size());
  return input;
}

std::ostream &operator<<(std::ostream &output, const HttpHeader &h) {
  return output << h.name_ << ": " << h.value_;
}

bool operator==(const HttpHeader &lhs, const HttpHeader &rhs) {
  return lhs.name_ == rhs.name_ && lhs.value_ == rhs.value_;
}

std::pair<size_t, std::string> ParseIdAndContent(const std::string &body) {
  auto [id_string, content] = SplitBy(body, " ");
  return {FromString<size_t>(id_string), content};
}

std::pair<std::string, std::string> SplitBy(const std::string &what,
                                            const std::string &by) {
  size_t pos = what.find(by);
  if (by.size() < what.size() && pos < what.size() - by.size()) {
    return {what.substr(0, pos), what.substr(pos + by.size())};
  } else {
    return {what, {}};
  }
}

HttpResponse CommentServer::ServePostRequest(const HttpRequest &req) {
  HttpResponse httpResponse(HttpCode::OK);

  if (req.path == "/add_user") {
    auto response = std::to_string(comments_.size());
    comments_.emplace_back();
    return HttpResponse(HttpCode::OK)
        .AddHeader("Content-Length", std::to_string(response.size()))
        .SetContent(response);
  } else if (req.path == "/add_comment") {
    auto [user_id, comment] = ParseIdAndContent(req.body);

    if (!last_comment || last_comment->user_id != user_id) {
      last_comment = LastCommentInfo{user_id, 1};
    } else if (++last_comment->consecutive_count > 3) {
      banned_users.insert(user_id);
    }

    if (banned_users.count(user_id) == 0) {
      comments_[user_id].push_back(std::string(comment));
      return HttpResponse(HttpCode::OK);
    } else {
      return HttpResponse(HttpCode::Found).AddHeader("Location", "/captcha");
    }
  } else if (req.path == "/checkcaptcha") {
    if (auto [id, response] = ParseIdAndContent(req.body); response == "42") {
      banned_users.erase(id);
      if (last_comment && last_comment->user_id == id) {
        last_comment.reset();
      }
      return HttpResponse(HttpCode::OK);
    }
    return HttpResponse(HttpCode::Found).AddHeader("Location", "/captcha");
  } else {
    return HttpResponse(HttpCode::NotFound);
  }
}

HttpResponse CommentServer::ServeRequest(const HttpRequest &req) {
  if (req.method != "POST" && req.method != "GET") {
    return HttpResponse(HttpCode::NotFound);
  } else if (req.method == "POST") {
    return ServePostRequest(req);
  } else {
    return ServeGetRequest(req);
  }
}
HttpResponse CommentServer::ServeGetRequest(const HttpRequest &req) {
  if (req.path == "/user_comments") {
    auto user_id = FromString<size_t>(req.get_params.at("user_id"));
    std::string response;
    for (const std::string &c : comments_[user_id]) {
      response += c + '\n';
    }
    return HttpResponse(HttpCode::OK)
        .AddHeader("Content-Length", std::to_string(response.size()))
        .SetContent(response);
  } else if (req.path == "/captcha") {
    return HttpResponse(HttpCode::OK)
        .AddHeader("Content-Length", std::to_string(82))
        .SetContent("What's the answer for The Ultimate Question of Life, "
                    "the Universe, and Everything?");
  } else {
    return HttpResponse(HttpCode::NotFound);
  }
}

std::string to_string(HttpCode code) {
  switch (code) {
  case OK:
    return "OK";
  case Found:
    return "FOUND";
  case NotFound:
    return "NotFound";
  }
}

std::ostream &operator<<(std::ostream &output, const HttpResponse &resp) {
  output << "HTTP/1.1 " << resp.GetCode() << ' ' << to_string(resp.GetCode())
         << std::endl;
  for (const auto &header : resp.GetHeaders()) {
    output << header << std::endl;
  }
  output << std::endl;
  output << resp.GetContent() << std::endl;
  return output;
}
