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
     * @return  0  - success finish;
     *          -1 - dir path error;
     *          -2 - address initerror;
     *          -3 - socket init error.
     */
    int start( const std::string &path, const std::string &port );

    /**
     * @brief setSharefoulderPath - setting path for sharing files.
     * @param path - dir path to share place.
     */
    void setSharefoulderPath( const std::string &path );

private:
    /**
     * @brief server - api.
     */
    HTTPServerPrivate *server;

    /**
     * @brief sharefolderPath - path for sharing.
     */
    std::string sharefolderPath;
};

#endif // HTTPSERVER_H
