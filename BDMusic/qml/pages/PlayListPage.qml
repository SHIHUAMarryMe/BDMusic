import QtQuick 2.0
import Sailfish.Silica 1.0
import MPlayer 1.0

Page{
    id:forPlaylist;

    property bool attached:false;
    property int songsNumber;
    onStatusChanged:{
        if(forPlaylist.status === PageStatus.Active && !attached){
            pageStack.pushAttached(Qt.resolvedUrl("FirstPage.qml"));

            songsNumber=player.readSettings();//get the number of the list of songs;
            console.log(songsNumber);
            attached=true;
        }
    }
    MPlayer{
        id:player;
    }
    SilicaListView{
        id:lView;
        anchors{
            top:parent.top;
            topMargin:parent.height*1/10;
            fill:parent;
        }
        model:songsNumber;
        ViewPlaceholder{
            text:qsTr("empty");
        }
        delegate:listDelegate;
    }
    Component{
        id:listDelegate;
        Item{
            width:ListView.view.width;
            height:Theme.itemSizeSmall;
            Text{
                text:player.getTheSong(index);
                color:"white";
                font.pixelSize:40;
                anchors.centerIn:parent;
            }
            MouseArea{
                anchors.fill:parent;
                onClicked:{
                    player.playSingle( player.getTheSong(index), player.getTheSongUrl(index) );
                }
            }
        }
    }
}
