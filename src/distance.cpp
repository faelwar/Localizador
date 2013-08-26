#include "distance.h"
#include "pointprocessed.h"

Distance::Distance()
{
}

Distance::Distance(PointProcessed* point, double distance){
    p = point;
    d = distance;
}

double Distance::getDistance(){
    return d;
}

PointProcessed* Distance::getPoint()const {
    return p;
}

bool Distance::operator<(Distance dis)const{
    return d < dis.d;
}

bool Distance::operator==(Distance dis)const{
    return d == dis.d;
}

bool Distance::operator!=(Distance dis)const{
    return d != dis.d;
}

bool Distance::operator<=(Distance dis)const{
    return d <= dis.d;
}

bool Distance::operator>(Distance dis)const{
    return d > dis.d;
}

bool Distance::operator>=(Distance dis)const{
    return d >= dis.d;
}
