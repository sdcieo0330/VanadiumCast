import QtQuick 2.0
import QtQuick.Controls 2.12

Page {
    header: Label {
        font.pixelSize: Qt.application.font.pixelSize * 2
        text: qsTr("Sink Display")
        padding: 10
    }

    Label {
        anchors.centerIn: parent
        text: qsTr("Todo")
    }
}
