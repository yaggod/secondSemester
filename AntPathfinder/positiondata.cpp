#include "positiondata.h"

PositionData::PositionData()
{
    _shortestKnownDistance = 0;
    _isNull = true;
    _isHandled = false;
}

PositionData::PositionData(int distance)
{
    _shortestKnownDistance = distance;
    _isNull = false;
    _isHandled = false;
}

bool PositionData::isHandled() const
{
    return _isHandled;
}

void PositionData::setIsHandled(bool newIsHandled)
{
    _isHandled = newIsHandled;
}

bool PositionData::shouldBeHandled()
{
    return !isNull() && !isHandled();
}

bool PositionData::isNull() const
{
    return _isNull;
}

int PositionData::shortestKnownDistance() const
{
    return _shortestKnownDistance;
}
