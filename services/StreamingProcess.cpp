#include "StreamingProcess.h"

using namespace services;

StreamingProcess::StreamingProcess( QTcpSocket* client ) : GenericProcess( client )
{
    this->client->moveToThread( this );
}

StreamingProcess::~StreamingProcess()
{

}

void StreamingProcess::run()
{
    QByteArray response;
    QString filename;
    QString sessionID;
    QString username;
    qint64 rangeStart = -1;

    if ( this->client->bytesAvailable() == 0)
        this->client->waitForReadyRead();

    QString recivedData = this->client->readAll();
    if ( recivedData.isEmpty() )
        return;


    QStringList data = recivedData.split("\n");
    for ( int i = 0; i < data.count(); i++ )
    {
        if ( data[i].contains("GET ") )
        {
            QString getRequest = data[i].section( " ", 1 , 1 );
            filename = getRequest.section( "/", -1 ).replace("+", " ");
            sessionID = getRequest.section( "/", 1, 1 );
            username = getRequest.section( "/", 2, 2 );
        }
        else if ( data[i].contains( "Range: " ) )
        {
            rangeStart = data[i].section( "=", 1, 1 ).section("-", 0, 0).toLongLong();
        }
    }

    if ( username.isEmpty() || sessionID.isEmpty() || filename.isEmpty() )
    {
        response = "HTTP/1.1 400 Bad Request\r\n";
        response += "Date: Sun, 01 Feb 2015 17:41:33 GMT\r\n";
        response += "Server: MEMC Server\r\n";
        response += "Content-Length: 13\r\n";
        response += "Connection: close\n\r\n";
        response += "404 Bad Request";
        this->client->write(response);
        this->client->waitForBytesWritten();
        return;
    }

    UserManager* userManager = UserManager::getInstance();
    if( !userManager->getUser( username )->isOnLine( sessionID.toInt() ) )
    {
        response = "HTTP/1.1 403 Forbidden\r\n";
        response += "Date: Sun, 01 Feb 2015 17:41:33 GMT\r\n";
        response += "Server: MEMC Server\r\n";
        response += "Content-Length: 13\r\n";
        response += "Connection: close\n\r\n";
        response += "403 Forbidden";
        this->client->write(response);
        this->client->waitForBytesWritten();
        return;
    }
    User *user = userManager->getUser( username );
    MediaFile *media = user->getFile( filename );
    if ( media == nullptr )
    {
        this->return404();
        return;
    }

    QFile mediaFile( media->getPath() );
    if ( !mediaFile.open( QIODevice::ReadOnly ) )
    {
        this->return404();
        return;
    }

    qint64 mediaSize = media->getSize();
    qint64 contentLenght = mediaSize;

    if (rangeStart == -1)
        response += "HTTP/1.1 200 OK\r\n";
    else
    {
        contentLenght -= rangeStart;
        response += "HTTP/1.1 206 Partial Content\r\n";
    }

    response += "Server: MEMC Server\r\n";
    response += "Accept-Ranges: bytes\r\n";
    response += "Content-Length: "+ QString::number( contentLenght )  + "\r\n";

    if ( rangeStart != -1 )
        response += "Content-Range: bytes "+ QString::number( rangeStart ) + "-"+ QString::number( mediaSize-1 ) + "/"+ QString::number( mediaSize ) + "\r\n";

    response += "Connection: close\r\n";

    if ( media->getName().section( ".", -1 ) == "mkv" )
    {
        response += "Content-Type: video/x-matroska\r\n";
    }
    else if ( media->getName().section( ".", -1 ) == "avi" )
    {
        response += "Content-Type: video/avi\r\n";
    }
    else if ( media->getName().section( ".", -1 ) == "mp3" )
    {
        response += "Content-Type: audio/mpeg;\r\n";
    }
    else if ( media->getName().section( ".", -1 ) == "ogg" )
    {
        response += "Content-Type: audio/ogg\r\n";
    }

    response += "\r\n";

    if (rangeStart != -1)
        mediaFile.seek( rangeStart );

    bool connectionClose = false;
    while ( !mediaFile.atEnd() && !connectionClose )
    {
        response += mediaFile.read( 1024 );
        if ( this->client->state() != QAbstractSocket::UnconnectedState )
        {
            this->client->write( response );
            this->client->waitForBytesWritten();
            response = "";
        }
        else
            connectionClose = true;
    }
    this->client->close();
    mediaFile.close();
}


void StreamingProcess::closeConnection()
{

}

void StreamingProcess::return404()
{
    QByteArray response;
    response = "HTTP/1.1 404 Not Found\r\n";
    response += "Date: Sun, 01 Feb 2015 17:41:33 GMT\r\n";
    response += "Server: MEMC Server\r\n";
    response += "Content-Length: 13\r\n";
    response += "Connection: close\n\r\n";
    response += "404 Not Found";

    this->client->write(response);
    this->client->waitForBytesWritten();
}
