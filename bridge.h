#ifndef BRIDGE_H
#define BRIDGE_H

#include <QGraphicsItem>
#include <QPainter>
#include "Enums.h"
#include "Port.h"
#include "bridgedialog.h"
class Port;

class Bridge : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
public:
    enum { Type = UserType + 2 };
    int type() const override;
    explicit Bridge(QGraphicsLineItem *parent = nullptr, Port *f = nullptr, Port *s = nullptr, unsigned int GlobalBridgeId = 0, unsigned int pc = 0/*, unsigned int p = 0*/);
    QPen activePen = QPen(Qt::green, 2, Qt::SolidLine);
    QPen disabledPen = QPen(Qt::red, 2, Qt::SolidLine);
    QPen basicPen = QPen(Qt::blue, 2, Qt::SolidLine);
    QPen rootPen = QPen(Qt::magenta, 2, Qt::SolidLine);
    //#===================================#
    Status stat;
    unsigned int Id;
    //unsigned int priority;
    unsigned int PathCost;
    Port *first;
    Port *second;
    //========================
    BridgeDialog* d;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e) override;
    void editEntry();
    void updColor();
};

#endif // BRIDGE_H
