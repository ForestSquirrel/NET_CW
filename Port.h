#ifndef PORT_H
#define PORT_H

#include "bridge.h"
#include "Enums.h"
#include "Router.h"

class Bridge;
class Router;

class Port {
public:
    Bridge * link;
    Status stat;
    Router * r;
    Port(){
        link = nullptr;
        stat = Status::Default;
        r = nullptr;
    }
    Port(Bridge *b, Status s, Router* r){
        this->link = b;
        this->stat = s;
        this->r = r;
    }

    void changeStatus(Status s){
        stat = s;
    }
};

#endif // PORT_H
