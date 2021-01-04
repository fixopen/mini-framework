#include "WebServer.h"

#include <thread> // for std::thread
#include <cstdlib>
#include <iostream>
#include <memory>

#include <WinSock2.h> // for socket api

#include "../microhttpd.h"

//#include <boost/beast.hpp>
//#include <boost/beast/core.hpp>
//#include <boost/beast/http.hpp>
//#include <boost/beast/version.hpp>
//#include <boost/asio/ip/tcp.hpp>
//#include <boost/config.hpp>

//namespace beast = boost::beast;         // from <boost/beast.hpp>
//namespace http = beast::http;           // from <boost/beast/http.hpp>
//namespace net = boost::asio;            // from <boost/asio.hpp>
//using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

namespace utils {
    /*
    namespace {
        beast::string_view mime_type(beast::string_view path) {
            using beast::iequals;
            auto const ext = [&path] {
                auto const pos = path.rfind(".");
                if (pos == beast::string_view::npos)
                    return beast::string_view{};
                return path.substr(pos);
            }();
            if (iequals(ext, ".htm"))  return "text/html";
            if (iequals(ext, ".html")) return "text/html";
            if (iequals(ext, ".php"))  return "text/html";
            if (iequals(ext, ".css"))  return "text/css";
            if (iequals(ext, ".txt"))  return "text/plain";
            if (iequals(ext, ".js"))   return "application/javascript";
            if (iequals(ext, ".json")) return "application/json";
            if (iequals(ext, ".xml"))  return "application/xml";
            if (iequals(ext, ".swf"))  return "application/x-shockwave-flash";
            if (iequals(ext, ".flv"))  return "video/x-flv";
            if (iequals(ext, ".png"))  return "image/png";
            if (iequals(ext, ".jpe"))  return "image/jpeg";
            if (iequals(ext, ".jpeg")) return "image/jpeg";
            if (iequals(ext, ".jpg"))  return "image/jpeg";
            if (iequals(ext, ".gif"))  return "image/gif";
            if (iequals(ext, ".bmp"))  return "image/bmp";
            if (iequals(ext, ".ico"))  return "image/vnd.microsoft.icon";
            if (iequals(ext, ".tiff")) return "image/tiff";
            if (iequals(ext, ".tif"))  return "image/tiff";
            if (iequals(ext, ".svg"))  return "image/svg+xml";
            if (iequals(ext, ".svgz")) return "image/svg+xml";
            return "application/text";
        }

        // Append an HTTP rel-path to a local filesystem path.
        // The returned path is normalized for the platform.
        std::string path_cat(beast::string_view base, beast::string_view path) {
            if (base.empty())
                return std::string(path);
            std::string result(base);
#ifdef BOOST_MSVC
            char constexpr path_separator = '\\';
            if (result.back() == path_separator)
                result.resize(result.size() - 1);
            result.append(path.data(), path.size());
            for (auto& c : result)
                if (c == '/')
                    c = path_separator;
#else
            char constexpr path_separator = '/';
            if (result.back() == path_separator)
                result.resize(result.size() - 1);
            result.append(path.data(), path.size());
#endif
            return result;
        }

        // This function produces an HTTP response for the given
        // request. The type of the response object depends on the
        // contents of the request, so the interface requires the
        // caller to pass a generic lambda for receiving the response.
        template<
            class Body, class Allocator,
            class Send>
            void
            handle_request(
                beast::string_view doc_root,
                http::request<Body, http::basic_fields<Allocator>>&& req,
                Send&& send) {
            // Returns a bad request response
            auto const bad_request =
                [&req](beast::string_view why) {
                http::response<http::string_body> res{ http::status::bad_request, req.version() };
                res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                res.set(http::field::content_type, "text/html");
                res.keep_alive(req.keep_alive());
                res.body() = std::string(why);
                res.prepare_payload();
                return res;
            };

            // Returns a not found response
            auto const not_found =
                [&req](beast::string_view target) {
                http::response<http::string_body> res{ http::status::not_found, req.version() };
                res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                res.set(http::field::content_type, "text/html");
                res.keep_alive(req.keep_alive());
                res.body() = "The resource '" + std::string(target) + "' was not found.";
                res.prepare_payload();
                return res;
            };

            // Returns a server error response
            auto const server_error =
                [&req](beast::string_view what) {
                http::response<http::string_body> res{ http::status::internal_server_error, req.version() };
                res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                res.set(http::field::content_type, "text/html");
                res.keep_alive(req.keep_alive());
                res.body() = "An error occurred: '" + std::string(what) + "'";
                res.prepare_payload();
                return res;
            };

            // Make sure we can handle the method
            if (req.method() != http::verb::get &&
                req.method() != http::verb::head)
                return send(bad_request("Unknown HTTP-method"));

            // Request path must be absolute and not contain "..".
            if (req.target().empty() ||
                req.target()[0] != '/' ||
                req.target().find("..") != beast::string_view::npos)
                return send(bad_request("Illegal request-target"));

            // Build the path to the requested file
            std::string path = path_cat(doc_root, req.target());
            if (req.target().back() == '/')
                path.append("index.html");

            // Attempt to open the file
            beast::error_code ec;
            http::file_body::value_type body;
            body.open(path.c_str(), beast::file_mode::scan, ec);

            // Handle the case where the file doesn't exist
            if (ec == beast::errc::no_such_file_or_directory)
                return send(not_found(req.target()));

            // Handle an unknown error
            if (ec)
                return send(server_error(ec.message()));

            // Cache the size since we need it after the move
            auto const size = body.size();

            // Respond to HEAD request
            if (req.method() == http::verb::head) {
                http::response<http::empty_body> res{ http::status::ok, req.version() };
                res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                res.set(http::field::content_type, mime_type(path));
                res.content_length(size);
                res.keep_alive(req.keep_alive());
                return send(std::move(res));
            }

            // Respond to GET request
            http::response<http::file_body> res{
                std::piecewise_construct,
                std::make_tuple(std::move(body)),
                std::make_tuple(http::status::ok, req.version()) };
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, mime_type(path));
            res.content_length(size);
            res.keep_alive(req.keep_alive());
            return send(std::move(res));
        }

        // Report a failure
        void fail(beast::error_code ec, char const* what) {
            std::cerr << what << ": " << ec.message() << "\n";
        }

        // This is the C++11 equivalent of a generic lambda.
        // The function object is used to send an HTTP message.
        template<class Stream>
        struct send_lambda {
            Stream& stream_;
            bool& close_;
            beast::error_code& ec_;

            explicit
                send_lambda(
                    Stream& stream,
                    bool& close,
                    beast::error_code& ec)
                : stream_(stream)
                , close_(close)
                , ec_(ec) {}

            template<bool isRequest, class Body, class Fields>
            void operator()(http::message<isRequest, Body, Fields>&& msg) const {
                // Determine if we should close the connection after
                close_ = msg.need_eof();

                // We need the serializer here because the serializer requires
                // a non-const file_body, and the message oriented version of
                // http::write only works with const messages.
                http::serializer<isRequest, Body, Fields> sr{ msg };
                http::write(stream_, sr, ec_);
            }
        };

        // Handles an HTTP server connection
        void do_session(tcp::socket& socket, std::shared_ptr<std::string const> const& doc_root) {
            bool close = false;
            beast::error_code ec;

            // This buffer is required to persist across reads
            beast::flat_buffer buffer;

            // This lambda is used to send messages
            send_lambda<tcp::socket> lambda{ socket, close, ec };

            for (;;) {
                // Read a request
                http::request<http::string_body> req;
                http::read(socket, buffer, req, ec);
                if (ec == http::error::end_of_stream)
                    break;
                if (ec)
                    return fail(ec, "read");

                // Send the response
                handle_request(*doc_root, std::move(req), lambda);
                if (ec)
                    return fail(ec, "write");
                if (close) {
                    // This means we should close the connection, usually because
                    // the response indicated the "Connection: close" semantic.
                    break;
                }
            }

            // Send a TCP shutdown
            socket.shutdown(tcp::socket::shutdown_send, ec);

            // At this point the connection is closed gracefully
        }
    }
    */

    namespace {
        int answer_to_connection(void* cls, struct MHD_Connection* connection,
            const char* url,
            const char* method, const char* version,
            const char* upload_data,
            size_t* upload_data_size, void** con_cls) {
            const char* page = "<html><body>Hello, browser!</body></html>";
            struct MHD_Response* response;
            int ret;

            response = MHD_create_response_from_buffer(strlen(page),
                (void*)page, MHD_RESPMEM_PERSISTENT);
            ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
            MHD_destroy_response(response);

            return ret;
        }
    }

    WebServer::WebServer(std::string const& host, unsigned short port, std::string const& root) {
        auto t = std::thread([&host, &port, &root] {
            try {
                /*
                auto const address = net::ip::make_address(host);
                // auto const port = static_cast<unsigned short>(port);
                auto const doc_root = std::make_shared<std::string>(root);

                // The io_context is required for all I/O
                net::io_context ioc{ 1 };

                // The acceptor receives incoming connections
                tcp::acceptor acceptor{ ioc, {address, port} };
                for (;;) {
                    // This will receive the new connection
                    tcp::socket socket{ ioc };

                    // Block until we get a connection
                    acceptor.accept(socket);

                    // Launch the session, transferring ownership of the socket
                    // std::thread{ std::bind(&do_session, std::move(socket), doc_root) }.detach();
                }
                */
                /*
                // Create a socket (IPv4, TCP)
                SOCKET sockfd = socket(AF_INET, SOCK_STREAM, 0);
                if (sockfd == -1) {
                    std::cout << "Failed to create socket. errno: " << errno << std::endl;
                    exit(EXIT_FAILURE);
                }

                // Listen to port 9999 on any address
                sockaddr_in sockaddr;
                sockaddr.sin_family = AF_INET;
                sockaddr.sin_addr.s_addr = INADDR_ANY;
                sockaddr.sin_port = htons(9999); // htons is necessary to convert a number to
                                                 // network byte order
                if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
                    std::cout << "Failed to bind to port 9999. errno: " << errno << std::endl;
                    exit(EXIT_FAILURE);
                }

                // Start listening. Hold at most 10 connections in the queue
                if (listen(sockfd, 10) < 0) {
                    std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
                    exit(EXIT_FAILURE);
                }

                // Grab a connection from the queue
                auto addrlen = sizeof(sockaddr);
                // int connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
                SOCKET connection = accept(sockfd, (struct sockaddr*)&sockaddr, (int*)&addrlen);
                if (connection < 0) {
                    std::cout << "Failed to grab connection. errno: " << errno << std::endl;
                    exit(EXIT_FAILURE);
                }

                // Read from the connection
                char buffer[100];
                auto bytesRead = recv(connection, buffer, 100, 0);
                std::cout << "The message was: " << buffer;

                // Send a message to the connection
                std::string response = "Good talking to you\n";
                send(connection, response.c_str(), response.size(), 0);

                // Close the connections
                closesocket(connection);
                closesocket(sockfd);
                */
                struct MHD_Daemon* daemon;

                daemon = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD, port, NULL, NULL,
                    &answer_to_connection, NULL, MHD_OPTION_END);
                if (NULL == daemon) return 1;
                getchar();

                MHD_stop_daemon(daemon);
                return 0;
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
                return EXIT_FAILURE;
            }
        });
        t.detach();
    }
}
