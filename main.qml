import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

Window {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("Game of Life")

    Timer {
        id: timer
        onTriggered: surface.cycle()
        interval: speedInput.value
        repeat: true
    }

    Rectangle {
        id: controlBar
        anchors.top: parent.top
        width: root.width
        color: "silver"
        height: 32
        Row {
            anchors.fill: parent
            spacing: 4
            Row {
                height: parent.height
                Text { text: "Rows:"; anchors.verticalCenter: parent.verticalCenter }
                SpinBox {
                    id: rowsInput
                    height: parent.height
                    value: surface.rows
                    from: 1
                    to: 1000
                }
            }
            Row {
                height: parent.height
                Text { text: "Cols:"; anchors.verticalCenter: parent.verticalCenter }
                SpinBox {
                    id: colsInput
                    height: parent.height
                    value: surface.cols
                    from: 1
                    to: 1000
                }
            }
            SpinBox {
                id: speedInput
                height: parent.height
                value: 1000
                stepSize: 50
                from: 50
                to: 5000
            }
            Button {
                id: runButton
                height: parent.height
                text: timer.running ? "STOP" : "RUN"
                onClicked: {
                    timer.running = !timer.running
                }
            }
        }
    }

    Surface {
        id: surface
        anchors.top: controlBar.bottom
        anchors.bottom: parent.bottom
        width: root.width
        rows: rowsInput.value
        cols: colsInput.value
    }
}
