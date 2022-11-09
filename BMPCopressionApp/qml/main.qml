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
    property string rootFilesFolder: path
    property bool useListViewMode: true
    width: Style.windowDefaultSize.width
    height: Style.windowDefaultSize.height
    color: Style.windowBackgroundColor
    property real headerTextPixelSize: Style.headerRelTextSize * Style.menuWidth * root.width
    property real standardTextPixelSize: Style.sectionRelTextSize * Style.menuWidth * root.width
    property real smallTextPixelSize: Style.menuRelTextSize * Style.menuWidth * root.width

    function openCMPpopup(file){
        dialog.fileselected = file
        dialog.open()
    }

    function getFileName(path){
        const myarr = path.split("/");
        return myarr[myarr.length - 1]
    }

    function processFile(file){
        var extension = (file.slice(file.lastIndexOf(".")+1))
        if(extension == "cmp")
            compressor.ReadCompressedFile(file)
        else
            compressor.CompressFile(file)
        dialog.close()
    }

   Row {
        FileBrowser {
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
        height: parent.height * Style.menuWidth * 2
        title: "Compress/Decompress file"
        standardButtons: Dialog.Apply | Dialog.Cancel
        property string fileselected: ""
        contentItem: Text {
                text: getFileName(dialog.fileselected)
            }

        background: Rectangle{
            color: Style.fileBrowserTitleBarColor
        }

        onApplied: processFile(fileselected)
    }
}
