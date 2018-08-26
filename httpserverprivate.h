#ifndef HTTPSERVERPRIVATE_H
#define HTTPSERVERPRIVATE_H

#include <string>

#include <winsock2.h>
#include <ws2tcpip.h>

using std::string;

/**
 * @brief The HTTPServerPrivate class provides api for HTTP server
 */
class HTTPServerPrivate
{
public:
    HTTPServerPrivate(): listen_socket(0), addr(nullptr){}

    /**
     * @brief start - starting loop for processing clients requests
     * @param path - host path
     * @param port - host port
     * @return
     */
    int start(const string &path, const string &port);

private:
    /**
     * @brief initAddress - initing address structure
     * @param path - host path
     * @param port - host port
     * @return true on success, false on fail
     */
    bool initAddress(const string &path, const string &port);

    /**
     * @brief initSocket - initing socket for address
     * @return true on success, false on fail
     */
    bool initSocket();

    /**
     * @brief startProcessLoop - initing processing clients requests
     */
    void startProcessLoop();

    /**
     * @brief getFilenameFromResponse - reading filename for download from client request
     * @param response - response text
     * @return filename, empty string in error case
     */
    string getFilenameFromResponse( char response[] );

    /**
     * @brief readFile - Reading file from filepath to buffer
     * @param filepath - path to file
     * @param buffer   - stores readed data
     * @return  readed size
     */
    char *readFile( const string &filepath, __int64 &size );

    /**
     * @brief close - Free all data
     */
    void close();

    /**
     * @brief listen_socket - Socket for listner
     */
    SOCKET listen_socket;

    /**
     * @brief addr - Storing information about listner IP-address
     */
    struct addrinfo* addr;
};

#endif // HTTPSERVERPRIVATE_H
