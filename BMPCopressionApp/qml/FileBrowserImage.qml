import QtQuick
import "style.js" as Style

Item {
    id: wrapper
    property string file: ""

    Rectangle {
        id: placeholder
        anchors.fill: parent
        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: Style.placeholderTopColor
            }
            GradientStop {
                position: 1.0
                color: Style.placeholderBottomColor
            }
        }
        visible: true
    }

    Image {
        id: img
        anchors.fill: placeholder
        fillMode: Image.PreserveAspectFit
        cache: false
        source: wrapper.file
    }
}