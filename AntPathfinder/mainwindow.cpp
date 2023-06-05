#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QBrush>
#include <QLineF>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QVector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    isPathCalculated = false;
    if(event->modifiers() & Qt::ControlModifier)
    {
        if(event->button() == Qt::LeftButton)
            _treeCenter = event->pos();
        else
            _treeRadius = QLineF(event->pos(), _treeCenter).length();
    }
    else
    {
        if(event->button() == Qt::LeftButton)
            _antPosition = adjustPosition(event->pos());
        else
            _antDestination = adjustPosition(event->pos());
    }
    repaint();
}

QPoint MainWindow::adjustPosition(QPoint point)
{
    int x = (point.x() + _gridSize / 2) / _gridSize * _gridSize;
    int y = (point.y() + _gridSize / 2) / _gridSize * _gridSize;
    return QPoint(x , y);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    drawGrid();

    QPainter painter(this);
    if(!_treeCenter.isNull())
        painter.drawEllipse(_treeCenter, _treeRadius, _treeRadius);

    QPen bluePen(Qt::blue), redPen(Qt::red);
    bluePen.setWidth(15);
    redPen.setWidth(5);

    painter.setPen(redPen);
    painter.drawPolyline(getOptimalPath().data(), getOptimalPath().size());

    painter.setPen(bluePen);
    painter.drawPoint(_antPosition);
    painter.drawPoint(_antDestination);
}

void MainWindow::drawGrid()
{
    QPainter painter(this);

    for(int x = 0; x <= contentsRect().width(); x += _gridSize)
    {
        painter.drawLine(QPoint(x, 0), QPoint(x, contentsRect().height()));
    }

    for(int y = 0; y < contentsRect().height(); y += _gridSize)
    {
        painter.drawLine(QPoint(0, y), QPoint(contentsRect().width(), y));
    }

}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    isPathCalculated = false;
}

QVector<QPoint> MainWindow::getOptimalPath()
{
    if(!isPathCalculated)
        recalculatePath();
    isPathCalculated = true;
    return _calculatedPath;

}

void MainWindow::recalculatePath()
{
    initializePositionsMatrix();
    recalculateWeights();
    restorePath();
}

bool MainWindow::isWeightOnIndexesEquals(QPoint indexes, int expectedWeight)
{
    if(isValidIndexesPair(indexes))
            if(getPositionDataOnIndexes(indexes).shortestKnownDistance() == expectedWeight)
                return true;

    return false;
}

void MainWindow::recalculateWeights()
{
    int xSize = _positionsMatrix[0].size();
    int ySize = _positionsMatrix.size();
    int maxPossibleDistance = (xSize + ySize) * 2;

    QPoint directions[4] =
    {
        QPoint(0, 1),
        QPoint(0, -1),
        QPoint(1, 0),
        QPoint(-1, 0)
    };

    for(int i = 0; i < maxPossibleDistance; i++)
        for(int y = 0; y < ySize; y++)
            for(int x = 0; x < xSize; x++)
            {
                PositionData currentData = _positionsMatrix[y][x];
                if(currentData.shouldBeHandled())
                {
                    for(int i = 0; i < 4; i++)
                    {
                        trySettingPositionData(QPoint(x, y) + directions[i], PositionData(currentData.shortestKnownDistance() + 1));
                    }
                }
            }
}

void MainWindow::initializePositionsMatrix()
{
    int yPositionsCount = contentsRect().height() / _gridSize + 1;
    int xPositionsCount = contentsRect().width() / _gridSize + 1;
    _positionsMatrix = QVector<QVector<PositionData>>(yPositionsCount);
    for(int i = 0; i < yPositionsCount; i++)
    {
        _positionsMatrix[i] = QVector<PositionData>(xPositionsCount);
    }
    QPoint indexes = getIndexesFromPoint(_antPosition);
    trySettingPositionData(indexes, PositionData(0));
}

void MainWindow::restorePath()
{
    QPoint lastPositionIndexes = getIndexesFromPoint(_antDestination);
    PositionData destinationData = getPositionDataOnIndexes(lastPositionIndexes);
    _calculatedPath = QVector<QPoint>();
    QPoint directions[4] =
    {
        QPoint(0, 1),
        QPoint(0, -1),
        QPoint(1, 0),
        QPoint(-1, 0)
    };

    if(destinationData.isNull())
        return;

    int shortestDistance = destinationData.shortestKnownDistance();

    _calculatedPath.append(getPointFromIndexes(lastPositionIndexes));
    for(int expectedDistance = shortestDistance - 1; expectedDistance > 0; expectedDistance--)
    {
        for(int i = 0; i < 4; i++)
            if(isWeightOnIndexesEquals(lastPositionIndexes + directions[i], expectedDistance))
            {
                lastPositionIndexes = lastPositionIndexes + directions[i];
                _calculatedPath.append(getPointFromIndexes(lastPositionIndexes));
                break;
            }
    }
    _calculatedPath.append(_antPosition);
}

void MainWindow::trySettingPositionData(QPoint indexes, PositionData data)
{
    if(isValidIndexesPair(indexes) && isNotInsideTheTreeIndexes(indexes))
        if(getPositionDataOnIndexes(indexes).isNull() || getPositionDataOnIndexes(indexes).shortestKnownDistance() > data.shortestKnownDistance())
            setPositionDataOnIndexes(indexes, data);
}

bool MainWindow::isNotInsideTheTreeIndexes(QPoint indexes)
{
    return QLineF(getPointFromIndexes(indexes), _treeCenter).length() > _treeRadius;
}

bool MainWindow::isValidIndexesPair(QPoint indexes)
{
    int x = indexes.x();
    int y = indexes.y();
    if(x >= 0 && x < _positionsMatrix[0].size())
        if(y >= 0 && y < _positionsMatrix.size())
            return true;
    return false;
}

QPoint MainWindow::getIndexesFromPoint(QPoint point)
{
    return QPoint(point.x() / _gridSize, point.y() / _gridSize);
}

QPoint MainWindow::getPointFromIndexes(int x, int y)
{
    return QPoint(x * _gridSize, y * _gridSize);
}

QPoint MainWindow::getPointFromIndexes(QPoint indexes)
{
    return getPointFromIndexes(indexes.x(), indexes.y());
}

void MainWindow::setPositionDataOnIndexes(QPoint indexes, PositionData dataToSet)
{
    _positionsMatrix[indexes.y()][indexes.x()] = dataToSet;
}

PositionData MainWindow::getPositionDataOnIndexes(QPoint indexes)
{
    return _positionsMatrix[indexes.y()][indexes.x()];
}

