#include "parseAPI.h"

parseAPI::parseAPI(QQuickItem* parent)
    :QQuickItem(parent)
{

}

parseAPI::~parseAPI()
{
    delete downloader;
}

QVariant parseAPI::search(const QVariant& keyword)
{
    QEventLoop eventLoop;
    QObject::connect(&network, SIGNAL(finished(QNetworkReply* )), &eventLoop, SLOT(quit()));
    qDebug()<<"开始循环事件";
    QNetworkRequest request(QUrl(QString("http://music.163.com/api/search/get/")));
    request.setRawHeader("Referer", "http://music.163.com");
    request.setRawHeader("Cookie", "appver=2.0.2");
    request.setRawHeader("Content-type","application/x-www-form-urlencoded");

    QByteArray parameter;
    parameter.append("s="+keyword.toByteArray());
    parameter.append("&offset=0&limit=24&type=1");

    QNetworkReply* reply=network.post(request, parameter);
    eventLoop.exec();
    if(reply->error() == QNetworkReply::NoError){
        QByteArray json=reply->readAll();
        return QVariant(json);
    }
    return QVariant("");
}

QVariant parseAPI::song(const QVariant& code)
{
    QEventLoop eventLoop;
    QObject::connect(&network, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QNetworkRequest request(QUrl(QString("http://music.163.com/api/song/detail/?id=" + code.toString() + "&ids=%5B" + code.toString() + "%5D")));
    request.setRawHeader("Referer", "http://music.163.com");
    request.setRawHeader("Cookie", "appver=2.0.2");
    request.setRawHeader("Content-type","application/x-www-form-urlencoded");

    QByteArray parameter;
    parameter.append("s=" + code.toString());
    parameter.append("&offset=0&limit=48&type=2");

    QNetworkReply* reply=network.post(request, parameter);
    eventLoop.exec();
    if(reply->error() == QNetworkReply::NoError){
        QByteArray json=reply->readAll();
        return QVariant(json);
    }
    return QVariant("");
}

QVariant parseAPI::albumsByArtist(const QVariant& code)//from the id who is from searchJSON's artists-id;
{
    // Create custom temporary event loop on stack
    QEventLoop eventLoop;

    // "quit()" the event-loop, when the network request "finished()"

    QObject::connect(&network, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // Prepare request
    QNetworkRequest request( QUrl( QString("http://music.163.com/api/artist/albums/" + code.toString() + "?offset=0&limit=16") ) );

    request.setRawHeader ("Referer", "http://music.163.com");
    request.setRawHeader ("Cookie", "appver=2.0.2");

    QNetworkReply *reply = network.get(request);
    eventLoop.exec();

    //Success
    if ( reply -> error() == QNetworkReply::NoError ) {
        //qDebug() << "Success" << reply -> readAll();
        return QVariant(reply -> readAll());
    }
    else {
        //Failure
        qDebug() << "Failure" << reply -> errorString();
    }
    return QVariant("");
}

QVariant parseAPI::songInAlbums(const QVariant& code)//专辑id=code;
{
    // Create custom temporary event loop on stack
    QEventLoop eventLoop;

    // "quit()" the event-loop, when the network request "finished()"

    QObject::connect(&network, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // the HTTP request
    qDebug()<<"http://music.163.com/api/album/" + code.toString();
    QNetworkRequest request( QUrl( QString("http://music.163.com/api/album/" + code.toString()) ) );
    request.setRawHeader("Referer", "http://music.163.com");
    request.setRawHeader("Cookie", "appver=2.0.2");

    QNetworkReply *reply = network.get(request);
    eventLoop.exec();

    //Success
    if ( reply -> error() == QNetworkReply::NoError ) {
        qDebug() << "Success" << reply -> readAll();
        return QVariant(reply -> readAll());
    }
    else {
        //Failure
        qDebug() << "Failure" << reply -> errorString();
    }
    return QVariant("");
}

QVariant parseAPI::lyric(const QVariant& code)
{
    // Create custom temporary event loop on stack
    QEventLoop eventLoop;

    // "quit()" the event-loop, when the network request "finished()"

    QObject::connect(&network, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // the HTTP request
    qDebug() << "http://music.163.com/api/album/" + code.toString();
    QNetworkRequest request( QUrl( QString("http://music.163.com/api/song/lyric?os=pc&id=" + code.toString() + "&lv=-1&kv=-1&tv=-1") ) );
    request.setRawHeader ("Referer", "http://music.163.com");
    request.setRawHeader ("Cookie", "appver=2.0.2");

    QNetworkReply *reply = network.get(request);
    eventLoop.exec();

    //Success
    if ( reply -> error() == QNetworkReply::NoError ) {
        return QVariant(reply -> readAll());
    }

    else {
        //Failure
        qDebug() << "Failure" << reply -> errorString();
    }
    return QVariant("");
}

/*QVariant parseAPI::getSongInfoJSON(const QVariant& code)
{
    QEventLoop eventLoop;
    QObject::connect(&network, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QNetworkRequest request(QUrl(QString("http://music.163.com/api/playlist/detail?id="+code.toString()+"&updateTime=-1")));
    request.setRawHeader("Referer", "http://music.163.com");
    request.setRawHeader("Cookie", "appver=1.5.0.75771");

    QNetworkReply* reply=network.get(request);
    eventLoop.exec();

}*/

void parseAPI::parseJSON(const QVariant& json)
{
    QList<qint64> forSongId;
    QList<qint64> forArtistId;
    QStringList forArtistName;
    QStringList forSongsName;
    QByteArray byteArray;
    QJsonParseError jsonParseError;
    QJsonDocument jsonDocument=QJsonDocument::fromJson(byteArray.append(json.toString()), &jsonParseError);

    //qDebug()<<"change to document";
    if(jsonParseError.error == QJsonParseError::NoError){
        qDebug()<<"successful";
        //qDebug()<<jsonDocument;
        if(jsonDocument.isObject()){
            qDebug()<<"success";
            QJsonObject jsonObject=jsonDocument.object();
            if(jsonObject.contains(QString("result"))){
                qDebug()<<"two";
                QJsonValue jsonValue=jsonObject.take(QString("result"));
                QJsonObject secondObject=jsonValue.toObject();
                if(secondObject.contains(QString("songs"))){
                    qDebug()<<"three";
                    QJsonValue secondValue=secondObject.take(QString("songs"));
                    if(secondValue.isArray()){
                        qDebug()<<"four";
                        QJsonArray jsonArray=secondValue.toArray();
                        int totalNumber=jsonArray.size();
                        for(int i=0; i<totalNumber; ++i){
                            QJsonValue thirdValue=jsonArray.at(i);
                            QJsonObject thirdObject=thirdValue.toObject();
                            if(thirdObject.contains(QString("id")) && thirdObject.contains(QString("artists")) && thirdObject.contains(QString("name")) ){
                                QJsonValue forthIDValue=thirdObject.take(QString("id"));

                                QJsonValue forthArtistValue=thirdObject.take(QString("artists"));

                                QJsonValue forthNameValue=thirdObject.take(QString("name"));

                                //get the code of list for others functions;
                                qint64 valueIDName=forthIDValue.toInt();
                                forSongsName.append(forthNameValue.toString());
                                //qDebug()<<QString("you");
                                forSongId.append(valueIDName);

                                if(forthArtistValue.isArray()){
                                    //qDebug()<<"yes";
                                    QJsonArray fiveJsonArray=forthArtistValue.toArray();
                                    for(int j= 0; j<fiveJsonArray.size(); ++j){//artists id;
                                        QJsonValue sixJsonValue=fiveJsonArray.at(j);
                                        if(sixJsonValue.isObject()){
                                            QJsonObject sevenObject=sixJsonValue.toObject();
                                            if(sevenObject.contains(QString("id")) && sevenObject.contains(QString("name"))){
                                                QJsonValue eightValue=sevenObject.take(QString("id"));
                                                QJsonValue eightArtistValue=sevenObject.take(QString("name"));

                                                QString artist=eightArtistValue.toString();
                                                qint64 artistID=eightValue.toInt();
                                                forArtistId.append(artistID);
                                                forArtistName.append(artist);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    songsName=forSongsName;
    songsID=forSongId;
    artistsId=forArtistId;
    artistsName=forArtistName;
    //qDebug()<<forArtistId.size();
}

QString parseAPI::parseSongsJSON(const QVariant& json)
{
    //this function for getting songs of list;
    //QStringList forSongsList;
    QString forSong;
    QByteArray byteArray;
    QJsonParseError jsonParseError;
    QJsonDocument jsonDocument=QJsonDocument::fromJson(byteArray.append(json.toString()), &jsonParseError);
    qDebug()<<"change to document";

    if(jsonParseError.error == QJsonParseError::NoError){
        qDebug()<<"success";
        if(jsonDocument.isObject()){
            qDebug()<<"first";
            QJsonObject firstJsonObject=jsonDocument.object();
            if(firstJsonObject.contains(QString("songs"))){
                qDebug()<<"second";
                QJsonValue jsonValue=firstJsonObject.take(QString("songs"));
                if(jsonValue.isArray()){
                    qDebug()<<"third";
                    QJsonArray jsonArray=jsonValue.toArray();
                    for(int i=0; i<jsonArray.size(); ++i){
                        QJsonValue thirdJsonValue=jsonArray.at(i);
                        if(thirdJsonValue.isObject()){
                            QJsonObject thirdJsonObject=thirdJsonValue.toObject();
                            if(thirdJsonObject.contains(QString("mp3Url"))){
                                QJsonValue forthJsonValue=thirdJsonObject.take(QString("mp3Url"));
                                QString mp3UrlStr=forthJsonValue.toString();
                                qDebug()<<mp3UrlStr;
                                forSong=mp3UrlStr;
                                //forSongsList.append(mp3UrlStr);
                            }else{
                                qDebug()<<"failed";
                            }
                        }
                    }
                }
            }
        }
    }
    songs=forSong;
    return songs;
}

QString parseAPI::getSongsImgUrl(const QVariant& json)
{
    QString imgUrl;
    QByteArray byteArray;
    QJsonParseError jsonParseError;
    QJsonDocument jsonDocument=QJsonDocument::fromJson(byteArray.append(json.toByteArray()), &jsonParseError);

    if(jsonParseError.error == QJsonParseError::NoError){
        qDebug()<<"successful";
        if(jsonDocument.isObject()){
            QJsonObject oneJsonObject=jsonDocument.object();
            if(oneJsonObject.contains(QString("artist"))){
                QJsonValue twoJsonValue=oneJsonObject.take(QString("artist"));
                if(twoJsonValue.isObject()){
                    QJsonObject threeJsonObject=twoJsonValue.toObject();
                    if(threeJsonObject.contains(QString("picUrl"))){
                        QJsonValue fourJsonValue=threeJsonObject.take(QString("picUrl"));
                        if(fourJsonValue.isString()){
                            QString url=fourJsonValue.toString();
                            imgUrl=url;
                        }
                    }
                }
            }
        }
    }
    songPicUrl=imgUrl;
    return songPicUrl;
}

void parseAPI::parseLyricJSON(const QVariant& json)
{
    QString forLyric;
    QByteArray byteArray;
    QJsonParseError jsonParseError;
    QJsonDocument jsonDocument=QJsonDocument::fromJson(byteArray.append(json.toString()), &jsonParseError);

    if(jsonParseError.error == QJsonParseError::NoError){
        if(jsonDocument.isObject()){
            QJsonObject firstObject=jsonDocument.object();
            if(firstObject.contains(QString("lrc"))){
                QJsonValue twoValue=firstObject.take(QString("lrc"));
                if(twoValue.isObject()){
                    QJsonObject threeObject=twoValue.toObject();
                    if(threeObject.contains(QString("lyric"))){
                        QJsonValue fourValue=threeObject.take(QString("lyric"));
                        QString lyricValue=fourValue.toString();
                        forLyric=lyricValue;
                    }
                }
            }
        }
    }
    theLyric=forLyric;
    qDebug()<<theLyric;
}

//download mp3;
void parseAPI::save(const QVariant& url, const QVariant& songName)
{
    name=songName.toString();
    QString downloadUrl=url.toString();
    downloader=new QNetworkAccessManager(this);
    QObject::connect(downloader,SIGNAL(finished(QNetworkReply*)), this, SLOT(write(QNetworkReply*)));
    downloader->get( QNetworkRequest(QUrl(downloadUrl)) );
}

void parseAPI::write(QNetworkReply* reply)
{
    if(!reply->isFinished()){
        qDebug()<<"error";
    }else{
        QDir dir;
        qDebug()<<dir.homePath()+name.toUtf8();
        QFile* file=new QFile(dir.homePath()+ QString("/Music/") + name + QString(".mp3"));
        file->open(QFile::Append);
        file->write(reply->readAll());
        if(file->flush()){
            qDebug()<<"success to save";
            file->close();
        }
        delete file;
        reply->deleteLater();
    }
}

qint64 parseAPI::getTotalSongsNumber()//how many id in songsID?
{
    /*for(const qint64& currentNumber: songsID){
        qDebug()<<currentNumber;
    }*/
    return songsID.size();
}

qint64 parseAPI::getCurrentId(const int& index)//get the id of the current song;
{
    return songsID.at(index);
}


QString parseAPI::getCurrentSongName(const int& index)
{
    return songsName.at(index);
}

QString parseAPI::getCurrentArtistName(const int& index)
{
    /*for(const QString& artist:artistsName){
        qDebug()<<artist;
    }*/
    return artistsName.at(index);
}

qint64 parseAPI::getCurrentArtistId(const int& index)
{
    return artistsId.at(index);
}

void parseAPI::regLyric()
{
    QRegExp rx("\\s*\\w*\\s*:*\\s*\\w*\n");
    QStringList sTrList=theLyric.split(rx);
    QString sTr;
    //int currentPositon=0;
    //int currentCount;
    QStringList forLyricList=theLyric.split(QString("\n"));
    qDebug()<<forLyricList;
}
