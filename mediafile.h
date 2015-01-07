#ifndef MEDIAFILE_H
#define MEDIAFILE_H

#include <QString>

enum FileType { MUSIC = 0, VIDEO, IMAGE };

class MediaFile
{
private:
    QString name;
    QString owner;
    QString path;
    FileType type;
    unsigned long size;
    bool _public;

public:
    MediaFile();

    QString getName() const;
    void setName( const QString& name );

    QString getOwner() const;
    void setOwner( const QString& owner );

    QString getPath() const;
    void setPath( const QString& path );

    FileType getType() const;
    void setType( const FileType& type );

    unsigned long getSize() const;
    void setSize( unsigned long size );

    bool isPublic() const;
    void set_Public( bool _public );

    ~MediaFile();
};

#endif // MEDIAFILE_H
