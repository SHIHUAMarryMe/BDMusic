import QtQuick 2.0
import Sailfish.Silica 1.0


Rectangle{
    id:rect;
    property string forSongAndSinger;
    width:page.width-showP.width;
    height:page.height*1/8;
    color:"#00000000";

    IconButton{
        id:pauseBtn;
        width:parent.width*1/5;
        height:parent.height*1/2;
        icon.source: "image://theme/icon-m-pause?"+(pressed ? Theme.highlightColor : Theme.primaryColor);
        anchors{
            bottom:parent.bottom;
            left:parent.left;
            margins:Theme.paddingSmall;
        }
        onClicked:{
            mediaPly.pause();
        }
    }
    IconButton{
        id:palyBtn;
        anchors{
            left:pauseBtn.right;
            bottom:parent.bottom;
            margins:Theme.paddingSmall;
        }
        width:parent.width*1/5;
        height:parent.height*1/2;
        icon.source:"image://theme/icon-m-play?"+(pressed ? Theme.highlightColor : Theme.primaryColor)
        onClicked:{
            mediaPly.play();
        }
    }
}
