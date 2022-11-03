import QtQuick 
import QtQuick.Controls
import QtQml.Models

import MyPlugin 1.0
import "style.js" as Style

Rectangle {
    id: listItem
    color: "transparent"

    signal clicked
    signal pressAndHold

    FileBrowserImage {
        id: fileimage
        height: parent.height
        width: height
        file: fileIsDir ? "images/folder.png" : "images/file.png"
    }

    Text {
        id: nameText
        text: fileName
        anchors.left: fileimage.right
        anchors.leftMargin: font.pixelSize
        anchors.right: sizeText.left
        elide: Text.ElideMiddle

        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: smallTextPixelSize
        color: Style.fileBrowserTextColor
    }

    Text {
        id: sizeText
        text: humanReadableFileSize(fileSize, true)
        anchors.right: listItem.right
        visible: !fileIsDir

        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: smallTextPixelSize
        color: Style.fileBrowserTextColor
    }

    MouseArea {
        id: mouseRegion
        anchors.fill: parent
        onPressed: listItem.ListView.view.currentIndex = index
        onClicked: clickAnimation.start()
        onPressAndHold: listItem.pressAndHold()
    }

    ColorAnimation on color {
        id: clickAnimation
        running: false
        from: Style.fileBrowserBlinkColor
        to: "transparent"
        onStopped: listItem.clicked()
    }
}