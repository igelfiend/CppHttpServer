#include "httpserver.h"

int main()
{
    HttpServer server;

    server.setSharefoulderPath( "D:/Pics2" );
    server.start( "127.0.0.1", "8000" );
    return 0;
}
