#ifndef PACKET_H
#define PACKET_H

#include <QString>

class Packet{
public:
    size_t SourceId;
    size_t DestinationId;
    QString data;

    Packet(size_t src, size_t dest, QString d){
        SourceId = src;
        DestinationId = dest;
        data = d;
    }
};

#endif // PACKET_H
