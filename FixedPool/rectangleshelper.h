#ifndef RECTANGLESHELPER_H
#define RECTANGLESHELPER_H

#include <QRect>



class RectanglesHelper
{
public:
    static int GetRectangleArea(QRect rectangle);
    static QRect GetResizedRectangle(QRect original, float multiplier);
    static float GetAreasRatio(QRect firstRectangle, QRect secondRectangle);
};

#endif // RECTANGLESHELPER_H
