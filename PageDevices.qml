import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

Page {
    width: 600
    height: 400

    header: Label {
        id: devicesPageHeader
        text: qsTr("Devices")
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }

    Component {
        id: deviceDelegate
        Item {
            width: 240; height: 32
            Row {
                anchors.centerIn: parent
                Text {text: name}
                Text {text: "(" + address + ")"}
            }
            MouseArea {
                anchors.fill: parent
                onClicked: devicesListView.currentIndex = index
            }
        }
    }
    function getListModel() {
        return devicesListModel
    }

    Rectangle {
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
            width: parent.width - 20
            height: parent.height - triggerIncomingConn.height - 20
            anchors.centerIn: parent
            model: DeviceListModel {id: devicesListModel}
            delegate: deviceDelegate
            highlight: Rectangle {
                radius: 2
                border.color: Material.accentColor
                color: Qt.lighter(Material.accentColor, 1.8)
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
                NumberAnimation { properties: "x,y"; duration: 128 }
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
