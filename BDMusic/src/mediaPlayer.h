#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QtGui>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QQuickItem>
#include <QSettings>

class mPlayer:public QQuickItem{
    Q_OBJECT

    //set mediaplay duration and positon from qml;
    /*Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged)
    Q_PROPERTY (qint64 position READ position WRITE setPositon NOTIFY positionChanged)

    //get mediaPlay's information;
    Q_PROPERTY(QString singer READ singer)*/

public:
    mPlayer(QQuickItem* parent=0);
    ~mPlayer();
    Q_INVOKABLE void playSingle(const QString& songName,const QVariant& song);
    Q_INVOKABLE QString getTheSong(const int& index);
    Q_INVOKABLE QString getTheSongUrl(const int& index);
    Q_INVOKABLE void savePlaylist(const QString& songName, const QString& songUrl);//to ini file;
    /*qint64 duration()const;//get current song's duration;
    qint64 position()const;//get current time(which is playing);
    QString Singer()const;*/
    Q_INVOKABLE qint64 readSettings();
private:
    QMediaPlayer *player;
    QMediaPlaylist *playerList;
    QStringList songsUrl;
    QStringList songs;
    QSettings* settings;
public slots:
    /*void pause();
    void play();
    void stop();

    void setPosition(const qint64& position);
    void playSingle(const QString& artist, const QString& song, const QString& source);
    void playPlaylist(const QVariant& songsOfList);*/
    void addToPlaylist(const QVariant& songId, const QString& songUrl);
/*signals:
    void durationChanged(const qint64& currentDuration);
    void positionChanged(const qint64& currentPositon);*/
};

#endif // MEDIAPLAYER_H
