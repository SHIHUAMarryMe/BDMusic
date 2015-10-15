/*
  Copyright (C) 2013 Jolla Ltd.
  Contact: Thomas Perl <thomas.perl@jollamobile.com>
  All rights reserved.

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Jolla Ltd nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0
import ParseAPI 1.0
import MPlayer 1.0

Page {
    id: page;
    property var document;
    property var listForSinger;
    property int number;
    property var singer;
    property bool attached:false;
    property string songUrl:"";
    property var songJSON;
    property string songPic:"";
    property var albumByArtistJson;
    property int testNumber:28949444;
    property var testDocument;
    property string testString:"xihuanni";

    onStatusChanged:{
        if(page.status === PageStatus.Active && !attached){
            pageStack.pushAttached(Qt.resolvedUrl("PlayListPage.qml"));
            attached=true;
        }
    }

    ParseAPI{
        id:parseAPI;
    }
    MPlayer{
        id:mPlayer;
    }

    SilicaListView{
        id:listView;
        anchors{
            top:searchField.bottom;
            topMargin:parent.height*1/10;
            bottom:parent.bottom;
            bottomMargin:Theme.itemSizeHuge;
            fill:parent;
        }
        delegate:singersDele;
        model:number;
        quickScroll:true;
        VerticalScrollDecorator{ flickable:listView; }
        ViewPlaceholder{
            id:placeHoler;
            enabled:true;
            text:qsTr("empty");
            textFormat:Text.PlainText;
        }
    }
    Component{
        id:singersDele;
        Item{
            id:item;
            property Item downMenu;
            width:page.width;
            height:page.height*1/10;
            Text{
                id:singers;
                anchors.horizontalCenter:parent.horizontalCenter;
                anchors.bottom:parent.bottom;
                text:parseAPI.getCurrentSongName(index) + "-" + parseAPI.getCurrentArtistName(index);
                wrapMode:Text.WordWrap;
                color:"white";
                font.pixelSize:30;
                /*MouseArea{
                    id:mouseArea;
                    anchors.fill:parent;
                    onClicked:{
                        songJSON=parseAPI.song(parseAPI.getCurrentId(index));
                        console.log(songJSON);
                        songUrl=parseAPI.parseSongsJSON(songJSON);
                        console.log(songUrl);
                        mPlayer.playSingle(parseAPI.getCurrentSongName(index)+"-"+parseAPI.getCurrentArtistName(index),songUrl);//play the song;

                        albumByArtistJson=parseAPI.albumsByArtist(parseAPI.getCurrentArtistId(index));
                        console.log(albumByArtistJson);
                        songPic=parseAPI.getSongsImgUrl(albumByArtistJson);
                        console.log(songPic);
                        showP.source=songPic;



                        //get lyric;
                        testDocument=parseAPI.lyric(testNumber);
                        console.log(testDocument);
                        parseAPI.parseLyricJSON(testDocument);
                        parseAPI.regLyric();
                    }
                }*/
            }
            Component{
                id:contextComponent;
             ContextMenu{
                width:page.width;
                MenuItem{
                    text:qsTr("add to playlist");
                    onClicked:{
                        songJSON=parseAPI.song(parseAPI.getCurrentId(index));
                        songUrl=parseAPI.parseSongsJSON(songJSON);
                        mPlayer.savePlaylist(parseAPI.getCurrentSongName(index) + "-" + parseAPI.getCurrentArtistName(index), songUrl);//save the songName and song's url to local file ini;
                    }
                }
            }
           }
            Separator{
                width:page.width;
                color:"white";
                anchors{
                    bottom:parent.bottom;
                }
            }
            MouseArea{
                anchors.fill:parent;
                onPressAndHold:{
                   downMenu =contextComponent.createObject(listView);
                   downMenu.show(item);
                }
                onClicked:{
                    songJSON=parseAPI.song(parseAPI.getCurrentId(index));
                    console.log(songJSON);
                    songUrl=parseAPI.parseSongsJSON(songJSON);
                    console.log(songUrl);
                    mPlayer.playSingle(parseAPI.getCurrentSongName(index)+"-"+parseAPI.getCurrentArtistName(index),songUrl);//play the song;

                    albumByArtistJson=parseAPI.albumsByArtist(parseAPI.getCurrentArtistId(index));
                    console.log(albumByArtistJson);
                    songPic=parseAPI.getSongsImgUrl(albumByArtistJson);
                    console.log(songPic);
                    showP.source=songPic;



                    //get lyric;
                    /*testDocument=parseAPI.lyric(testNumber);
                    console.log(testDocument);
                    parseAPI.parseLyricJSON(testDocument);
                    parseAPI.regLyric();*/
                }
            }
        }
    }
    SearchField{
        id:searchField;
        visible:true;
        width:parent.width-parent.width*1/10;
        placeholderText:qsTr("Search your favorite singers");
        font.pixelSize:22;
        anchors{
            top:parent.top;
        }
        onTextChanged:{
            singer=searchField.text;
            placeHoler.enabled=false;
        }
    }
    Connections{
        target:searchField;
        onTextChanged:{
            console.log(singer,"test");
            document=parseAPI.search(singer);
            console.log(document);
            parseAPI.parseJSON(document);
            number=parseAPI.getTotalSongsNumber();
        }
    }
    SlideshowView{
        id:playList;
        width:page.width-showP.width;
        height:page.height*1/8;
        z:1;
        model:1;
        anchors{
            bottom:page.bottom;
            left:showP.right;
        }
        Text{
            id:showSongAndSinger;
            anchors{
                top:parent.top;
                horizontalCenter:parent.horizontalCenter;
                topMargin:Theme.paddingSmall;
            }
            color:"white";
            font.pixelSize:22;
        }
        delegate:MediaPlyOperator{}
    }
   /* MediaPlayer{
        id:mediaPly;
        autoPlay:true;
        //source:songUrl;
        onSourceChanged:{
            console.log("time", mediaPly.duration)
        }
    }*/
    ShowSongPic{
        id:showP;
        anchors{
            bottom:page.bottom;
            left:page.left;
        }
        onSourceChanged:{
            console.log(showP.source);
        }
    }
}



