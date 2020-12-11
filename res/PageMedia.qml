import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.2
import QtAV 1.6
import QtQuick.Controls.Material 2.12

Page {
    property alias streamButton: startStreamBtn
    Timer {
        id: timer
    }

    id: pageMediaClass
    width: 772
    height: 460

    signal selectedMedia(url fileName)

    header: Label {
        text: qsTr("2. Select media file you want to stream")
        font.pixelSize: Qt.application.font.pixelSize * 1.2
        padding: 8
    }

    FileDialog {
        id: mediaSelectionDialog
        title: "Please choose a media file"
        folder: "file:///home/silas"
        selectMultiple: false
        selectExisting: true
        selectFolder: false
        onAccepted: selectedMedia(mediaSelectionDialog.fileUrl)
        nameFilters: ["Video files (*.mp4 *.mkv *.avi *.mov)", "Audio files (*.mp3 *.m4a *.flac *.mp2 *.wav)", "All files (*.*)"]
        visible: false
    }

    TextField {
        anchors.top: parent.top
        anchors.topMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.right: selectMediaBtn.left
        anchors.rightMargin: 8

        id: mediaPathInput
        placeholderText: "Select media file"
        onEditingFinished: {
            selectedMedia(text)
        }
    }

    Connections {
        target: pageMediaClass
        function onSelectedMedia(fileName) {
            mediaPathInput.text = fileName
            previewVideo.stop()
            previewVideo.source = fileName
            previewVideo.play()
            startStreamBtn.enabled = true
        }
    }

    Button {
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.verticalCenter: mediaPathInput.verticalCenter
        height: mediaPathInput.heigth
        id: selectMediaBtn
        text: "Browse"
        onClicked: {
            mediaSelectionDialog.open()
        }
    }
    Item {
        id: previewVideoContainer
        Timer {
            id: controlsAutoHideTimer
            interval: 1520
            repeat: false
            onTriggered: {
                previewVideoControls.hide()
            }
        }

        Timer {
            id: controlsMouseOutHideTimer
            interval: 500
            repeat: false
            onTriggered: {
                previewVideoControls.hide()
            }
        }

        anchors.top: selectMediaBtn.bottom
        anchors.bottom: streamButton.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.margins: 8
        Label {
            id: previewLabel
            text: qsTr("Preview")
            anchors.top: parent.top
            anchors.left: parent.left
        }

        Video {
            id: previewVideo
            source: ""
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: previewLabel.bottom
            anchors.bottom: parent.bottom
            anchors.topMargin: 8
            height: 1280
            width: 720
            videoCodecPriority: ["QSV", "CUDA", "FFMPEG"]
            audioBackends: ["null"]
            Component.onCompleted: console.log(previewVideo.audioBackends)
            smooth: true
            onPositionChanged: {
                previewVideoSlider.isChangedFromVideo = true
                previewVideoSlider.value = position
            }

            onSeekFinished: {
                play()
            }

            onPlaybackStateChanged: {
                if (playbackState == MediaPlayer.PlayingState) {
                    playPause.icon.name = "media-playback-pause"
                } else if (playbackState == MediaPlayer.PausedState) {
                    playPause.icon.name = "media-playback-start"
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (previewVideo.playbackState == MediaPlayer.PlayingState){
                        previewVideo.pause()}
                    else{
                        previewVideo.play()
                    }
                }
            }
        }
        Rectangle {
            z: 24
            id: previewVideoControls
            height: 48
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.margins: 0
            color: Material.backgroundColor
            Button {
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                width: 48
                anchors.margins: 0
                id: playPause
                enabled: previewVideo.hasVideo
                icon.name: "media-playback-start"
                onClicked: {
                    if (icon.name == "media-playback-pause") {
                        previewVideo.pause()
                        icon.name = "media-playback-start"
                    } else {
                        previewVideo.play()
                        icon.name = "media-playback-pause"
                    }
                }
            }
            Slider {
                id: previewVideoSlider
                property bool isChangedFromVideo: false
                anchors.right: parent.right
                anchors.left: playPause.right
                anchors.leftMargin: 16
                anchors.rightMargin: 8
                anchors.verticalCenter: parent.verticalCenter
                from: 0
                to: previewVideo.duration
                value: 0
                onPressedChanged: {
                    if (!pressed) {
                        previewVideo.pause()
                        previewVideo.seek(value)
                    }
                }
            }

            visible: false
            opacity: 0.0
            scale: 0.75
            Behavior on opacity {
                PropertyAnimation {
                    duration: 256
                    onFinished: {
                        if (previewVideoControls.opacity == 0.0) {
                            previewVideoControls.visible = false
                        }
                    }
                }
            }
            Behavior on scale {
                PropertyAnimation {
                    duration: 256
                }
            }
            function show() {
                visible = true
                opacity = 1.0
                scale = 1.0
            }

            function hide() {
                previewVideoControls.opacity = 0.0
                previewVideoControls.scale = 0.75
            }
        }

        HoverHandler {
            onPointChanged: {
                if (!previewVideoControls.visible) {
                    previewVideoControls.show()
                    controlsAutoHideTimer.restart()
                } else if (previewVideoControls.opacity != 1.0){
                    previewVideoControls.opacity = 1.0
                    previewVideoControls.visible = true
                    controlsAutoHideTimer.restart()
                } else {
                    controlsAutoHideTimer.restart()
                }
            }
        }

        MouseArea {
            anchors.fill: parent
            //onDoubleClicked: //Fullscreen
            focus: true
            hoverEnabled: true
            onPositionChanged: {
                if (!previewVideoControls.visible) {
                    previewVideoControls.show()
                    controlsAutoHideTimer.restart()
                } else if (previewVideoControls.opacity != 1.0){
                    previewVideoControls.show()
                    controlsAutoHideTimer.restart()
                } else {
                    controlsAutoHideTimer.restart()
                }
            }
            onEntered: {
                controlsAutoHideTimer.restart()
                previewVideoControls.show()
            }
            onExited: {
                controlsMouseOutHideTimer.start()
                controlsAutoHideTimer.stop()
            }
        }
    }
    Button {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 8
        id: startStreamBtn
        text: qsTr("Stream")
        enabled: false
        onClicked: previewVideo.stop()
    }
}
