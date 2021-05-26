#ifndef BPDU_H
#define BPDU_H
#include <QString>
class BPDU{
public:
    unsigned int RootId;
    unsigned int RootPathCost;
    unsigned int SourceId;
    unsigned int DestinationId;
    unsigned int priority;
    bool f;

    BPDU(unsigned int r, unsigned int rpc, unsigned int src, unsigned int dest, unsigned int p, bool f){
        RootId = r;
        RootPathCost = rpc;
        SourceId = src;
        priority = p;
        DestinationId = dest;
        this->f = f;
    }
    BPDU(){
        RootId = 0;
        RootPathCost = 0;
        SourceId = 0;
        DestinationId = 0;
        priority = 0;
        f = false;
    }
    bool operator==(const BPDU p){
        if (    this->RootId == p.RootId &&
                this->RootPathCost == p.RootPathCost &&
                this->SourceId == p.SourceId &&
                this->DestinationId== p.DestinationId &&
                this->priority == p.priority &&
                this->f == p.f)
            return true;
        else
            return false;
    }
    void changeRoot(unsigned int r){
        RootId = r;
    }
    void changeRPC(unsigned int rpc){
        RootPathCost = rpc;
    }
    unsigned int addRPC(unsigned int rpc){
        RootPathCost += rpc;
        return RootPathCost;
    }
    void setDest(unsigned int dest){
        DestinationId = dest;
    }
    QString toString(){
        QString temp = NULL;
        temp += '\n';
        if (!f)
            temp += "#=====HELLO BPDU======#";
        else
            temp += "#====LEARNING BPDU====#";
        temp += '\n';
        temp += "Root Id is " + QString::number(RootId) + '\n';
        temp += "Priority is " + QString::number(priority) + '\n';
        temp += "Root Path Cost is " + QString::number(RootPathCost) + '\n';
        temp += "Source Id is " + QString::number(SourceId) + '\n';
        temp += "Destination Id is " + QString::number(DestinationId) + '\n';
        temp += "#====================#";
        temp += '\n';
        return temp;
    }
};

#endif // BPDU_H
