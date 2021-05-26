#include "Router.h"

Router::Router(QGraphicsRectItem *parent, unsigned int GlobalRouterId) : QGraphicsRectItem(parent) {
    QRectF temprect(0, 0, 49, 40);
    setRect(temprect);
    Id = GlobalRouterId;
    ServicePacket = BPDU();
    isRoot = true;
    d = new RouterDialog;

}
void Router::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    QRectF rect = boundingRect();
    if (isRoot){
        painter->setPen(this->outlinePen);
        painter->drawRect(rect);
    }
    if (isSelected()){
        painter->setPen(this->outlinePenSelected);
        painter->drawRect(rect);
    }

    painter->drawImage(boundingRect(), QImage(":/icons/router.png"));
    Q_UNUSED(option);
    Q_UNUSED(widget);

}
int Router::type() const{
    return Type;
}

bool Router::has(BPDU b){
    foreach(BPDU bpdu, this->serviceLog){
        if (bpdu == b)
            return true;
    }
    return false;
}
void Router::editEntry(){
    d->setModal(false);
    d->setFieldText(this->Id, this->Priority, this->serviceLog);
    d->show();
    if (d->exec() == QDialog::Accepted){
        this->Id = d->Id();
        this->Priority = d->p();
    } else
        return;

}
void Router::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e){
    editEntry();
    Q_UNUSED(e);
}
