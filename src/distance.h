#ifndef DISTANCE_H
#define DISTANCE_H

#include "pointprocessed.h"

class Distance
{
public:
    Distance();
    Distance(PointProcessed* point, double distance);
    double getDistance();
    PointProcessed *getPoint()const;
    bool operator<(Distance dis)const;
    bool operator==(Distance dis)const;
    bool operator!=(Distance dis)const;
    bool operator<=(Distance dis)const;
    bool operator>(Distance dis)const;
    bool operator>=(Distance dis)const;
private:
    PointProcessed* p;
    double d;
};

#endif // DISTANCE_H
