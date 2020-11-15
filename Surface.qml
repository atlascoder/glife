import QtQuick 2.12
import QtQml 2.12
import ua.kpi.lifemodel 1.0

Rectangle {
    id: root
    color: "black"
    LifeModel {
        id: lifemodel
        onDataChanged: canvas.requestPaint()
    }
    property alias rows: lifemodel.rows
    property alias cols: lifemodel.cols
    property double cellSize: width / cols > height / rows ? height / rows : width / cols
    function cycle() {
        lifemodel.cycle()
    }
    Canvas {
        id: canvas
        anchors.centerIn: parent
        width: cols*cellSize
        height: rows*cellSize
        onPaint: {
            var ctx = getContext("2d");
            const deadColor = Qt.rgba(1, 1, 1, 0.5);
            const aliveColor = Qt.rgba(1, 1, 1, 1);
            var active = lifemodel.data(lifemodel.index(0,0));
//            ctx.fillStyle = active ? aliveColor : deadColor;
            ctx.fillStyle = active ? Qt.rgba(Math.random(), Math.random(), Math.random(), 1) : deadColor;
            for (var r = 0; r < rows; r++) {
                for (var c = 0; c < cols; c++) {
                    if (active !== lifemodel.data(lifemodel.index(r,c))) {
                        active = !active;
                        ctx.fillStyle = active ? Qt.rgba(Math.random(), Math.random(), Math.random(), 1) : deadColor;
//                        ctx.fillStyle = active ? aliveColor : deadColor;
                    }
                    ctx.fillRect(c*cellSize, r*cellSize, cellSize, cellSize);
                }
            }
        }
        MouseArea {
            anchors.fill: parent
            onClicked: function(event){
                lifemodel.toggle(Math.floor(event.y / cellSize), Math.floor(event.x / cellSize))
            }
        }
    }
}
