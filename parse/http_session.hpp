#ifndef HTTP_SESSION_HPP
#define HTTP_SESSION_HPP

#include <memory>
#include <vector>

namespace parse {

class HttpSession {
public:
  HttpSession(const std::string& host, const std::string& port);
  void post_data(const std::string& endpoint, const std::string& data);
  ~HttpSession();
private:
  class Impl;
  std::unique_ptr<Impl> connection_impl_;
};

}

#endif
