import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.2
import QtQuick.Controls.Styles 1.4

ApplicationWindow {
    id: window
    visible: true
    width: 430
    height: 740
    maximumHeight: height
    maximumWidth: width
    minimumHeight: height
    minimumWidth: width
    title: qsTr("Программатор")
    flags: Qt.Window | Qt.CustomizeWindowHint | Qt.WindowTitleHint | Qt.WindowCloseButtonHint | Qt.WindowSystemMenuHint

    Connections{
        target: appCore

        onSendToQmlSlider:{
            spinBoxTH.spinboxTH_Val = valueTH
            spinBoxTL.spinboxTL_Val = valueTL
        }
        onSendToQmlUI:{
            if (UD=="UU")
                up.checked = true
            else
                down.checked = true

            if (HL=="IH")
                minToMax.checked = true
            else
                maxToMin.checked = true
        }
        onSendDisconnect:{
            control.checked = connect;
        }
        onSendMonitoring:{
            textEditMonitoring.text = valueM
        }
    }

    //Соединение с сервером и мониторинг
    RowLayout {
        id: rowLayout1
        anchors.top: parent.top
        anchors.topMargin: 35
        anchors.right: parent.right
        anchors.rightMargin: 15
        anchors.left: parent.left
        anchors.leftMargin: 15
        height: parent.height/4

        Switch {
            id: control

            indicator: Rectangle {
                implicitWidth: 180
                implicitHeight: 40
                x: control.leftPadding
                y: parent.height / 2 - height / 2
                radius: 13
                color: control.checked ? "#17a81a" : "#ff0000"
                border.color: control.checked ? "#17a81a" : "#cccccc"

                Rectangle {
                    x: control.checked ? parent.width - width : 0
                    width: parent.width/2
                    height: 40
                    radius: 13
                    color: control.down ? "#cccccc" : "#ffffff"
                    border.color: control.checked ? (control.down ? "#17a81a" : "#21be2b") : "#999999"
                }
            }
            onClicked:
                appCore.on_connect_clicked()
        }

        ColumnLayout{
            Layout.preferredWidth: parent.width/2

            CheckBox {
                id: checkBox
                checked: false
                text: qsTr("Мониторинг")

                indicator: Rectangle {
                    implicitWidth: 26
                    implicitHeight: 26
                    x: checkBox.leftPadding
                    y: parent.height / 2 - height / 2
                    radius: 3
                    border.color: "gray"

                    Rectangle {
                        width: 14
                        height: 14
                        x: 6
                        y: 6
                        radius: 2
                        color: "gray"
                        visible: checkBox.checked
                    }

                }
                contentItem: Text {
                    text: checkBox.text
                    font: checkBox.font
                    opacity: enabled ? 1.0 : 0.3
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: checkBox.indicator.width + checkBox.spacing
                }
                onToggled: {
                    appCore.on_monitoringBox_clicked(checkBox.checked)
                }
            }
            Frame {
                id: groupBox3
                Layout.fillWidth:  true
                Layout.fillHeight: true

                background: Rectangle {
                    Layout.preferredWidth: parent.width
                    border.color: "gray"
                    radius: 15
                    color: checkBox.checked ? "#ffffff" : "#cccccc"
                }
                TextEdit {
                    id: textEditMonitoring
                    color: "#f22828"
                    font.bold: true
                    anchors.topMargin: 15
                    anchors.bottomMargin: 15
                    anchors.rightMargin: 15
                    anchors.leftMargin: 15
                    anchors.fill: parent
                    font.pixelSize: 50
                    horizontalAlignment: Text.AlignHCenter
                    font.weight: Font.ExtraBold
                    font.capitalization: Font.SmallCaps
                    enabled: checkBox.checked
                    readOnly: true                    
                }
            }
        }
    }

    //Значения TH/TL
    Frame {
        id: groupBox2
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 15
        anchors.bottom: rowLayout2.top
        anchors.bottomMargin: 15
        height: parent.height/4

        background: Rectangle {
            color: "transparent"
            border.color: "gray"
            radius: 15
        }

        GridLayout {
            id: gridLayout
            rows: 2
            columns: 3
            anchors.fill: parent
            columnSpacing: 20
            rowSpacing: 40

            Label {
                id: label
                text: qsTr("TH:")
                Layout.fillWidth: false
            }

            SpinBox {
                id: spinBoxTH
                from: -45// * 100
                value: spinboxTH_Val
                to: 125// * 100
                stepSize: 1
                editable: true
                //                property int decimals: 2
                //                property real realValue: value

                //                textFromValue: function(value, locale) {
                //                    return Number(value / 100).toLocaleString(locale, 'f', spinboxTH.decimals)
                //                }

                property real spinboxTH_Val: 0
                onValueChanged: {
                    sliderTH.sliderTH_Val = value
                }

                Layout.fillWidth: false
            }

            Slider {
                id: sliderTH
                from: -45// * 100
                value: sliderTH_Val
                to: 125// * 100
                stepSize: 1
                Layout.fillHeight: false
                Layout.fillWidth: true

                property real sliderTH_Val: 0
                onValueChanged: {
                    spinBoxTH.spinboxTH_Val = value
                }
            }

            Label {
                id: label1
                text: qsTr("TL:")
                Layout.fillWidth: false
            }

            SpinBox {
                id: spinBoxTL
                from: -45// * 100
                value: spinboxTL_Val
                to: 125// * 100
                stepSize: 1
                editable: true
                //                property int decimals: 2
                //                property real realValue: value / 100

                //                textFromValue: function(value, locale) {
                //                    return Number(value / 100).toLocaleString(locale, 'f', spinBoxTL.decimals)
                //                }
                Layout.fillWidth: false

                property real spinboxTL_Val: 0
                onValueChanged: {
                    sliderTL.sliderTL_Val = value
                }
            }

            Slider {
                id: sliderTL
                from: -45// * 100
                value: sliderTL_Val
                to: 125// * 100
                stepSize: 1
                Layout.fillHeight: false
                Layout.fillWidth: true

                property real sliderTL_Val: 0
                onValueChanged: {
                    spinBoxTL.spinboxTL_Val = value
                }
            }
        }
    }

    //Направления тока и температуры
    RowLayout {
        id: rowLayout2
        anchors.right: parent.right
        anchors.rightMargin: 15
        anchors.left: parent.left
        anchors.leftMargin: 15
        anchors.bottom: rowLayout3.top
        anchors.bottomMargin: 15
        height: parent.height/4

        ColumnLayout{
            Layout.preferredWidth: parent.width/2
            Label {
                Layout.fillHeight: true
                Layout.fillWidth:  true
                font.pointSize: 12

                text: qsTr("<b>Температура</b>")
            }
            Frame {
                id: frameUpDown

                Layout.fillHeight: true
                Layout.fillWidth:  true

                background: Rectangle {
                    color: "transparent"
                    border.color: "gray"
                    radius: 15
                }

                ColumnLayout {
                    id: columnLayout

                    RadioButton {
                        id: up
                        text: qsTr("Вверх")
                        font.pointSize: 15
                        checked: true
                        Layout.topMargin: 15
                        indicator: Rectangle {
                            implicitWidth: 35
                            implicitHeight: 35
                            radius: 30
                            border.color: up.activeFocus ? "darkgray" : "gray"
                            border.width: 1
                            Rectangle {
                                anchors.fill: parent
                                visible: up.checked
                                color: "#555"
                                radius: 30
                                anchors.margins: 4
                            }
                        }
                        property string ud
                        ud: up.checked ? "UU" : "DD"
                    }

                    RadioButton {
                        id: down
                        text: qsTr("Вниз")
                        font.pointSize: 15
                        Layout.topMargin: 15
                        indicator: Rectangle {
                            implicitWidth: 35
                            implicitHeight: 35
                            radius: 30
                            border.color: down.activeFocus ? "darkgray" : "gray"
                            border.width: 1
                            Rectangle {
                                anchors.fill: parent
                                visible: down.checked
                                color: "#555"
                                radius: 30
                                anchors.margins: 4
                            }
                        }
                    }
                }
            }
        }

        ColumnLayout{
            Layout.preferredWidth: parent.width/2
            Label {
                Layout.fillHeight: true
                Layout.fillWidth:  true
                font.pointSize: 12

                text: qsTr("<b>Ток</b>")
            }
            Frame {
                id: frameminToMax

                Layout.fillWidth:  true
                Layout.fillHeight: true

                background: Rectangle {
                    color: "transparent"
                    border.color: "gray"
                    radius: 15
                }

                ColumnLayout {
                    id: columnLayout1

                    RadioButton {
                        id: minToMax
                        text: qsTr("Imin -> Imax")
                        font.pointSize: 15
                        checked: true
                        Layout.topMargin: 15
                        indicator: Rectangle {
                            implicitWidth: 35
                            implicitHeight: 35
                            radius: 30
                            border.color: minToMax.activeFocus ? "darkgray" : "gray"
                            border.width: 1
                            Rectangle {
                                anchors.fill: parent
                                visible: minToMax.checked
                                color: "#555"
                                radius: 30
                                anchors.margins: 4
                            }
                        }
                        property string ih
                        ih: minToMax.checked ? "IH" : "IL"
                    }


                    RadioButton {
                        id: maxToMin
                        text: qsTr("Imax -> Imin")
                        font.pointSize: 15
                        Layout.topMargin: 15
                        indicator: Rectangle {
                            implicitWidth: 35
                            implicitHeight: 35
                            radius: 30
                            border.color: maxToMin.activeFocus ? "darkgray" : "gray"
                            border.width: 1
                            Rectangle {
                                anchors.fill: parent
                                visible: maxToMin.checked
                                color: "#555"
                                radius: 30
                                anchors.margins: 4
                            }
                        }
                    }
                }
            }

        }
    }

    //Кнопки
    RowLayout {
        id: rowLayout3

        anchors.right: parent.right
        anchors.rightMargin: 15
        anchors.left: parent.left
        anchors.leftMargin: 15
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 15
        height: parent.height/10

        Button {
            id: send
            text: "Записать"
            font.pointSize: 15
            Layout.preferredWidth: parent.width/2
            Layout.fillHeight: true
            background: Rectangle {
                color: send.down ? "#d6d6d6" : "#f6f6f6"
                border.color: "#26282a"
                border.width: 2
                radius: 15
            }
            onClicked: {
                appCore.getValueFromQml(spinBoxTH.spinboxTH_Val,spinBoxTL.spinboxTL_Val, up.ud,minToMax.ih)
            }

        }

        Button {
            id: write
            text: "Прочитать"
            font.pointSize: 15
            Layout.preferredWidth: parent.width/2
            Layout.fillHeight: true
            background: Rectangle {
                color: write.down ? "#d7d7d7" : "#f7f7f7"
                border.color: "#26282a"
                border.width: 2
                radius: 15
            }
            onClicked: {
                appCore.readValueFromServer(checkBox.checked)
            }
        }
    }
}
