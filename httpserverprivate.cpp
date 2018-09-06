#include "httpserverprivate.h"

#include <iostream>
#include <sstream>

#include <fstream> // for reading file

#include "utilities.h"

// Required for stable freeaddrinfo in MinGW
// details: http://stackoverflow.com/a/20306451

#define _WIN32_WINNT 0x501

// Links winsock2 lib
#pragma comment(lib, "Ws2_32.lib")

using std::cerr;
using std::cout;
using std::endl;

int HTTPServerPrivate::start(const std::string &path, const std::string &port)
{
    // Checking share path
    if( !Utilities::dirExists( sharefoulderPath ) )
    {
        cerr << sharefoulderPath << ": directory doesn\'t exists." << endl;
        return -1;
    }

    // Initing address
    if( !initAddress( path, port ) )
    {
        return -2;
    }

    // Initing socket
    if( !initSocket() )
    {
        return -3;
    }

    // Start catching request and responding to them
    startProcessLoop();
    return 0;
}

void HTTPServerPrivate::setSharefoulderPath(const std::string &path)
{
    sharefoulderPath = path;
}

bool HTTPServerPrivate::initAddress(const std::string &path, const std::string &port)
{
    WSADATA wsaData; // Struct for store service information

    // Uploading ws2_32.dll
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (result != 0)
    {
        cerr << "WSAStartup failed: " << result << endl;
        return false;
    }

    addr = nullptr;

    // initing address struct
    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));

    hints.ai_family   = AF_INET;        // AF_INET select web for working with socket
    hints.ai_socktype = SOCK_STREAM;    // Stream type socket
    hints.ai_protocol = IPPROTO_TCP;    // Using TCP/IP protocol
    hints.ai_flags    = AI_PASSIVE;     // Socket will binds on adress for receiving messages

    // Initing address struct
    result = getaddrinfo(path.c_str(), port.c_str(), &hints, &addr);

    // On error close all
    if (result != 0)
    {
        cerr << "getaddrinfo failed: " << result << endl;
        close();
        return false;
    }

    return true;
}

bool HTTPServerPrivate::initSocket()
{
    // Creating socket
    listenSocket = socket( addr->ai_family,
                           addr->ai_socktype,
                           addr->ai_protocol );

    // On error free memory
    if( listenSocket == INVALID_SOCKET )
    {
        cerr << "Error at socket: " << WSAGetLastError() << endl;
        listenSocket = 0;

        close();
        return false;
    }

    // Binding socket to IP-address
    int result = bind( listenSocket,
                       addr->ai_addr,
                       int( addr->ai_addrlen ) );

    // If binding failed, free memory
    if( result == SOCKET_ERROR )
    {
        cerr << "bind failed with error: " << WSAGetLastError() << endl;

        close();
        return false;
    }

    // Initing listner socket
    if( listen( listenSocket, SOMAXCONN ) == SOCKET_ERROR )
    {
        cerr << "listen failed with error: " << WSAGetLastError() << endl;

        close();
        return false;
    }

    return true;
}

void HTTPServerPrivate::startProcessLoop()
{
    const int maxClientBufferSize = 1024;
    char buf[ maxClientBufferSize ];
    SOCKET clientSocket = INVALID_SOCKET;

    cout << "server started." << endl;
    cout << "sharing path: " << sharefoulderPath << endl;
    while( true )
    {
        // Receiving incomming connection
        clientSocket = accept(listenSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET)
        {
            cerr << "Accept failed: " << WSAGetLastError() << endl;
            close();
            return;
        }

        int result = recv(clientSocket, buf, maxClientBufferSize, 0);

        // On receving data error
        if( result == SOCKET_ERROR )
        {
            cerr << "Recv failed: " << result << endl;
            cerr << "Error code:  " << WSAGetLastError() << endl;

            close();
            return;
        }
        else if( result == 0 )
        {
            // Connection closed by client
            cerr << "Connection closed..." << endl;
        }
        else if( result > 0 )
        {
            // Pasting endline in end of data in buffer.
            buf[ result ] = '\0';
            std::cout << "Received: " << buf << endl;

            // generate response for client
            std::stringstream response = generateResponse( buf );

            // Sending data to client
            result = send(  clientSocket, response.str().c_str(),
                            response.str().length(), 0);

            // If there are error on sending data
            if( result == SOCKET_ERROR )
            {
                cerr << "send failed: " << WSAGetLastError() << endl;
            }

            // Closing connection with client
            closesocket(clientSocket);
        }
    }
}

std::string HTTPServerPrivate::getFilenameFromResponse(char response[])
{
    string s( response );
    vector<string> lines = Utilities::split( s, '\n' );

    if( lines.size() == 0 )
    {
        cerr << "Received empty response data." << endl;
        return string();
    }

    vector<string> fragments = Utilities::split( lines.front(), ' ' );

    if( fragments.size() < 3 )
    {
        cerr << "Response first line has to few parameters." << endl;
        return string();
    }

    string filepath = fragments[ 1 ];
    filepath.erase( 0, 1 );

    return filepath;
}

char *HTTPServerPrivate::readFile( const string &filepath, __int64 &size )
{
    std::ifstream is( filepath.c_str(), std::ifstream::binary );

    if( !is.is_open() )
    {
        cerr << "File not found!" << endl;
        return nullptr;
    }

    is.seekg( 0, is.end );
    size = is.tellg();
    is.seekg( 0, is.beg );

    char *buffer = new char[ size ];

    is.read( buffer, size );
    is.close();

    return buffer;
}

std::stringstream HTTPServerPrivate::generateResponse(char buffer[])
{
    string filename = getFilenameFromResponse( buffer );

    if( filename.empty() )
    {
        cerr << "Get request error." << endl;
        return generateResponseWithError();
    }

    __int64 size = 0;
    char *data = readFile( sharefoulderPath + "/" + filename, size );

    if( !data )
    {
        cerr << "Error reading file." << endl;
        return generateResponseWithError();
    }

    std::stringstream response;
    std::stringstream responseBody;

    responseBody.write( data, size );

    // Generating response
    response    << "HTTP/1.1 200 OK\r\n"
                << "Version: HTTP/1.1\r\n"
                << "Host: 127.0.0.1\r\n"
                << "Connection: close\r\n"
                << "Content-Type: application/octet-stream\r\n"
                << "Content-Disposition: form-data; filename=\""
                << filename << "\";\r\n\r\n"
                << responseBody.str();

    return response;
}

std::stringstream HTTPServerPrivate::generateResponseWithError()
{
    std::stringstream response;
    response    << "HTTP/1.1 404 Not found\r\n"
                << "Version: HTTP/1.1\r\n"
                << "Host: 127.0.0.1\r\n";
    return response;
}

void HTTPServerPrivate::close()
{
    if( addr ) freeaddrinfo( addr );
    if( listenSocket != 0 ) closesocket( listenSocket );
    WSACleanup(); // uploading ws2_32.dll
}
