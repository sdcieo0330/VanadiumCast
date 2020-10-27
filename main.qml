import QtQuick 2.15
import QtQuick.Controls 2.15
import QtMultimedia 5.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2

ApplicationWindow {
    width: 512
    height: 420
    visible: true
    title: qsTr("VanadiumCast")

    SoundEffect {
        id: incomingPing
        source: Qt.resolvedUrl("/res/wav/incoming.wav")
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

    Popup {
        id: newConnPopup

        width: 256
        height: 128
        x: parent.width - width
        y: (parent.height - height) / 2.0
        visible: false
        modal: true
        focus: true
        property var callback: function(answer){}
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

        Connections {
            target: sinkHandler
            function onIncomingConnectionRequest() {
                console.log("QML: onIncomingConnectionRequest() triggered")
                newConnPopup.setCallback(function(answer){
                    sinkHandler.incomingConnectionRequestAnswer(answer)
                })
                newConnPopup.open()
            }
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
                    newConnPopup.callback(true)
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
                    newConnPopup.callback(false)
                }
            }
        }
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

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
                    pageStreaming.enabled = true
                    pageStreamingBtn.enabled = true
                    console.debug(fileName) // second data entry is the address label
                    inputFileName = fileName
                }
            }
            Connections {
                target: pageMedia.streamButton
                function onClicked() {
                    pageStreaming.enabled = true
                    pageStreamingBtn.enabled = true
                    backendAPI.startSource(pageMedia.inputFileName, pageDevices.deviceAddress)
                }
            }
        }

        PageStreaming {
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
