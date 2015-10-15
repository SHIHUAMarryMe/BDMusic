import QtQuick 2.0
import Sailfish.Silica 1.0

Image{
    id:showPics;
    property string theSource;
    width:page.width*1/5;
    height:width;
    anchors{
        bottom:page.bottom;
        left:page.left;
    }
    onStatusChanged:{
        if(showPics.status == Image.Loading){
            unknow.visible=true;
        }else if(showPics.status == Image.Ready){
            unknow.visible=false;
        }
    }

    Rectangle{
        id:theRect;
        anchors{
            bottom:showPics.bottom;
            left:showPics.left;
        }
        width:showPics.width;
        height:showPics.height;
        color:"#00000000";
        Image{
            id:unknow;
            width:parent.width;
            height:parent.width;
            anchors{
                centerIn:parent;
            }
            source:"images/unknow.jpg";
        }
    }
}
