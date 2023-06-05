#include "circle.h"

#include <qline.h>
#include <qrect.h>

Circle::Circle()
{

}

Circle::Circle(QPoint center, int radius)
{
    this->_center = center;
    this->_radius = radius;
}

float Circle::DistanceTo(Circle another)
{
    return DistanceTo(another.GetCenter());
}

float Circle::DistanceTo(QPoint point)
{
    return QLineF(_center, point).length();
}

bool Circle::Intersects(Circle another)
{
    return DistanceTo(another) < (_radius + another.GetRadius());
}

bool Circle::Intersects(QRect another)
{
    int minX = another.topLeft().x();
    int maxX = another.bottomRight().x();
    int minY = another.topLeft().y();
    int maxY = another.bottomRight().y();
    for(int x = minX; x <= maxX; x++)
        for(int y = minY; y <= maxY; y++)
        {
            if(Contains(QPoint(x, y)))
                return true;
        }

    return false;
}

bool Circle::Contains(QPoint point)
{
    return DistanceTo(point) < _radius;
}

bool Circle::Contains(QRect rectangle)
{
    return Contains(rectangle.topLeft()) &&
            Contains(rectangle.topRight()) &&
            Contains(rectangle.bottomLeft()) &&
            Contains(rectangle.bottomRight());
}
