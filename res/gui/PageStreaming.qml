import QtQuick 2.0
import QtQuick.Controls 2.12

Page {
    width: 772
    height: 460

    header: Label {
        font.pixelSize: Qt.application.font.pixelSize * 2
        text: qsTr("Streaming Control Panel")
        padding: 10
    }

    Button {
        id: playPauseBtn
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 20
        height: 48
        width: 48
        icon.name: "media-playback-start"
        onClicked: {
            icon.name = "media-playback-pause"
            backendAPI.togglePlayPause()
        }
    }

    Label {
        anchors.left: playPauseBtn.right
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 0
        id: currentPlayPos
        text: "00:00:00"
    }
    Label {
        anchors.left: currentPlayPos.right
        anchors.leftMargin: 0
        anchors.verticalCenter: parent.verticalCenter
        id: duration
        text: " / 00:00:00"
    }

    Slider {
        anchors.left: duration.right
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 20
        id: positionSlider
        onPressedChanged: {
            if (!pressed) {
                backendAPI.seek(value)
            }
        }
    }
}
