#include "scene.h"

Scene::Scene(QObject* parent): QGraphicsScene(parent)
{
    sceneMode = NoMode;
    bridgeToDraw = 0;
    itemToDraw = 0;
    rectToDraw = 0;
    rectItem = 0;
    root = 0;
    modeSTP = ModeSTP::Listenning;
    txt = new QGraphicsSimpleTextItem;
    txt->setPos(0,-350);
    txt->setText("Current Mode: Listenning");
    txt->setFont(QFont("Helvetica [Cronyx]", 12, QFont::Bold));
    this->addItem(txt);
}

void Scene::setMode(Mode mode){
    sceneMode = mode;
    QGraphicsView::DragMode vMode =
            QGraphicsView::NoDrag;
    if(mode == SelectObject){
        makeItemsControllable(true);
        vMode = QGraphicsView::RubberBandDrag;
    }
    else if(mode == DrawRouter){
        makeItemsControllable(false);
        vMode = QGraphicsView::NoDrag;
    }
    QGraphicsView* mView = views().at(0);
    if(mView)
        mView->setDragMode(vMode);
}

void Scene::makeItemsControllable(bool areControllable){
    foreach(QGraphicsItem* item, items()){
        item->setFlag(QGraphicsItem::ItemIsSelectable,
                      areControllable);
        item->setFlag(QGraphicsItem::ItemIsMovable,
                      areControllable);
        item->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    }
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if  (sceneMode == DrawRouter)
        origPoint = event->scenePos();
    else if (sceneMode == SelectObject){
        foreach (QGraphicsItem* item, selectedItems()){
            if (item->type() == QAbstractGraphicsShapeItem::UserType + 1){
                item->update();
            }
        }
    }
    QGraphicsScene::mousePressEvent(event);
}
void Scene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e){
    QGraphicsScene::mouseDoubleClickEvent(e);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
        QGraphicsScene::mouseMoveEvent(event);
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if(sceneMode == DrawRouter){
        if(!rectToDraw){
            rectToDraw = new Router;

            rectToDraw->Id = GlobalRouterId;
            GlobalRouterId++;
            rectToDraw->Priority = QRandomGenerator::global()->bounded(1, 1000);
            this->addItem(rectToDraw);
            rectToDraw->setPos(origPoint);
        }
        Routers.push_back(rectToDraw);
        rectToDraw = 0;
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

void Scene::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Delete){
        Routers.clear();
        Bridges.clear();
        Ports.clear();
        root = nullptr;
        foreach(QGraphicsItem * item, this->items()){
            if (item != txt){
                this->removeItem(item);
                delete(item);
            }
        }
        recDepth = 0;
        GlobalBridgeId = 1;
        GlobalRouterId = 1;
        this->modeSTP = ModeSTP::Listenning;
        txt->setText("Current Mode: Listenning");
        this->update();
    }
    else
        QGraphicsScene::keyPressEvent(event);
}
void Scene::keyReleaseEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_B){
        if (selectedItems().size() == 2){
            if (selectedItems().at(0)->type() == QAbstractGraphicsShapeItem::UserType + 1 &&
                    selectedItems().at(1)->type() == QAbstractGraphicsShapeItem::UserType + 1){
                if (!bridgeToDraw){
                    bridgeToDraw = new Bridge;
                    Port* port1 = new Port;
                    Port* port2 = new Port;
                    QLineF line;
                    bridgeToDraw->Id = GlobalBridgeId;
                    GlobalBridgeId++;
                    bridgeToDraw->first = port1;
                    bridgeToDraw->second = port2;
                    bridgeToDraw->PathCost = QRandomGenerator::global()->bounded(1, 100);
                    bool first = true;
                    port1->link = bridgeToDraw;
                    port2->link = bridgeToDraw;
                    foreach(Router* r, Routers){
                        if (r->isSelected() && first){
                            line.setP1(QPointF(r->scenePos().x() + 29, r->scenePos().y() + 23));
                            port1->r = r;
                            r->Ports.push_back(port1);
                            first = false;
                        } else if (r->isSelected() && !first){
                            port2->r = r;
                            r->Ports.push_back(port2);
                            line.setP2(QPointF(r->scenePos().x() + 29, r->scenePos().y() + 23));
                        }
                    }
                    bridgeToDraw->setLine(line);
                    this->addItem(bridgeToDraw);
                }
                bridgeToDraw->updColor();
                this->Bridges.push_back(bridgeToDraw);
                bridgeToDraw = 0;
            }
        }
    }
    else if (event->key() == Qt::Key_1){
        if (modeSTP == ModeSTP::Listenning){
            Listen();
            txt->setText("Current Mode: Learning");
            this->update();
        }
    }
    else if (event->key() == Qt::Key_2){
        if (modeSTP == ModeSTP::Learning){
            Learn();
            txt->setText("Current Mode: Forwarding");
            this->update();
        }
    }
    else if (event->key() == Qt::Key_4){

    }
    else
        QGraphicsScene::keyReleaseEvent(event);
}

void Scene::wheelEvent(QGraphicsSceneWheelEvent *event){
       qreal degrees;
       qreal scaleFactor;
       if (selectedItems().size() == 1){
           scaleFactor = selectedItems().at(0)->scale();
           degrees = event->delta() / 120;
           scaleFactor += degrees / 10;
           qDebug() << event->delta() << degrees << scaleFactor;
           selectedItems().at(0)->setScale(scaleFactor);
       }
       QGraphicsScene::wheelEvent(event);
}

//======================================STP========================================
void Scene::initBPDU(){
        foreach(Router* router, Routers){
            router->ServicePacket = BPDU(router->Id, 0, router->Id, 0, router->Priority, false);
        }
}
void Scene::sendHello(){
    foreach(Router* router, Routers){
        foreach(Port* port, router->Ports){
            if (port->link->first->r->Id == router->Id){
                BPDU temp = BPDU(router->ServicePacket.RootId, router->ServicePacket.RootPathCost + port->link->PathCost, router->Id, port->link->second->r->Id, router->ServicePacket.priority, false);
                port->link->second->r->serviceLog.push_back(temp);
                port->link->second->r->recentBPDU.enqueue(temp);
            } else if (port->link->second->r->Id == router->Id){
                BPDU temp = BPDU(router->ServicePacket.RootId, router->ServicePacket.RootPathCost + port->link->PathCost, router->Id, port->link->first->r->Id,router->ServicePacket.priority, false);
                port->link->first->r->serviceLog.push_back(temp);
                port->link->first->r->recentBPDU.enqueue(temp);
            }
        }
    }
}
void Scene::readHello(){
    QQueue<BPDU> priorityQ;
    QQueue<BPDU> idQ;
    unsigned int minP;
    unsigned int minD;
    foreach(Router* router, Routers){
        minD = router->Id;
        minP = UINT_MAX;
        while (!router->recentBPDU.empty()){
            BPDU temp = router->recentBPDU.dequeue();
            if (temp.RootId != router->Id){
                if (minP > temp.priority){
                    minP = temp.priority;
                    priorityQ.clear();
                    priorityQ.enqueue(temp);
                } else if (temp.priority == minP){
                    priorityQ.enqueue(temp);
                }
            }
        }
        if (priorityQ.size() == 1){
            BPDU temp = priorityQ.dequeue();
            if (router->ServicePacket.priority > temp.priority){
                router->ServicePacket.RootId = temp.RootId;
                router->ServicePacket.priority = temp.priority;
                router->ServicePacket.RootPathCost = temp.RootPathCost;
                router->isRoot = false;
                assignRootDefault(router, temp);
            } else if (router->ServicePacket.priority == temp.priority && router->ServicePacket.RootId > temp.RootId){
                router->ServicePacket.RootId = temp.RootId;
                router->ServicePacket.priority = temp.priority;
                router->ServicePacket.RootPathCost = temp.RootPathCost;
                router->isRoot = false;
                assignRootDefault(router, temp);
            }
        } else if (priorityQ.size() > 1){
            if (minP < router->ServicePacket.priority){
                minD = UINT_MAX;
            }
            while (!priorityQ.empty()){
                BPDU temp = priorityQ.dequeue();
                if (minD > temp.RootId){
                    minD = temp.RootId;
                    idQ.clear();
                    idQ.enqueue(temp);
                }
            }
            if (idQ.size() >= 1){
                BPDU temp = idQ.dequeue();
                router->ServicePacket.RootId = temp.RootId;
                router->ServicePacket.priority = temp.priority;
                router->ServicePacket.RootPathCost = temp.RootPathCost;
                router->isRoot = false;
                assignRootDefault(router, temp);
            }
        }
    }
}
void Scene::Listen(){
    initBPDU();
    while (!checkRoot()){
        sendHello();
        readHello();
        this->drawPorts();
        this->update();
        delay(delayValue);
    }
    foreach(Port* p, root->Ports){
        if (p->stat == Status::Default){
            if (p->link->first == p){
                p->link->second->stat = Status::Root;
            }
            else{
                p->link->first->stat = Status::Root;
            }
        }
    }
    validateListenning();
    foreach(Bridge* b, Bridges){
        if (b->first->stat == Status::Root){
            b->second->stat = Status::DP;
        }
        else if (b->second->stat == Status::Root){
            b->first->stat = Status::DP;
        }
    }
    this->drawPorts();
    this->update();
    modeSTP = ModeSTP::Learning;
}

void Scene::sendBPDU(Router* router){
    if (!router->recentBPDU.empty()){
        readSingleBPDU(router);
    }
    foreach(Port* port, router->Ports){
        if (port->link->first->r->Id == router->Id && port->stat != Status::Root && port->stat != Status::NDP){
            BPDU temp = BPDU(root->Id, router->ServicePacket.RootPathCost + port->link->PathCost, router->Id, port->link->second->r->Id, router->Priority, true);
            if (!port->link->second->r->recentBPDU.contains(temp)){
                port->link->second->r->serviceLog.push_back(temp);
                port->link->second->r->recentBPDU.enqueue(temp);

                qDebug() << router->Id << " -> "<< port->link->second->r->Id << "\n";
            }
            else
                return;
            //==recursive send==
            if (!port->link->second->r->isRoot)
                sendBPDU(port->link->second->r);
            qDebug() << recDepth++ << '\n';
            //==================
        } else if (port->link->second->r->Id == router->Id && port->stat != Status::Root && port->stat != Status::NDP){
            BPDU temp = BPDU(root->Id, router->ServicePacket.RootPathCost + port->link->PathCost, router->Id, port->link->first->r->Id,router->Priority, true);
            if (!port->link->first->r->recentBPDU.contains(temp)){
                port->link->first->r->serviceLog.push_back(temp);
                port->link->first->r->recentBPDU.enqueue(temp);

                qDebug() << router->Id << " -> "<< port->link->first->r->Id << "\n";
            }
            else
                return;
            //==recursive send==
            if (!port->link->first->r->isRoot)
                sendBPDU(port->link->first->r);
            qDebug() << recDepth++ << '\n';
            //==================
        }
    }
}
void Scene::readBPDU(){
    foreach (Router * router, Routers){
        readSingleBPDU(router);
        router->recentBPDU.clear();
    }
}

void Scene::readSingleBPDU(Router * router){
    unsigned int minRPC = UINT_MAX;
    unsigned int minP = UINT_MAX;
    unsigned int minId = UINT_MAX;
    QQueue<BPDU> rpcQ;
    QQueue<BPDU> pQ;
    QQueue<BPDU> IdQ;
    QQueue<BPDU> copy;
    rpcQ.clear();
    pQ.clear();
    IdQ.clear();
    if (!router->recentBPDU.empty()){
        copy = router->recentBPDU;
        foreach(BPDU b, copy){
            if (b.SourceId == root->Id){
                copy.removeOne(b);
            }
            foreach(Port* p, router->Ports){
                if ((b.SourceId == p->link->first->r->Id || b.SourceId == p->link->second->r->Id) && p->stat != Status::Default){
                    copy.removeOne(b);
                }
            }
        }
        //router->recentBPDU.clear();
    }
    while (!copy.empty()){
        BPDU temp = copy.dequeue();
        if (temp.RootPathCost < minRPC){
            minRPC = temp.RootPathCost;
            rpcQ.clear();
            rpcQ.enqueue(temp);
        } else if (temp.RootPathCost == minRPC){
            rpcQ.enqueue(temp);
        }
    }
    if (rpcQ.size() >= 1){
        while (!rpcQ.empty()){
            BPDU temp = rpcQ.dequeue();
            if (temp.priority < minP){
                minP = temp.priority;
                pQ.clear();
                pQ.enqueue(temp);
            } else if (temp.priority == minP){
                pQ.enqueue(temp);
            }
        }
        if (pQ.size() >= 1){
            while (!pQ.empty()){
                BPDU temp = pQ.dequeue();
                if (temp.SourceId < minId){
                    minId = temp.SourceId;
                    IdQ.clear();
                    IdQ.enqueue(temp);
                } else if (temp.SourceId == minId){
                    IdQ.enqueue(temp);
                }
            }
            if (IdQ.size() == 1){
                BPDU temp = IdQ.dequeue();
                assignDPNDP(router, temp);
            } else if (IdQ.size() > 1){
                BPDU temp = IdQ.dequeue();
                assignDPNDP(router, temp);
                qDebug() << "overflow \n";
            }
        }
    }
}
void Scene::Learn(){
    sendBPDU(root);
    foreach (Router * router, Routers)
        router->recentBPDU.clear();
    this->drawPorts();
    this->update();
    modeSTP = ModeSTP::Forwarding;
}
void Scene::Forward(){

}
//=================================================================================
void Scene::assignRootDefault(Router * router, BPDU temp){
    foreach(Port* port, router->Ports){
        if (port->link->first->r->Id ==  temp.SourceId ||
                port->link->second->r->Id ==  temp.SourceId){
            port->stat = Status::Root;
        }
        else{
            port->stat = Status::Default;
        }
    }
}
void Scene::assignDPNDP(Router * router, BPDU temp){
    foreach(Port * port, router->Ports){
        if (port->link->first->r->Id == temp.SourceId && port->stat == Status::Default){
            if (router->ServicePacket.RootPathCost <= temp.RootPathCost){
                port->link->second->stat = Status::NDP;
                qDebug() << port->link->second->r->Id << " -> "<< port->link->first->r->Id << " mirror \n";
                forceRead(BPDU(root->Id, port->link->second->r->ServicePacket.RootPathCost + port->link->PathCost, port->link->second->r->Id, port->link->first->r->Id, port->link->second->r->Priority, true),
                          port->link->first->r,
                          port->link->first);
            }
            else{
                port->link->first->stat = Status::DP;
                port->link->first->r->serviceLog.push_back(BPDU(root->Id, port->link->second->r->ServicePacket.RootPathCost + port->link->PathCost, port->link->second->r->Id, port->link->first->r->Id, port->link->second->r->Priority, true));
                port->link->second->stat = Status::NDP;
            }
        }
        else if (port->link->second->r->Id == temp.SourceId && port->stat == Status::Default){
            if (router->ServicePacket.RootPathCost <= temp.RootPathCost){
                port->link->first->stat = Status::NDP;
                qDebug() << port->link->first->r->Id << " -> "<< port->link->second->r->Id << " mirror \n";
                forceRead(BPDU(root->Id, port->link->first->r->ServicePacket.RootPathCost + port->link->PathCost, port->link->first->r->Id, port->link->second->r->Id, port->link->first->r->Priority, true),
                          port->link->second->r,
                          port->link->second);
            }
            else{
                port->link->second->stat = Status::DP;
                port->link->second->r->serviceLog.push_back(BPDU(root->Id, port->link->second->r->ServicePacket.RootPathCost + port->link->PathCost, port->link->first->r->Id, port->link->second->r->Id, port->link->first->r->Priority, true));
                port->link->first->stat = Status::NDP;
            }
        }
    }
}
void Scene::validateListenning(){
    bool noRoot = true;
    foreach(Router * router, Routers){
        noRoot = true;
        foreach(Port* port, router->Ports){
            if (port->stat == Status::Root){
                noRoot = false;
            }
        }
        if (noRoot && router!= root){
            getRootWay(root, router->Id);
            validateListenning();
        }
    }
}
void Scene::getRootWay(Router* router, unsigned int dest){
    foreach(Port * p, router->Ports){
        if (p->link->first->r->Id == dest && p->link->first->stat == Status::Default){
            p->link->first->stat = Status::Root;
        }
        else if (p->link->second->r->Id == dest && p->link->second->stat == Status::Default){
            p->link->second->stat = Status::Root;
        }
        else{
            if (p->link->first == p && p->link->first->stat == Status::Root)
                getRootWay(p->link->second->r, dest);
            else if (p->link->second == p && p->link->second->stat == Status::Root)
                getRootWay(p->link->first->r, dest);
        }
    }
}
void Scene::forceRead(BPDU b, Router* r, Port* p){
    if (!r->has(b))
        r->serviceLog.push_back(b);
    if (b.RootPathCost < r->ServicePacket.RootPathCost){
        p->stat = Status::DP;
    }
    else if (b.RootPathCost == r->ServicePacket.RootPathCost){
        if (b.priority < r->Priority){
            p->stat = Status::DP;
        }
        else if (b.priority == r->Priority){
            if (b.SourceId < r->Id){
                p->stat = Status::DP;
            }
            else{
                p->stat = Status::NDP;
            }
        }
        else{
            p->stat = Status::NDP;
        }
    }
    else{
        p->stat = Status::NDP;
    }
}
bool Scene::checkRoot(){
    QVector<Router*> roots;
    foreach (Router* r, Routers){
        qDebug() << r->Id << " is " << r->isRoot << '\n';
        if (r->isRoot){
            roots.push_back(r);
        }
    }
    if (roots.size() == 1){
        root = roots.at(0);
        roots.clear();
        return true;
    } else{
        roots.clear();
        return false;
    }
}
bool Scene::checkDoublePortAssign(Port* port){
    if (    port->link->first->stat != Status::DP && port->link->second->stat != Status::DP &&
            port->link->first->stat != Status::NDP && port->link->second->stat != Status::NDP   ){
        return true;
    }
    else
        return false;
}
bool Scene::checkPort(){
    QVector<Port*> ports;
    foreach(Port* p, root->Ports){
        if (p->link->first->stat == Status::Default ||
                p->link->second->stat == Status::Default){
            return false;
        }
    }
    foreach(Router* r, Routers){
        ports.clear();
        if (r != root){
            foreach (Port* p, r->Ports){
                if (p->stat == Status::Root){
                    ports.push_back(p);
                }
            }
            if (ports.size() != 1){
                return false;
            }
        }
    }
    return true;
}

void Scene::drawPorts(){
    foreach (QGraphicsLineItem * item, Ports){
        this->removeItem(item);
        delete item;
    }
    Ports.clear();
    QPen activePen = QPen(Qt::green, 2, Qt::SolidLine);
    QPen disabledPen = QPen(Qt::red, 2, Qt::SolidLine);
    QPen basicPen = QPen(Qt::blue, 2, Qt::SolidLine);
    QPen rootPen = QPen(Qt::magenta, 2, Qt::SolidLine);
    QGraphicsLineItem* tl = 0;
    QLineF tll;
    foreach(Bridge* b, Bridges){
        tl = new QGraphicsLineItem;
        switch (b->first->stat) {
        case Status::DP:
            tl->setPen(activePen);
            break;
        case Status::NDP:
            tl->setPen(disabledPen);
            break;
        case Status::Default:
            tl->setPen(basicPen);
            break;
        case Status::Root:
            tl->setPen(rootPen);
            break;
        }
        tll.setP1(QPointF(b->first->r->scenePos().x() + 29, b->first->r->scenePos().y() + 23));
        tll.setP2(QPointF(b->first->r->scenePos().x() + 29 + 0.5*(b->second->r->scenePos().x() + 29 - (b->first->r->scenePos().x() + 29)),
                          b->first->r->scenePos().y() + 23 + 0.5*(b->second->r->scenePos().y() + 29 - (b->first->r->scenePos().y() + 29))));
        tl->setLine(tll);
        this->addItem(tl);
        this->Ports.push_back(tl);
        tl = 0;

        tl = new QGraphicsLineItem;
        switch (b->second->stat) {
        case Status::DP:
            tl->setPen(activePen);
            break;
        case Status::NDP:
            tl->setPen(disabledPen);
            break;
        case Status::Default:
            tl->setPen(basicPen);
            break;
        case Status::Root:
            tl->setPen(rootPen);
            break;
        }
        tll.setP1(QPointF(b->second->r->scenePos().x() + 29, b->second->r->scenePos().y() + 23));
        tll.setP2(QPointF(b->second->r->scenePos().x() + 29 + 0.5*(b->first->r->scenePos().x() + 29 - (b->second->r->scenePos().x() + 29)),
                          b->second->r->scenePos().y() + 23 + 0.5*(b->first->r->scenePos().y() + 29 - (b->second->r->scenePos().y() + 29))));
        tl->setLine(tll);
        this->addItem(tl);
        this->Ports.push_back(tl);
        tl = 0;
    }

}
