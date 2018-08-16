#include "additionalthread.h"
#include "mainwindow.h"

#include <iostream>

#include <QThread>
#include <QDebug>

AdditionalThread::AdditionalThread(QString pName, SystemControllerClass *pControl) : fName(pName) , fAddControl(pControl)
{
}

void AdditionalThread::getVAC1()
{
    while (true) {
        QString cStr = fAddControl->fTTiVolt->getVoltAndCurr();
        emit sendToThread(cStr);
        QThread::sleep(5);
    }
}

void AdditionalThread::getVAC2()
{
    while (true) {
        //QString cStr = fAddControl->fTTiVolt->getVoltAndCurr(2);
        //emit sendToThread2(cStr);
        QThread::sleep(10);
    }
}

void AdditionalThread::getRaspSensors()
{
    while(true){
        QString cStr = fAddControl->fConnectRasp->getInfoFromSensors();
        emit sendToThread(cStr);
        QThread::sleep(10);
    }
}