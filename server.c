/***
Very simple TCP server. The port number is passed as an argument.
It closes after processing one client. Maximum message size is 256 bytes.
***/
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h> /* memset */

#define MAX_MESSAGE_SIZE 256

int ReportError( const char * msg )
{
    fprintf( stderr,"%s\n", msg );   
    return 1;
}

int main(int argc, char *argv[])
{
    int socketId, newsocketId, portNumber, client;
    char buffer[MAX_MESSAGE_SIZE];
    struct sockaddr_in serverAdress, clientAdress;
    int n;
    if (argc < 2) 
        return ReportError( "Usage server port" );
    socketId = socket( AF_INET, SOCK_STREAM, 0 );
    if ( socketId < 0 )
        return ReportError( "ERROR: opening socket" );
    memset( &serverAdress, 0 , sizeof(serverAdress) );
    portNumber = atoi( argv[1] );
    serverAdress.sin_family = AF_INET;
    serverAdress.sin_addr.s_addr = INADDR_ANY;
    serverAdress.sin_port = htons( portNumber ); // Using network byte order
    if ( bind( socketId, (struct sockaddr *) &serverAdress, sizeof(serverAdress) ) < 0) 
        return ReportError( "ERROR: on binding" );
    listen( socketId,5 );
    client = sizeof( clientAdress );
    newsocketId = accept( socketId, (struct sockaddr *) &clientAdress, &client );
    if ( newsocketId < 0 ) 
        return ReportError( "ERROR: accept failed" );
    memset( buffer, 0 , MAX_MESSAGE_SIZE );
    n = read( newsocketId, buffer, MAX_MESSAGE_SIZE - 1 );
    if ( n < 0 ) 
        return ReportError( "ERROR: accept failed" );
    printf("Client send following message: %s\n",buffer);
    char* confirmationMessage = "I got your message"; 
    n = write( newsocketId, confirmationMessage, strlen( confirmationMessage ) );
    if ( n < 0 )
        return ReportError( "ERROR: writing to socket failed" );
    return 0; 
}
