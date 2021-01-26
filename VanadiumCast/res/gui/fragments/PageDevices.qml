import QtQuick 2.10
import QtQuick.Controls 2.10
import QtQuick.Controls.Universal 2.10
import QtQuick.Controls.Material 2.10
import "qrc:/gui/model"

Page {
    width: 772
    height: 450

    property alias deviceLV: devicesListView

    header: Label {
        id: devicesPageHeader
        text: qsTr("1. Select device you want to stream to")
        font.pixelSize: Qt.application.font.pixelSize * 1.2
        padding: 10
    }

    Component {
        id: deviceDelegate
//        property alias name: deviceDelegateItem.name
//        property alias address: deviceDelegateItem.address
        Item {
//            property string name: "" + name
//            property string address: "" + address
            anchors.horizontalCenter: parent.horizontalCenter
            id: deviceDelegateItem
            width: 240; height: 32
            Label {
                anchors.left: parent.left
                anchors.leftMargin: (parent.width - nameLabel.width - addressLabel.width) / 2
                anchors.verticalCenter: parent.verticalCenter
                id: nameLabel
                text: name + ":"
                color: Material.foreground
            }
            Label {
                id: addressLabel
                anchors.left: nameLabel.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: (parent.width - nameLabel.width - addressLabel.width) / 2
                text: address
                color: Material.foreground
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
        anchors.fill: parent
        anchors.margins: 8
        border.width: 1
        radius: 2
        border.color: Material.accent
        color: Qt.rgba(0, 0, 0, 0)
        ListView {
            id: devicesListView
            currentIndex: -1
            anchors.fill: parent
            anchors.margins: 8
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
                border.color: Material.accent
                color: Qt.darker(Material.accent, 1.8)
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
}
