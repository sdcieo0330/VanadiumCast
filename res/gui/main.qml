import QtQuick 2.15
import QtQuick.Controls 2.15
import QtMultimedia 5.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2

ApplicationWindow {
    width: 772
    height: 492
    visible: true
    title: qsTr("VanadiumCast")

    onClosing: {
        Qt.callLater(Qt.quit)
    }

    SoundEffect {
        id: incomingPing
        source: Qt.resolvedUrl("qrc:/sound/incoming.wav")
    }

    MessageDialog {
        id: alertDialog
        visible: false
        title: "Error"
        icon: StandardIcon.Critical
        standardButtons: "Ok"
        onAccepted: {
            alertDialog.close()
        }
    }

    Dialog {
        id: newConnPopup

        width: 256
        height: 128
        visible: false
        title: qsTr("Connection request")
        property var callback: function(answer){}

        function setCallback(cb) {
            callback = cb
        }

        standardButtons: Dialog.Ok | Dialog.Cancel

        Text {
            id: newConnPopupMessage
            anchors.centerIn: parent
            font.pixelSize: Qt.application.font.pixelSize * 1.2
            text: qsTr("Incoming Connection Request")
        }

        onAccepted: {
            callback(true)
        }

        onRejected: {
            callback(false)
        }

        Connections {
            target: sinkHandler
            function onIncomingConnectionRequest() {
                console.log("QML: onIncomingConnectionRequest() triggered")
                newConnPopup.setCallback(function(answer){
                    sinkHandler.incomingConnectionRequestAnswer(answer)
                })
                incomingPing.play()
                newConnPopup.visible = true
            }
        }

//        enter: Transition {
//            PropertyAnimation {
//                property: "x"
//                from: newConnPopup.parent.width
//                to: newConnPopup.parent.width - newConnPopup.width
//                duration: 512
//                easing.type: Easing.OutElastic
//            }
//        }
//        exit: Transition {
//            PropertyAnimation {
//                property: "x"
//                from: newConnPopup.parent.width - newConnPopup.width
//                to: newConnPopup.parent.width
//                duration: 512
//                easing.type: Easing.InBack
//            }
//        }

//        contentItem: Item {
//            id: newConnPopupContent
//            Item {
//                anchors.right: parent.right
//                anchors.left: parent.left
//                anchors.top: parent.top
//                anchors.bottom: newConnPopupAccept.top
//                Text {
//                    id: newConnPopupMessage
//                    anchors.centerIn: parent
//                    font.pixelSize: Qt.application.font.pixelSize * 1.2
//                    text: qsTr("Incoming Connection Request")
//                }
//            }
//            Button {
//                id: newConnPopupAccept
//                anchors.left: parent.left
//                anchors.bottom: parent.bottom
//                anchors.leftMargin: 20
//                anchors.bottomMargin: 12
//                text: qsTr("Accept")
//                onClicked: {
//                    newConnPopup.close()
//                    newConnPopup.callback(true)
//                }
//            }
//            Button {
//                id: newConnPopupDecline
//                anchors.right: parent.right
//                anchors.bottom: parent.bottom
//                anchors.rightMargin: 20
//                anchors.bottomMargin: 12
//                text: qsTr("Decline")
//                onClicked: {
//                    newConnPopup.close()
//                    newConnPopup.callback(false)
//                }
//            }
//        }
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        Connections {
            target: backendAPI

            function onStreamStarted() {
                pageStreaming.enabled = true
                pageStreamingBtn.enabled = true
                pageStreaming.togglePlayPauseBtn.icon.name = "media-playback-pause"
                pageStreaming.togglePlayPauseBtn.icon.source = "qrc:/gui/icons/pause.svg"
                swipeView.setCurrentIndex(2)
            }

            function onStreamEnded() {
                swipeView.setCurrentIndex(1)
                pageStreaming.enabled = false
                pageStreamingBtn.enabled = false
                pageStreaming.positionChangeSlider.value = 0
                pageStreaming.positionChangeSlider.to = 100
            }
        }

        PageDevices {
            property string deviceAddress;
            id: pageDevices
            Connections {
                target: pageDevices.deviceLV
                function onCurrentItemChanged() {
                    pageMedia.enabled = true
                    pageMediaBtn.enabled = true
                    console.debug(pageDevices.deviceLV.currentItem.data[1].text) // second data entry is the address label
                    if (!backendAPI.setDevice(pageDevices.deviceLV.currentItem.data[1].text)) {
                        alertDialog.setText("Cannot select device!")
                        alertDialog.setVisible(true)
                    }
                    pageDevices.deviceAddress = pageDevices.deviceLV.currentItem.data[1].text
                }
            }
        }

        PageMedia {
            property url inputFileName;
            id: pageMedia
            enabled: false
            Connections {
                target: pageMedia
                function onSelectedMedia(fileName) {
                    console.debug(fileName) // second data entry is the address label
                    pageMedia.inputFileName = fileName
                }
            }
            Connections {
                target: pageMedia.streamButton
                function onClicked() {
                    console.debug(pageMedia.inputFileName)
                    backendAPI.startSource(pageMedia.inputFileName, pageDevices.deviceAddress)
                }
            }
        }

        PageStreaming {
            Timer {
                id: getDurationTimer
                interval: 100
                running: false
                repeat: false
                onTriggered: {
                    pageStreaming.positionChangeSlider.to = backendAPI.getDuration()
                }
            }
            id: pageStreaming
            enabled: false
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        TabButton {
            id: pageDevicesBtn
            text: qsTr("Devices")
        }
        TabButton {
            id: pageMediaBtn
            text: qsTr("Media")
            enabled: false
        }
        TabButton {
            id: pageStreamingBtn
            text: qsTr("Streaming")
            enabled: false
        }
    }
}
