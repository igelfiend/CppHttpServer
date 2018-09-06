#include "httpserver.h"

#include "httpserverprivate.h"

HttpServer::~HttpServer()
{
    delete server;
}

int HttpServer::start(const std::string &path, const std::string &port)
{
    if( server )
    {
        delete server;
    }

    server = new HTTPServerPrivate();
    server->setSharefoulderPath( sharefolderPath );
    return server->start( path, port );
}

void HttpServer::setSharefoulderPath(const std::string &path)
{
    sharefolderPath = path;
}
