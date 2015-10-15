#include "mediaPlayer.h"


mPlayer::mPlayer(QQuickItem* parent)
    :QQuickItem(parent)
{
    player=new QMediaPlayer();
    playerList=new QMediaPlaylist();
    settings=new QSettings(QString("/home/nemo/Music/playlist.ini"), QSettings::IniFormat);
}

mPlayer::~mPlayer()
{
    delete player;
    delete playerList;
    delete settings;
}

void mPlayer::addToPlaylist(const QVariant& song, const QString& songUrl)
{    
    //save to the ini file;
    settings->setValue(song.toString(), songUrl);
    qDebug()<<"0000000";
}

void mPlayer::playSingle(const QString& songName,const QVariant& song)
{

    playerList->clear();
    player->stop();
    QDir dir;
    QString url=dir.homePath()+QString("/Music/")+songName+QString(".mp3");
    QFileInfo fileInfo(url);
    qDebug()<<fileInfo.absoluteFilePath();
    if(fileInfo.exists() && fileInfo.isFile()){
        qDebug()<<"123";
        playerList->addMedia(QUrl::fromLocalFile(url));
    }else{
        player->setMedia(song.toUrl());
        qDebug()<<"456";
    }
    player->play();
}

void mPlayer::savePlaylist(const QString& songName, const QString& songUrl)
{
    qDebug()<<"789";
    settings->setValue(songName, songUrl);
}

qint64 mPlayer::readSettings()
{
    songs=settings->allKeys();
    for(const QString& songName : songs){
        qDebug()<<songName;
        songsUrl.append(settings->value(songName).toString());
    }
    qint64 number=songs.size();
    qDebug()<<"11111111";
    return number;
}

QString mPlayer::getTheSong(const int& index)
{
    return songs.at(index);
}

QString mPlayer::getTheSongUrl(const int& index)
{
    return songsUrl.at(index);
}
