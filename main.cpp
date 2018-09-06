#include <iostream>
#include <string>

#include "httpserver.h"

using namespace std;

int main(int argc, char *argv[])
{
    if( argc < 4 )
    {
        cerr << "to few arguments count" << endl;
        return -1;
    }

    string strAddres( argv[ 1 ] );
    string strPort  ( argv[ 2 ] );
    string dirname  ( argv[ 3 ] );

    HttpServer server;
    server.setSharefoulderPath( dirname );
    server.start( strAddres, strPort );
    return 0;
}
