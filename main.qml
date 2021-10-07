import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import titkov.universeview 1.0
import titkov.lifecontroller 1.0
import titkov.sampleview 1.0
import titkov.samplescatalog 1.0


Window {
    id: root
    width: 800
    height: 600
    visible: true
    title: qsTr("Game of Life")

    property double universeViewScale: zoomInput.value

    SamplesCatalog {
        id: samplesCatalog
    }

    Rectangle {
        id: controlBar
        anchors.top: parent.top
        width: root.width
        color: "silver"
        height: 32
        property int buttonHeight: 28
        Row {
            width: parent.width
            anchors.leftMargin: 16
            anchors.rightMargin: 16
            anchors.verticalCenter: parent.verticalCenter
            height: controlBar.buttonHeight
            spacing: 4
            Button {
                id: runButton
                height: parent.height
                text: lifeController.simulationActive ? "STOP" : "RUN"
                onClicked: {
                    if (lifeController.simulationActive)
                        lifeController.stop();
                    else
                        lifeController.start();
                }
            }
            Button {
                id: stepButton
                height: parent.height
                visible: !lifeController.simulationActive
                text: "STEP"
                onClicked: lifeController.step()
            }
            Row {
                height: parent.height
                spacing: 8
                visible: !lifeController.simulationActive
                Text { text: "Height:"; anchors.verticalCenter: parent.verticalCenter }
                SpinBox {
                    id: rowsInput
                    height: parent.height
                    value: lifeController.height
                    from: 8
                    to: 10000
                    stepSize: 8
                    editable: true
                    onValueChanged: lifeController.height = value
                }
            }
            Row {
                height: parent.height
                spacing: 8
                visible: !lifeController.simulationActive
                Text { text: "Width:"; anchors.verticalCenter: parent.verticalCenter }
                SpinBox {
                    id: colsInput
                    height: parent.height
                    value: lifeController.width
                    from: 8
                    to: 10000
                    stepSize: 8
                    editable: true
                    onValueChanged: lifeController.width = value
                }
            }
            ComboBox {
                height: parent.height
                model: ListModel {
                    ListElement { text: "Closing" }
                    ListElement { text: "Dead" }
                    ListElement { text: "Alive" }
                }

                currentIndex: lifeController.borderMode
                onCurrentIndexChanged: lifeController.borderMode = currentIndex
            }
            Button {
                id: randomButton
                height: parent.height
                text: "RND"
                visible: !lifeController.simulationActive
                onClicked: lifeController.randomize()
            }
            Button {
                id: clearButton
                height: parent.height
                visible: !lifeController.simulationActive
                text: "CLEAR"
                onClicked: lifeController.clear()
            }
            Button {
                id: loadButton
                height: parent.height
                text: "INSERT"
                visible: !lifeController.simulationActive
                onClicked: lifeController.insertingMode = !lifeController.insertingMode
                down: lifeController.insertingMode
            }
        }
    }


    Rectangle {
        id: statusBar
        width: parent.width
        anchors.bottom: parent.bottom
        height: 16
        color: '#A0A0A0'

        Row {
            leftPadding: 8
            rightPadding: 8
            spacing: 16
            Text {
                id: statusText
                width: 50
                text: lifeController.simulationActive ? 'SIM' : 'IDLE'
                font.bold: true
            }
            Text {
                id: populationText
                width: 120
                text: 'population: ' + lifeController.population
                font.bold: true
            }
            Text {
                id: generationText
                width: 120
                text: 'generation: ' + lifeController.generation
                font.bold: true
            }
            Text {
                id: lgtimeText
                width: 120
                text: 'time: ' + lifeController.lastGenerationTime + ' us'
                font.bold: true
            }
        }
    }

    Rectangle {
        id: loaderPane
        anchors.right: parent.right
        anchors.top: controlBar.bottom
        anchors.bottom: statusBar.top
        width: 100
        visible: lifeController.insertingMode

        Rectangle {
            id: loaderHeading
            width: parent.width
            height: 32
            Text {
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: "Universe loader"
            }
        }

        GridView {
            id: samplesGridView
            anchors.top: loaderHeading.bottom
            anchors.bottom: parent.bottom
            width: parent.width
            cellWidth: 100
            cellHeight: 100
            model: samplesCatalog
            clip: true
            highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
            currentIndex: -1
            delegate: Item {
                id: sampleItem
                width: 100
                height: 100
                SampleView {
                    property bool wide: model.width >= model.height
                    width: wide ? 80 : 80 * model.width / model.height
                    height: wide ? 80 * model.height / model.width : 80
                    anchors.centerIn: parent
                    bitmap: model.bitmap
                    clip: true
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            universeOverlay.bitmap = bitmap
                            samplesGridView.currentIndex = index
                        }
                    }
                }
                Text {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    elide: Text.ElideRight
                    padding: 4
                    text: model.name
                }
            }
        }
    }

    LifeController {
        id: lifeController
        interval: speedInput.value
        onDataChanged: universeView.setImage(bitmap)
        onInsertingModeChanged: samplesGridView.currentIndex = -1
    }

    Rectangle {
        anchors.top: controlBar.bottom
        anchors.bottom: statusBar.top
        anchors.left: parent.left
        anchors.right: loaderPane.left

        Flickable {
            id: universePane
            anchors.top: parent.top
            anchors.bottom: paramsBar.top
            anchors.left: parent.left
            anchors.right: parent.right
            contentWidth: universeContainer.width
            contentHeight: universeContainer.height
            ScrollBar.vertical: ScrollBar {}
            ScrollBar.horizontal: ScrollBar {}
            clip: true
            Item {
                id: universeContainer
                width: universeView.scaledWidth <= universePane.width ? universePane.width : universeView.scaledWidth
                height: universeView.scaledHeight <= universePane.height ? universePane.height : universeView.scaledHeight
                UniverseView {
                    id: universeView
                    anchors.centerIn: parent
                    height: lifeController.height
                    width: lifeController.width
                    scale: universeViewScale
                    property int scaledHeight: height * scale
                    property int scaledWidth: width * scale
                    smooth: false
                    fillColor: "#f1f1f1"
                    cellColor: "black"
                    transformOrigin: "Center"
                    UniverseView {
                        id: universeOverlay
                        height: lifeController.height
                        width: lifeController.width
                        smooth: false
                        visible: lifeController.insertingMode
                        fillColor: "transparent"
                        cellColor: 'orange'
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: function(e) {
                            if (lifeController.insertingMode) {
                                lifeController.insertSample(universeOverlay.bitmap, e.x, e.y);
                                universeOverlay.clear()
                                samplesGridView.currentIndex = -1
                            }
                            else {
                                lifeController.toggle(e.x, e.y);
                            }
                        }
                        hoverEnabled: true
                        onPositionChanged: function(e) {
                            universeOverlay.originX = e.x
                            universeOverlay.originY = e.y
                        }
                    }
                }
            }
        }

        Rectangle {
            id: paramsBar
            anchors.bottom: parent.bottom
            width: parent.width
            height: 32
            Row {
                anchors.fill: parent
                spacing: 12
                Row {
                    height: parent.height
                    spacing: 8
                    Text { text: "Period, ms:"; anchors.verticalCenter: parent.verticalCenter }
                    Slider {
                        id: speedInput
                        width: 200
                        anchors.verticalCenter: parent.verticalCenter
                        from: 0
                        to: 1000
                        stepSize: 50
                        value: 200
                    }
                }
                Row {
                    height: parent.height
                    spacing: 8
                    Text { text: "Zoom:"; anchors.verticalCenter: parent.verticalCenter }
                    Slider {
                        id: zoomInput
                        width: 200
                        anchors.verticalCenter: parent.verticalCenter
                        from: 0.01
                        to: 10.0
                        value: 1.0
                    }
                }
            }
        }
    }
}
