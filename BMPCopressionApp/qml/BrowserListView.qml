import QtQuick
import QtQuick.Controls
import QtQml.Models

import MyPlugin 1.0
import "style.js" as Style

Item {
    id: listviewTable

    signal openItem(string filePath, bool fileIsDir)
    signal folderAction(int filesInDir, string filePath)

    property variant foldersModel
    property int listItemSize

    property real textColumnSize: smallTextPixelSize * 10

    ListView {
        id: listview
        width: Style.windowDefaultSize.width

        anchors.top: sortBar.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        //anchors.rightMargin: scrollBar.visible ? 0.5 * scrollBar.width
        //                                         + anchors.leftMargin : anchors.leftMargin

        anchors.rightMargin: anchors.leftMargin
        anchors.leftMargin: listItemSize * Style.mainPanelRelMargin
        spacing: listItemSize * Style.fileBrowserListItemSpacing
        clip: true

        model: DelegateModel {
            id: listDelegateModel
            model: foldersModel
            rootIndex: foldersModel.getRootIndex()

            Connections {
                target: foldersModel
                onRootFolderChanged: rootIndex = foldersModel.getRootIndex()
            }

            onRootIndexChanged: {
                rootIndex = foldersModel.getRootIndex()
            }

            delegate: FileListItem {
                width: listview.width
                height: listItemSize * Style.fileBrowserListItemRelSize
                onClicked: listviewTable.openItem(filePath, fileIsDir)
                onPressAndHold: {
                    if (!fileIsDir) {
                        listviewTable.folderAction(filesInDir, filePath)
                    }
                }
            }
        }

        section.criteria: ViewSection.FirstCharacter
        section.delegate: Item {
            width: parent.width
            height: 1.5 * smallTextPixelSize
            Text {
                text: section
                font.capitalization: Font.Capitalize
                font.pixelSize: smallTextPixelSize
            }
        }
    }

    Rectangle {
        id: sortBar
        height: titleBar.height / 2
        anchors.left: listview.left
        anchors.right: listview.right
        color: Style.fileBrowserBackgroundColor

        Text {
            id: nameText
            text: "name"
            horizontalAlignment: Text.AlignHCenter
            anchors.left: sortBar.left
            anchors.right: sizeText.left

            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: smallTextPixelSize
            color: Style.fileBrowserTextColor
        }
        Text {
            id: sizeText
            text: "size"
            anchors.right: sortBar.right
            width: textColumnSize
            horizontalAlignment: Text.AlignRight

            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: smallTextPixelSize
            color: Style.fileBrowserTextColor
        }
    }
}