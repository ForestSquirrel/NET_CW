#include "bridge.h"

Bridge::Bridge(QGraphicsLineItem *parent, Port *f, Port *s, unsigned int GlobalBridgeId, unsigned int pc/*, unsigned int p*/) : QGraphicsLineItem(parent) {
    stat = Status::Default;
    first = f;
    second = s;
    Id = GlobalBridgeId;
    PathCost = pc;
   // priority = p;
    d = new BridgeDialog;
}
int Bridge::type() const{
    return Type;
}
void Bridge::editEntry(){
    d->setModal(true);
    d->setFieldText(this->Id, /*this->priority, */this->PathCost);
    d->show();
    if (d->exec() == QDialog::Accepted){
      this->Id = d->Id();
      /*this->priority= d->priority();*/
      this->PathCost = d->PathCost();
    } else
        return;

}
void Bridge::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e){
    editEntry();
    Q_UNUSED(e);
}

void Bridge::updColor(){
    switch (stat) {
    case Status::DP:
        this->setPen(basicPen);
        break;
    case Status::NDP:
        this->setPen(basicPen);
        break;
    case Status::Default:
        this->setPen(basicPen);
        break;
    case Status::Root:
        this->setPen(basicPen);
        break;
    }
    this->update();
}
