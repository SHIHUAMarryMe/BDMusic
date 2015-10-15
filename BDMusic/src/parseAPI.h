#ifndef PARSEAPI_H
#define PARSEAPI_H

#include <QtGui>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QVariant>
#include <QQuickItem>
#include <QDebug>
#include <QList>
#include <QRegExp>

class parseAPI:public QQuickItem{
    Q_OBJECT
public:
    parseAPI(QQuickItem* parent=0);
    ~parseAPI();
    Q_INVOKABLE void parseJSON(const QVariant& json);
    Q_INVOKABLE QString parseSongsJSON(const QVariant& json);
    Q_INVOKABLE void parseLyricJSON(const QVariant& json);
    Q_INVOKABLE qint64 getCurrentId(const int& index);//songs id;
    Q_INVOKABLE qint64 getTotalSongsNumber();
    Q_INVOKABLE void regLyric();
public slots:
    //QVariant getSongInfoJSON(const QVariant& code);
    QVariant search(const QVariant& keyword);
    QVariant albumsByArtist(const QVariant& code);
    QVariant songInAlbums(const QVariant& code);
    QVariant lyric(const QVariant& code);
    QVariant song(const QVariant& code);
    QString getSongsImgUrl(const QVariant& json);

    QString getCurrentArtistName(const int& index);
    QString getCurrentSongName(const int& index);
    qint64 getCurrentArtistId(const int& index);

    //qint64 progress(const qint64& read, const qint64& total);
    void save(const QVariant& url, const QVariant& SongName);
private:
    QNetworkAccessManager network;

    QNetworkAccessManager* downloader;

    QString name;//for saving;
    QStringList songsName;//get singers' name for display on listView;

    //for listView;
    QList<qint64> songsID;
    QString songs;
    QString songPicUrl;
    QList<qint64> artistsId;
    QStringList artistsName;
    QString theLyric;//get this from JSON;
    QString lyricList;
private slots:
    void write(QNetworkReply* rply);//for downloading;
/*signals:
    void progressChanged(const qint64& read, const qint64& total);*/
};

#endif // PARSEAPI_H
