import QtQml.Models
import QtQuick
import QtQuick.Window
import QtQuick.Controls
import MyPlugin 1.0
import "style.js" as Style

Window {
    id: root
    visible: true
    title: qsTr("BMP compressor")
    property string rootFilesFolder: "/Users/andriihridin/Documents/Programming/TestPockerBook/build"
    property bool useListViewMode: true // retain presentation mode, shared for all panels
    width: Style.windowDefaultSize.width
    height: Style.windowDefaultSize.height
    color: Style.windowBackgroundColor
    property real headerTextPixelSize: Style.headerRelTextSize * Style.menuWidth * root.width
    property real standardTextPixelSize: Style.sectionRelTextSize * Style.menuWidth * root.width
    property real smallTextPixelSize: Style.menuRelTextSize * Style.menuWidth * root.width

    function openCMPpopup(file){
        dialog.fileselected = file
        //cmpRec.visible = true
        //compresspopup.open()
        dialog.open()
    }

    function processFile(file){
        compressor.ReadCompressedFile(file)
        //cmpRec.visible = true
        //compresspopup.open()
    }

   Row {
        FileBrowser {
            //id: fileBrowser
            rootfolder: rootFilesFolder
            allowOpeningFolder: true
            onFileSelected: (file) => root.openCMPpopup(file)
        }
    }

    BMPCompress {
        id: compressor
    }

    Dialog {
        id: dialog
        anchors.centerIn: parent
        width: parent.width * Style.menuWidth * 2
        height: parent.height * Style.menuWidth * 3
        title: "Compress/Decompress file"
        standardButtons: Dialog.Apply | Dialog.Cancel
        property string fileselected: ""
        contentItem: Text {
                text: dialog.fileselected
            }

        background: Rectangle{
            color: Style.fileBrowserTitleBarColor
        }

        onApplied: processFile(fileselected)
    }
}
