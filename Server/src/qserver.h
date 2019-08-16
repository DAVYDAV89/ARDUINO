#ifndef QServer_H
#define QServer_H

#include "qport.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QDateTime>
#include <QTimer>
#include <QThread>
#include <QMutex>

class QServer : public QTcpServer
{
    Q_OBJECT
signals:
    void m_write_in_port(QByteArray);
    void m_write_in_socket(QByteArray);
    void m_readPortValue();
    void m_readPortMonitor();
//    void m_readSocket();

private:
    QTcpSocket *socket;
    QPort *m_port;
    QTimer *timerMonitoring;
    QThread *threadPort;
    QHostAddress hostAddress;

public:
    explicit QServer(){
        if(listen(QHostAddress::Any,5555)){

//            system("ip addr add 1.1.1.1/0.0.0.0 dev enp3s0");
            qDebug() << "Listening...";
        }
        else
            qDebug() << "Error while starting: "<< errorString();

    }

    void incomingConnection(qintptr handle) {

        socket = new QTcpSocket();
        socket->setSocketDescriptor(handle);
        qDebug() << "Client connect" << handle;

        threadPort = new QThread(this);
        m_port = new QPort();

        m_port->moveToThread(threadPort);//

        connect(threadPort, SIGNAL(started()), m_port, SLOT(setPort()) );//
        connect(m_port, SIGNAL(finished()), threadPort, SLOT(quit()) );//

        threadPort->start();

        connect(this, SIGNAL(m_write_in_port(QByteArray)), m_port, SLOT(writeInPort(QByteArray)));//

        connect(this, SIGNAL(m_readPortValue()), m_port, SLOT(readValue()));//
        connect(this, SIGNAL(m_readPortMonitor()), m_port, SLOT(readMonitor()));//

        connect(socket, SIGNAL(readyRead()),   this, SLOT(onReadyRead()));//
        connect(socket, SIGNAL(disconnected()),this, SLOT(sockDisc()));

        timerMonitoring = new QTimer();
        connect(timerMonitoring, SIGNAL(timeout()), this, SLOT (isMonitoring()));//

    }

    ~QServer(){
        if (timerMonitoring->isActive())
            timerMonitoring->stop();

        m_port->closePort();
    }

public slots:
    void onReadyRead(){

        QByteArray byteTX;
        socket->waitForReadyRead(100);
        byteTX  = socket->readLine();
//        qDebug() << "byteTX: " << byteTX;

        if (byteTX == "RR"){
            emit m_readPortValue();
            sleep(500);
            socket->write(m_port->getReadOnPort());
        }
        else
            onWrite(byteTX);
    }

    void onWrite(QByteArray byteTX){

        if (byteTX == "HH"){
            timerMonitoring->start(500);
//                qDebug() << "byteTX: " << byteTX;
        }
        if (byteTX == "LL"){
            timerMonitoring->stop();
            sleep(500);
//                qDebug() << "byteTX: " << byteTX;
        }
        emit m_write_in_port(byteTX);
    }

    void isMonitoring(){
        emit m_readPortMonitor();
        socket->write(m_port->getMonitoringText());
    }

    void sleep(int ms){
        static QMutex mutex;
        static QMutexLocker locker(&mutex);
        mutex.tryLock(ms);
    }

    void sockDisc(){
        qDebug()<<"sockDisc";

        if (timerMonitoring->isActive()){
            timerMonitoring->stop();
            m_write_in_port("LL");
        }

        socket->deleteLater();
        socket->close();

        m_port->deleteLater();
    }

    
};

#endif // QServer_H
