import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.2

Page {
    id: pageMediaClass
    width: 600
    height: 400

    signal selectedMedia(url fileName)

    header: Label {
        text: qsTr("Media")
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }

    FileDialog {
        id: mediaSelectionDialog
        title: "Please choose a media file"
        folder: shortcuts.home
        selectMultiple: false
        selectExisting: true
        selectFolder: false
        onAccepted: selectedMedia(mediaSelectionDialog.fileUrl)
        nameFilters: ["Video files (*.mp4 *.mkv *.avi *.mov)", "Audio files (*.mp3 *.m4a *.flac *.mp2 *.wav)", "All files (*.*)"]
        visible: false
    }

    TextField {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: selectMediaBtn.left
        anchors.rightMargin: 20

        id: mediaPathInput
        placeholderText: "Select media file"
        Connections {
            target: pageMediaClass
            function onSelectedMedia(fileName) {
                mediaPathInput.text = fileName
            }
        }
    }

    Button {
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.verticalCenter: parent.verticalCenter
        id: selectMediaBtn
        text: "Browse"
        onClicked: mediaSelectionDialog.open()
    }
}
