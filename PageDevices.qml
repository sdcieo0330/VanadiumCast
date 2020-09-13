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
        }
    }

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
        }
        highlightFollowsCurrentItem: true
        highlightMoveDuration: 1000
        highlightMoveVelocity: -1
        flickableDirection: Flickable.AutoFlickDirection
        add: Transition {
            NumberAnimation { properties: "x,y"; from: 100; duration: 128 }
        }
        addDisplaced: Transition {
            NumberAnimation { properties: "x,y"; duration: 128 }
        }
        focus: true
    }

    Button {
        id: triggerIncomingConn
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        text: qsTr("Trigger incoming connection popup")
        onClicked: {
            incomingPing.play()
            newConnPopup.open()
            newConnPopup.aboutToHide = function () {
                devicesListModel.append({"name": "troll", "address": "lol"})
            }
        }
    }
}
