#include "httpserver.h"

int main()
{
    HttpServer server;

    server.start( "127.0.0.1", "8000" );
    return 0;
}
