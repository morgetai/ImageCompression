import QtQuick
import QtQuick.Controls
import QtQml.Models

import MyPlugin 1.0
import "style.js" as Style

Rectangle {
    id: fileBrowser

    signal fileSelected(string file)

    width: Style.windowDefaultSize.width
    height: Style.windowDefaultSize.height

    color: Style.fileBrowserBackgroundColor

    // options
    property string rootfolder: ""
    property alias hideExtensions: folders.hideExtensions
    property bool allowOpeningFolder: false

    // infos
    property alias titleBarHeight: titleBar.height

    // internal
    property int itemSize: height * Style.fileBrowserItemSizeRel

    function selectFile(file) {
        if (file !== "")
            fileSelected(file)
    }

    function goDown(path) {
        folders.rootFolder = path
    }

    function goUp() {
        if (folders.rootFolder !== fileBrowser.rootfolder)
            folders.rootFolder = folders.getParentFolder()
    }

    function open(filePath, fileIsDir) {
        if (fileIsDir)
            goDown(filePath)
        else
            selectFile(filePath)
    }

    function humanReadableFileSize(bytes, si) {
        var thresh = si ? 1000 : 1024
        if (Math.abs(bytes) < thresh)
            return bytes + ' bytes'
        var units = si ? ['kB', 'MB', 'GB', 'TB', 'PB', 'EB', 'ZB', 'YB'] : ['KiB', 'MiB', 'GiB', 'TiB', 'PiB', 'EiB', 'ZiB', 'YiB']
        var u = -1
        do {
            bytes /= thresh
            ++u
        } while (Math.abs(bytes) >= thresh && u < units.length - 1)
        return bytes.toFixed(1) + ' ' + units[u]
    }

    FolderModel {
        id: folders
        rootFolder: fileBrowser.rootfolder
    }

    BrowserListView {
        id: listview
        visible: true

        anchors.top: titleBar.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        foldersModel: folders
        listItemSize: itemSize
        
        onOpenItem: (filePath, fileIsDir) => fileBrowser.open(filePath, fileIsDir)
    }

    Rectangle {
        id: titleBar
        width: parent.width
        height: parent.height * Style.titleBarRelHeight
        color: Style.fileBrowserTitleBarColor
        anchors.top: parent.top

        property real spacing: Style.fileBrowserTitleBarSpacing * height

        Button {
            id: upButton
            anchors.top: titleBar.top
            anchors.bottom: titleBar.bottom
            anchors.left: titleBar.left
            anchors.margins: titleBar.spacing
            onClicked: fileBrowser.goUp()
            icon.source: "images/left.png"
        }

        Text {
            id: titleText
            height: titleBar.height
            anchors.left: upButton.right
            anchors.margins: titleBar.spacing
            color: Style.fileBrowserTextColor
            font.pixelSize: standardTextPixelSize
            verticalAlignment: Text.AlignVCenter
            text: folders.rootFolder
            elide: Text.ElideMiddle
            Behavior on color {
                PropertyAnimation {
                }
            }
        }
    }
}