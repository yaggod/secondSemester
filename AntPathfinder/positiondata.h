#ifndef POSITIONDATA_H
#define POSITIONDATA_H


class PositionData
{
public:
    PositionData();
    PositionData(int distance);

    bool isHandled() const;
    void setIsHandled(bool newIsHandled);
    bool shouldBeHandled();

    bool isNull() const;

    int shortestKnownDistance() const;

private:
    int _shortestKnownDistance;
    bool _isNull = true;
    bool _isHandled;
};

#endif // POSITIONDATA_H
