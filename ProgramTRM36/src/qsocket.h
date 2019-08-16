#ifndef QSOCKET_H
#define QSOCKET_H

#include <QDebug>
#include <QTcpSocket>
#include <QThread>
#include <QMutex>
#include <QTimer>

class QSocket : public QTcpSocket
{
    Q_OBJECT

signals:
    void finished();
    void setValue();
    void setMonitoring();

public:
    QSocket(){
    }

    ~QSocket(){
    }
private:
    bool connectServer; //
    QByteArray resultRead; //
    QByteArray resultMonitoring; //
    
    
public slots:

    bool getConnect(){
        return connectServer;
    }

    QByteArray getReading(){
//        qDebug() << "resultRead" << resultRead;
        return resultRead;
    }
    QByteArray getMonitoring(){
//        qDebug() << "resultMonitoring" << resultMonitoring;
        return resultMonitoring;
    }

private slots:
    void s_connected(){//
        connectToHost("127.0.0.1",5555);
//        qDebug()<<"s_connected()";
    }

    void s_isConnected(){
        bool _state = state() == QTcpSocket::ConnectedState;
        connectServer = _state;
//        qDebug()<<"Connected(): " <<  connectServer;

    }

    void s_disconnected(){//
        disconnectFromHost();
        connectServer = state() == QTcpSocket::ConnectedState;
//        qDebug()<<"s_disconnected(): " <<  connectServer;
        emit finished();
    }

    void s_writeToHost(QByteArray value){//Запись на сервер
//        if (value != "RR")
            waitForReadyRead(500);
        write(value);
        waitForBytesWritten(500);
//        qDebug() << "s_writeToHost";
    }

    void s_readOnHost(){//
        resultMonitoring = readAll();

        if (resultMonitoring.length() == 5){
            emit setMonitoring();
//            qDebug() << "readyRead: " << resultMonitoring;
        }
        else {
            resultRead = resultMonitoring;
            emit setValue();
//            qDebug() << "readyRead: " << resultRead;
        }

    }

//    void sleep(int ms){
//        static QMutex mutex;
//        static QMutexLocker locker(&mutex);
//        mutex.tryLock(ms);
//    }
};

#endif // QSOCKET_H
