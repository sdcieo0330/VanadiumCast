import QtQuick 2.15
import QtQuick.Controls 2.15

Page {
    width: 600
    height: 400

    header: Label {
        text: qsTr("Media")
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }

    Label {
        text: qsTr("Todo")
        anchors.centerIn: parent
    }
}
