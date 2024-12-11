#include "http_session.hpp"

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

namespace parse {

// https://live.boost.org/doc/libs/1_86_0/libs/beast/example/http/client/sync/http_client_sync.cpp
struct HttpSession::Impl {
  static boost::asio::io_context io_context;
  boost::beast::tcp_stream stream;
  std::string host;
  std::string port;
  std::string endpoint;

  Impl(const std::string& host, const std::string& port)
    : host(host)
    , port(port)
    , stream(io_context)
  {
    boost::asio::ip::tcp::resolver resolver(io_context);
    const auto results = resolver.resolve(host, port);
    boost::system::error_code ec;
    stream.connect(results, ec);
    if (ec) {
        throw std::runtime_error("Failed to connect: " + ec.message());
    }
  }

  ~Impl() {
    stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both);
  }
};


boost::asio::io_context HttpSession::Impl::io_context;

HttpSession::HttpSession(const std::string& host, const std::string& port) 
  : connection_impl_(std::make_unique<Impl>(host, port)) 
{}

void HttpSession::post_data(const std::string& endpoint, const std::string& data) {
  namespace http = boost::beast::http;
  http::request<http::string_body> req{http::verb::post, endpoint, 11};
  req.set(http::field::host, connection_impl_->host);
  req.body() = data;
  req.prepare_payload();

  http::write(connection_impl_->stream, req);
  boost::beast::flat_buffer buffer;
	http::response<http::string_body> response;
	http::read(connection_impl_->stream, buffer, response);
  if (response.result() != http::status::ok) {
    throw std::runtime_error("HTTP POST request failed endpoint=" + endpoint);
  }
}

HttpSession::~HttpSession() = default;

}
