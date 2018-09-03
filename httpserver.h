#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <string>

class HTTPServerPrivate;

/**
 * @brief The HttpServer class provides interface for simple server,
 *        which can share files on client request.
 */
class HttpServer
{
public:
    HttpServer(): server(nullptr){}
    ~HttpServer();

    /**
     * @brief start start server at path:port.
     * @param path - path of the server.
     * @param port - port of the server.
     * @return 0 when finished.
     */
    int start( const std::string &path, const std::string &port );

private:
    /**
     * @brief server - api.
     */
    HTTPServerPrivate *server;
};

#endif // HTTPSERVER_H
