#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include <QAction>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QVector>
#include <QDebug>
#include <QMap>
#include <QRandomGenerator>
#include <QDebug>
#include "Router.h"
#include "bridge.h"
#include "BPDU.h"
#include "Packet.h"
#include "Port.h"
#include "Enums.h"
#include "SupportFunctions.h"

class Scene : public QGraphicsScene
{
public:
    enum Mode {NoMode, SelectObject, DrawRouter};
    Scene(QObject* parent = 0);
    void setMode(Mode mode);

    //=======STP=========
    void initBPDU();

    void sendHello();
    void readHello();

    void sendBPDU(Router* router);
    void readBPDU();
    void readSingleBPDU(Router * router);
    void Listen();
    void Learn();
    void Forward();
    //===================
    void assignRootDefault(Router * router, BPDU temp);
    void assignDPNDP(Router * router, BPDU temp);
    bool checkRoot();
    bool checkPort();
    void drawPorts();
    void forceRead(BPDU b, Router* r, Port* p);
    void validateListenning();
    void getRootWay(Router * router, unsigned int dest);
    bool checkDoublePortAssign(Port * port);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void wheelEvent (QGraphicsSceneWheelEvent * event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e) override;
private:
    unsigned int GlobalRouterId = 1;//QRandomGenerator::global()->bounded(1000);
    unsigned int GlobalBridgeId = 1;//11QRandomGenerator::global()->bounded(1000);
    Mode sceneMode;
    QPointF origPoint;
    QGraphicsLineItem* itemToDraw;
    Bridge* bridgeToDraw;
    Router* rectToDraw;
    QGraphicsRectItem *rectItem;
    QVector<Router*> Routers;
    QVector<Bridge*> Bridges;
    Router * root;
    QVector<QGraphicsLineItem*> Ports;
    ModeSTP modeSTP;
    QGraphicsSimpleTextItem * txt;
    int recDepth = 0;
    //==========================
    int delayValue = 1000;
    //==========================
    void makeItemsControllable(bool areControllable);
};

#endif // SCENE_H
