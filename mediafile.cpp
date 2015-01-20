#include "mediafile.h"

MediaFile::MediaFile()
{
}

QString MediaFile::getOwner() const
{
    return owner;
}

void MediaFile::setOwner( const QString& owner )
{
    this->owner = owner;
}

QString MediaFile::getPath() const
{
    return path;
}

void MediaFile::setPath( const QString& path )
{
    this->path = path;
}

FileType MediaFile::getType() const
{
    return type;
}

void MediaFile::setType( const FileType& type )
{
    this->type = type;
}

qint64 MediaFile::getSize() const
{
    return size;
}

void MediaFile::setSize(qint64 size )
{
    this->size = size;
}

bool MediaFile::isPublic() const
{
    return _public;
}

void MediaFile::set_Public(bool _public)
{
    this->_public = _public;
}

QString MediaFile::getName() const
{
    return name;
}

void MediaFile::setName(const QString& name)
{
    this->name = name;
}


MediaFile::~MediaFile()
{
    
}

