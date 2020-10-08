import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import "qrc:/gui/model"

Page {
    width: 600
    height: 400

    property alias deviceLV: devicesListView

    header: Label {
        id: devicesPageHeader
        text: qsTr("1. Select device you want to stream to")
        font.pixelSize: Qt.application.font.pixelSize * 1.2
        padding: 10
    }

    Component {
        id: deviceDelegate
        Item {
            anchors.horizontalCenter: parent.horizontalCenter
            id: deviceDelegateItem
            width: 240; height: 32
            Row {
                anchors.centerIn: parent
                Label {
                    text: name
                    color: Material.foreground
                }
                Label {
                    text: "(" + address + ")"
                    color: Material.foreground
                }
            }
            MouseArea {
                id: delegateMouseArea
                anchors.fill: parent
                onClicked: {
                    devicesListView.currentIndex = index
                }
            }
        }
    }
    function getListModel() {
        return devicesListModel
    }

    Rectangle {
        id: deviceViewRect
        anchors.top: parent.top
        anchors.bottom: triggerIncomingConn.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.margins: 8
        border.width: 1
        radius: 2
        border.color: Material.accentColor
        color: Qt.rgba(0, 0, 0, 0)
        ListView {
            id: devicesListView
            currentIndex: -1
            width: parent.width - 20
            height: parent.height - triggerIncomingConn.height - 20
            anchors.centerIn: parent
            model: DeviceListModel {
                id: devicesListModel
            }

            Connections {
                id: addConn
                target: deviceDirectory
                function onAddedDevice (device) {
                    devicesListModel.append({"name": device.getName(), "address": device.getAddressString()})
                }
                function onRemovedDevice (device) {
                    for (var i = 0; i < devicesListModel.count; ++i) {
                        if (devicesListModel.get(i)["address"] === device.getAddressString()) {
                            devicesListModel.remove(i);
                            break;
                        }
                    }
                    console.log("exited removedDevice()");
                }
            }
            delegate: deviceDelegate
            highlight: Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter
                radius: 2
                border.color: Material.accentColor
                color: Qt.darker(Material.accentColor, 1.8)
                y: devicesListView.currentItem.y
                Behavior on y {
                    NumberAnimation {
                        easing.type: Easing.OutBack
                        easing.overshoot: 1.1
                    }
                }
            }
            highlightFollowsCurrentItem: true

            flickableDirection: Flickable.AutoFlickDirection
            add: Transition {
                PropertyAnimation {
                    property: "scale"
                    from: 0.0
                    to: 1.0
                    duration: 160
                    easing.type: Easing.OutBack
                    easing.overshoot: 1.2
                }
            }
            addDisplaced: Transition {
                NumberAnimation {
                    properties: "x,y"
                    duration: 128
                }
            }
            remove: Transition {
                PropertyAnimation {
                    property: "scale"
                    from: 1.0
                    to: 0.0
                    duration: 160
                    easing.type: Easing.InBack
                    easing.overshoot: 1.2
                }
            }
            removeDisplaced: Transition {
                NumberAnimation {
                    properties: "x,y"
                    duration: 128
                }
            }
            focus: true
        }
    }

    Button {
        id: triggerIncomingConn
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        text: qsTr("Trigger incoming connection popup")
        onClicked: {
            incomingPing.play()
            newConnPopup.open()
        }
    }
}
