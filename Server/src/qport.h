#ifndef QPORT_H
#define QPORT_H

#define DEBUG
//#define THREAD


#include <qdebug.h>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class QPort : public QSerialPort
{
    Q_OBJECT

signals:
    void finished();

public:
    QPort(){
#ifdef DEBUG
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
            qDebug() << "availablePorts: " << info.portName();
            setPortName(info.portName());
        }
#endif

    }

private:
    QList<QString> availablePorts;//
    QByteArray result_read;//
    QByteArray result_monitor;//

private slots:
    QList<QString> getAvailablePorts(){//
        return availablePorts;
    }

    void setPort(){//Открываем порт
#ifndef DEBUG
        setPortName("/dev/ttyS0");
#endif
        open(QIODevice::ReadWrite);
        setBaudRate(QSerialPort::Baud9600);
        setDataBits(QSerialPort::Data8);
        setParity(QSerialPort::NoParity);
        setStopBits(QSerialPort::OneStop);
        setFlowControl(QSerialPort::NoFlowControl);
        qDebug() << "UsePortName: " << portName();
    }

    void writeInPort(QByteArray array){//Запись в порт
        write(array + "\r\n");
        waitForBytesWritten(500);
        clear(QSerialPort::Output);

#ifdef DEBUG
        qDebug()<<"writeInPort(): "<<array;
#endif
    }

    void readValue(){

        writeInPort("RR");//
        result_read = readAll();
        while(waitForReadyRead(200))
            result_read.append(readAll());


#ifdef DEBUG
        result_read = "TH25.00TL-25.00ILDD";
        qDebug()<<"readValue(): "<<result_read;
#endif
        clear(QSerialPort::Input);
    }

    void  readMonitor(){

        if(waitForReadyRead(200));
        result_monitor = (readLine(8));

        if ( (result_monitor[0] == '(') && ( result_monitor[1] == ')') )
            result_monitor = result_monitor.mid(2,5);
        else
            result_monitor = "";


#ifdef DEBUG
        result_monitor = "25.00";
        qDebug()<<"readMonitor(): "<<result_monitor;
#endif
        clear(QSerialPort::Input);
    }

public slots:
    QByteArray getReadOnPort(){
        return result_read;
    }

    QByteArray getMonitoringText(){
        return result_monitor;
    }

    void closePort(){
        writeInPort("LL");
        emit finished();
        close();
    }
};

#endif // PORT_H
