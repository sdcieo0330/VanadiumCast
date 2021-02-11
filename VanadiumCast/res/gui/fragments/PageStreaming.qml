import QtQuick 2.10
import QtQuick.Controls 2.10

Page {
    width: 772
    height: 460

    property alias togglePlayPauseBtn: playPauseBtn
    property alias positionChangeSlider: positionSlider

    function msToTime(duration) {
        var seconds = Math.floor((duration / 1000) % 60);
        var minutes = Math.floor((duration / (1000 * 60)) % 60);
        var hours = Math.floor((duration / (1000 * 60 * 60)) % 24);

        hours = (hours < 10) ? "0" + hours : hours;
        minutes = (minutes < 10) ? "0" + minutes : minutes;
        seconds = (seconds < 10) ? "0" + seconds : seconds;

        return hours + ":" + minutes + ":" + seconds;
    }

    header: Label {
        font.pixelSize: Qt.application.font.pixelSize * 2
        text: qsTr("Streaming Control Panel")
        padding: 10
    }

    Button {
//        Timer {
//            id: playPauseBtnBreak
//            interval: 3200
//            repeat: false
//            onTriggered: {
//                playPauseBtn.enabled = true
//            }
//        }

//        Connections {
//            target: playPauseBtn
//            function onClicked() {
//                playPauseBtn.enabled = false
//                playPauseBtnBreak.start()
//            }
//        }

        id: playPauseBtn
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 20
        height: 48
        width: 48
        icon.name: "media-playback-pause"
        icon.source: "qrc:/gui/icons/pause.svg"
        onClicked: {
            enabled = false
            backendAPI.togglePlayPause()
        }
        Connections {
            target: backendAPI
            function onPlaybackToggled(paused) {
                if (paused) {
                    playPauseBtn.icon.source = "qrc:/gui/icons/play.svg"
                    playPauseBtn.icon.name = "media-playback-start"
                } else {
                    playPauseBtn.icon.source = "qrc:/gui/icons/pause.svg"
                    playPauseBtn.icon.name = "media-playback-pause"
                }
                playPauseBtn.enabled = true;
            }
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

        onMoved: {
            currentPlayPos.text = pageStreaming.msToTime(positionSlider.value)
//            positionSliderPopup.label.text = msToTime(value)
        }

        from: 0
        value: 0
        to: 100
        enabled: true

        Behavior on value {
            NumberAnimation {
                duration: 640
                easing.type: Easing.Linear
            }
        }

        Connections {


            target: backendAPI
            function onPlaybackPositionChanged(position) {
                if (!positionSlider.pressed) {
                    positionSlider.value = position
                    console.log("[PageStreaming] position set:" + position + "/" + positionSlider.to)
                    currentPlayPos.text = pageStreaming.msToTime(position)
                }
            }

            function onDurationLoaded(dur) {
                positionSlider.to = dur
                console.log("[PageStreaming] duration set")
                duration.text = "/ " +  pageStreaming.msToTime(dur)
            }
        }
    }
    Label {
        id: positionLabel
        text: positionSlider.value
    }
}
