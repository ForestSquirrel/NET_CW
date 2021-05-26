#ifndef ROUTER_H
#define ROUTER_H

#include <QPainter>
#include <QGraphicsItem>
#include <QVector>
#include <QString>
#include <QQueue>
#include "Enums.h"
#include "Port.h"
#include "Packet.h"
#include "BPDU.h"
#include "routerdialog.h"

class Port;

class Router :public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    enum { Type = UserType + 1 };
    int type() const override;
    explicit Router(QGraphicsRectItem *parent = nullptr, unsigned int GlobalRouterId = 0);
    QPen outlinePen = QPen(Qt::yellow, 2, Qt::DotLine);
    QPen outlinePenSelected = QPen(Qt::blue, 2, Qt::SolidLine);
    QPen pen = QPen(Qt::black, 2, Qt::DotLine);
    void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option,
               QWidget * widget) override;
    //#===================================#
    bool isRoot;
    unsigned int Id;
    unsigned int Priority;
    QVector<Port*> Ports;
    //QVector<Packet> Packets;
    BPDU ServicePacket;
    //QVector<QString> log;
    QVector<BPDU> serviceLog;
    QQueue<BPDU> recentBPDU;

    RouterDialog* d;
    bool has(BPDU b);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e) override;
    void editEntry();
};

#endif // ROUTER_H
