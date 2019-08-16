#ifndef APPCORE_H
#define APPCORE_H

#include <QObject>
#include <QDebug>
#include <QList>
#include <QThread>
#include <QTimer>
#include <QApplication>

#include "qsocket.h"

class AppCore : public QObject
{
    Q_OBJECT
signals:

    void sendToQmlSlider(int valueTH = 0, int valueTL = 0 );//������ �������� ������ � qml-��������� (�������/����)
//    void sendToQmlSlider(int (*valueTH)(), int (*valueTL)() );//������ �������� ������ � qml-��������� (�������/����)
    void sendToQmlUI(QByteArray UD, QByteArray HL);//������ �������� ������ � qml-��������� (�����������/���)
    void sendDisconnect(bool connect = false); //������ �������� ���������� � qml-���������
    void sendMonitoring(QByteArray valueM);

    void m_connect();
    void m_disconnect();
    void m_isConnect();
    void m_write(QByteArray value);
    void m_monitoringText();

public:

    explicit AppCore(){

        socket.moveToThread(&connectingThread);
        connect(&connectingThread, SIGNAL(started()), &socket, SLOT(s_connected()) );
        connect(this, SIGNAL(m_connect()), &socket, SLOT(s_connected()));

        connect(&socket, SIGNAL(finished()), &connectingThread, SLOT(quit()) );
        connect(this, SIGNAL(m_disconnect()), &socket, SLOT(s_disconnected()));

        connect(&connectingTimer, SIGNAL(timeout()), this, SLOT (m_connectIsON()));
        connect(this, SIGNAL(m_isConnect()), &socket, SLOT(s_isConnected()));

        connect(this, SIGNAL(m_write(QByteArray)), &socket, SLOT(s_writeToHost(QByteArray)));//������ ������ �� ������
        connect(&socket, SIGNAL(readyRead()), &socket, SLOT (s_readOnHost()));//������ ������ � �������

        connect(&socket, SIGNAL(setValue()), this, SLOT (setValue()));//��������


        //        connect(&timerMonitoring, SIGNAL(timeout()), this, SLOT (readFromMonitoring()));//
        //        connect(this, SIGNAL(m_monitoringText()), &socket, SLOT(s_readMonitoring()));//
    }

    //public ����� ���������� �� qml �����
public slots:

    //������� � ��������
    void on_connect_clicked(){
        if (!connectingThread.isRunning()){
            connectingThread.start();
            if (!connectingTimer.isActive())
                connectingTimer.start(TIME);
            readValueFromServer(false);
        }
        else{
            emit m_disconnect();
            connectingTimer.stop();
        }
    }

    // ���. ����������
    void on_monitoringBox_clicked(bool checked){
        if(checked){
            emit m_write("LL");
            emit m_write("HH");
            connect(&socket, SIGNAL(setMonitoring()), this, SLOT (setMonitoring()));//�������� �����������
        }
        else{
            disconnect(&socket, SIGNAL(setMonitoring()), this, SLOT (setMonitoring()));//�������� �����������
            emit m_write("LL");
            emit sendMonitoring(nullptr);
        }
    }


    //������ "��������"
    void getValueFromQml(QByteArray TH = "0", QByteArray TL = "0", QByteArray UD = "UU", QByteArray HL = "IH"/*�������� � �����*/ ){

        //������������ � ����� ����� �������
        auto convertForWrite = [](QByteArray ThTl){
            qDebug() << "value" << ThTl;
            float valueF = ThTl.toFloat();
            uint8_t *p = reinterpret_cast<uint8_t*>(&valueF);
            for (int i = 0; i < 4; i++)
                ThTl += static_cast<char>(p[3-i]);
            return ThTl;
        };

        //���������� �� ������
        emit m_write("TH" + convertForWrite(TH));
        emit m_write("TL" + convertForWrite(TL));//���������� �� ������
        emit m_write(UD);//���������� �� ������
        emit m_write(HL);//���������� �� ������
    }

    //������ "���������"
    void readValueFromServer(bool checked){

        if(checked)//���� ���������� �������
            emit m_write("LL");

        emit m_write("RR");

        if(checked)//���� ���������� �������
            emit m_write("HH");
    }

private:
    QSocket socket; // ����� �������������� � ��������
    QThread connectingThread; //����� ������ �� ������

    int TIME = 1000;
    QTimer connectingTimer; //������ ����������� ����������

private slots:
    void m_connectIsON()
    {
        emit m_isConnect();

        if (socket.getConnect()){
            emit sendDisconnect(true);
        }
        else{
            emit m_connect();
            emit sendDisconnect(false);
        }
    }

    void setValue(){

        QApplication::setOverrideCursor(Qt::WaitCursor);

        int valueTH = 0;
        int valueTL = 0;
        QByteArray UD = "UU";
        QByteArray HL = "IH";

        QByteArray byteRX = socket.getReading();

        auto convertTH_ForQML = [byteRX, &valueTH](){
            for (int i = 0; i < byteRX.length(); i++) {
                if (byteRX[i] == 'T' && byteRX[i+1] == 'H'){
                    if (byteRX[i+2] == '-'){
                        if (byteRX[i+4] == '.')//0-9
                            valueTH = byteRX.mid(i+2,2).toInt();
                        if (byteRX[i+5] == '.')//10-45
                            valueTH = byteRX.mid(i+2,3).toInt();
                    }
                    else{
                        if (byteRX[i+3] == '.')//0-9
                            valueTH = byteRX.mid(i+2,1).toInt();
                        if (byteRX[i+4] == '.')//10-99
                            valueTH = byteRX.mid(i+2,2).toInt();
                        if (byteRX[i+5] == '.')//100-125
                            valueTH = byteRX.mid(i+2,3).toInt();
                    }
                }
            }
        };
        auto convertTL_ForQML = [byteRX, &valueTL](){
            for (int i = 0; i < byteRX.length(); i++) {
                if (byteRX[i] == 'T' && byteRX[i+1] == 'L'){
                    if (byteRX[i+2] == '-'){
                        if (byteRX[i+4] == '.')//0-9
                            valueTL = byteRX.mid(i+2,2).toInt();
                        if (byteRX[i+5] == '.')//10-45
                            valueTL = byteRX.mid(i+2,3).toInt();
                    }
                    else{
                        if (byteRX[i+3] == '.')//0-9
                            valueTL = byteRX.mid(i+2,1).toInt();
                        if (byteRX[i+4] == '.')//10-99
                            valueTL = byteRX.mid(i+2,2).toInt();
                        if (byteRX[i+5] == '.') {//100-125
                            valueTL = byteRX.mid(i+2,3).toInt();
                        }
                    }
                }
            }
        };
        auto convertUD_ForQML = [byteRX, &UD](){
            for (int i = 0; i < byteRX.length(); i++) {
                if(byteRX[i] == 'U' )
                    UD = "UU";
                else if(byteRX[i] == 'D' )
                    UD = "DD";
            }
        };
        auto convertHL_ForQML = [byteRX, &HL](){
            for (int i = 0; i < byteRX.length(); i++) {

                if(byteRX[i] == 'I'){
                    if(byteRX[i+1] == 'H')
                        HL = "IH";
                    else
                        if(byteRX[i+1] == 'L')
                            HL = "IL";
                }
            }
        };

        convertTH_ForQML();
        convertTL_ForQML();
        emit sendToQmlSlider(valueTH, valueTL);

        convertUD_ForQML();
        convertHL_ForQML();
        emit sendToQmlUI(UD, HL);

        QApplication::restoreOverrideCursor();
    }

    void setMonitoring(){

        QByteArray valueMonitoring = socket.getMonitoring();
        if(valueMonitoring.length() == 5){
            emit sendMonitoring(valueMonitoring);
        }
    }
};

#endif // APPCORE_H
