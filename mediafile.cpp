#include "mediafile.h"


QString MediaFile::getOwner() const
{
    return owner;
}

void MediaFile::setOwner(const QString& owner )
{
    this->owner = owner;
}

QString MediaFile::getPath() const
{
    return path;
}

void MediaFile::setPath(const QString& path )
{
    this->path = path;
}

FileType MediaFile::getType() const
{
    return type;
}

void MediaFile::setType(const FileType& type )
{
    this->type = type;
}

unsigned long MediaFile::getSize() const
{
    return size;
}

void MediaFile::setSize(unsigned long size )
{
    size = size;
}

bool MediaFile::isPublic() const
{
    return _public;
}

void MediaFile::set_Public(bool _public)
{
    this->_public = _public;
}
MediaFile::MediaFile()
{

}

MediaFile::~MediaFile()
{

}

