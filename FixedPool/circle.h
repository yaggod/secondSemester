#ifndef CIRCLE_H
#define CIRCLE_H

#include <QPoint>
#include <qrect.h>

class Circle
{

public:

    Circle();
    Circle(QPoint center, int radius);

    float DistanceTo(Circle another);
    float DistanceTo(QPoint point);
    bool Intersects(Circle another);
    bool Intersects(QRect another);
    bool Contains(QPoint point);
    bool Contains(QRect rectangle);

    int GetRadius()
    {
        return _radius;
    }
    QPoint GetCenter()
    {
        return _center;
    }
    void SetRadius(int radius)
    {
        if(radius > 0)
            _radius = radius;
    }
    void SetCenter(QPoint center)
    {
        _center = center;
    }

private:
    int _radius;
    QPoint _center;
};

#endif // CIRCLE_H
