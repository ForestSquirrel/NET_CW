#ifndef SUPPORTFUNCTIONS_H
#define SUPPORTFUNCTIONS_H
#include <QTime>
#include<QCoreApplication>

inline void delay(int msecs){
    QTime dieTime= QTime::currentTime().addMSecs(msecs);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

#endif // SUPPORTFUNCTIONS_H
