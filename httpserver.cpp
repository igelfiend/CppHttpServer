#include "httpserver.h"

#include "httpserverprivate.h"

int HTTPServer::start()
{

    HTTPServerPrivate server;
    server.start( "127.0.0.1", "8000" );

    return 0;
}
