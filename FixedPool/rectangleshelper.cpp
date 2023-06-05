#include "rectangleshelper.h"
#include <QRect>

int RectanglesHelper::GetRectangleArea(QRect rectangle)
{
    return rectangle.width() * rectangle.height();
}

QRect RectanglesHelper::GetResizedRectangle(QRect original, float multiplier)
{
    QRect rectangleToReturn;
    rectangleToReturn.setSize(original.size() * multiplier);
    return rectangleToReturn;
}

float RectanglesHelper::GetAreasRatio(QRect firstRectangle, QRect secondRectangle)
{
    return ((float)GetRectangleArea(firstRectangle)) / GetRectangleArea(secondRectangle);
}
