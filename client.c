/***
Very simple TCP client. The first argument is host name, than port.
Maximum message size is 256 bytes.
***/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <string.h> /* memset, memcpy */

#define MAX_MESSAGE_SIZE 256

int ReportError( const char * msg )
{
    fprintf( stderr,"%s\n", msg );   
    return 1;
}

int main(int argc, char *argv[])
{
    int socketId, portNumber, n;
    struct sockaddr_in serverAdress;
    struct hostent *server;
    char buffer[256];
    if (argc < 3) 
        return ReportError( "Usage client hostname port" );
    portNumber = atoi(argv[2]);
    socketId = socket(AF_INET, SOCK_STREAM, 0);
    if ( socketId < 0 )
        return ReportError( "ERROR: opening socket failed" );
    server = gethostbyname( argv[1] );
    if ( server == NULL )
        return ReportError( "ERROR: no such host" );   
    memset( &serverAdress, 0 , sizeof(serverAdress) );
    serverAdress.sin_family = AF_INET;
    memcpy( &serverAdress.sin_addr.s_addr, server->h_addr, server->h_length );
    serverAdress.sin_port = htons( portNumber ); // Using network byte order
    if ( connect( socketId, (struct sockaddr *)&serverAdress, sizeof(serverAdress) ) < 0 ) 
        return ReportError( "ERROR: conecting failed" );
    printf("Please enter the message: ");
    memset( buffer, 0 , MAX_MESSAGE_SIZE );
    fgets( buffer, MAX_MESSAGE_SIZE-1, stdin );
    n = write( socketId,buffer, strlen(buffer) );
    if ( n < 0 ) 
        return ReportError( "ERROR: writing to socket failed" );
    bzero(buffer,256);
    memset( buffer, 0 , MAX_MESSAGE_SIZE );
    n = read( socketId, buffer, MAX_MESSAGE_SIZE - 1 );
    if ( n < 0 )
        return ReportError( "ERROR: reading from socket failed" );
    printf( "%s\n", buffer );
    return 0;
}
