import QtQuick 2.15
import QtQuick.Controls 2.15
import QtMultimedia 5.12
import QtQuick.Controls.Styles 1.4

ApplicationWindow {
    width: 512
    height: 420
    visible: true
    title: qsTr("VanadiumCast")

    SoundEffect {
        id: incomingPing
        source: Qt.resolvedUrl("/res/wav/incoming.wav")
    }

    Popup {
        width: 256
        height: 128
        x: parent.width - width
        y: (parent.height - height) / 2.0
        id: newConnPopup
        visible: false
        modal: true
        focus: true
        property var callback: function(){}
        closePolicy: Popup.NoAutoClose

        function setCallback(cb) {
            callback = cb
        }

        enter: Transition {
            PropertyAnimation {
                property: "x"
                from: newConnPopup.parent.width
                to: newConnPopup.parent.width - newConnPopup.width
                duration: 512
                easing.type: Easing.OutElastic
            }
        }
        exit: Transition {
            PropertyAnimation {
                property: "x"
                from: newConnPopup.parent.width - newConnPopup.width
                to: newConnPopup.parent.width
                duration: 512
                easing.type: Easing.InBack
            }
        }

        onClosed: {
            callback()
        }

        contentItem: Item {
            id: newConnPopupContent
            Item {
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: newConnPopupAccept.top
                Text {
                    id: newConnPopupMessage
                    anchors.centerIn: parent
                    font.pixelSize: Qt.application.font.pixelSize * 1.2
                    text: qsTr("Incoming Connection Request")
                }
            }
            Button {
                id: newConnPopupAccept
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.leftMargin: 20
                anchors.bottomMargin: 12
                text: qsTr("Accept")
                onClicked: {
                    newConnPopup.close()
                    newConnPopup.setCallback(function () {
                        pageDevices.getListModel().append({"name": "Random Device", "address": "0.0.0.0"})
                        newConnPopup.setCallback(function(){})
                    })
                }
            }
            Button {
                id: newConnPopupDecline
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.rightMargin: 20
                anchors.bottomMargin: 12
                text: qsTr("Decline")
                onClicked: {
                    newConnPopup.close()
                }
            }
        }
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        PageDevices {
            id: pageDevices
            Connections {
                target: pageDevices.deviceLV
                function onCurrentItemChanged() {
                    pageMedia.enabled = true
                    pageMediaBtn.enabled = true
                }
            }
        }

        PageMedia {
            id: pageMedia
            enabled: false
            Connections {
                target: pageMedia
                function onSelectedMedia(fileName) {
                    pageStreaming.enabled = true
                    pageStreamingBtn.enabled = true
                }
            }
        }

        PageStreaming {
            id: pageStreaming
            enabled: false
        }

        PageSink {
            id: pageSink
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
        TabButton {
            text: qsTr("Sink")
            enabled: false
        }
    }
}
