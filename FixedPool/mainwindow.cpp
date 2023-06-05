#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <rectangleshelper.h>

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
    if(event->button() == Qt::LeftButton && !_isPoolSetup)
    {
            _pool.setTopLeft(event->pos());
    }
    else if(_isPoolSetup && !_pool.contains(event->pos()))
    {
        if(event->modifiers() & Qt::ControlModifier)
            addCirclePot(event->pos());
        else
            addRectanglePot(event->pos());
    }

    repaint();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && !_isPoolSetup)
    {
        _isPoolSetup = true;
        _pool.setBottomRight(event->pos());
    }
    repaint();
}

void MainWindow::addRectanglePot(QPoint position)
{
    QRect rectangleToAdd = RectanglesHelper::GetResizedRectangle(_pool, _rectanglePoolModifier);
    rectangleToAdd.moveCenter(position);
    if(isValidRectangle(rectangleToAdd))
        _rectanglePots.append(rectangleToAdd);
}

void MainWindow::addCirclePot(QPoint position)
{
    Circle circleToAdd = Circle(position, _potsRadius);
    if(isValidCircle(circleToAdd))
        _circlePots.append(circleToAdd);
}

bool MainWindow::isValidRectangle(QRect rectangle)
{
    for(int i = 0; i < _rectanglePots.size(); i++)
        if(!isValidRectanglesPair(_rectanglePots[i], rectangle))
            return false;
    for(int i = 0; i < _circlePots.size(); i++)
        if(!isValidCircleRectanglePair(_circlePots[i], rectangle))
            return false;
    return true;
}

bool MainWindow::isValidCircle(Circle circle)
{
    for(int i = 0; i < _circlePots.size(); i++)
        if(circle.Intersects(_circlePots[i]))
            return false;
    for(int i = 0; i < _rectanglePots.size();i ++)
        if(!isValidCircleRectanglePair(circle, _rectanglePots[i]))
            return false;
    return true;
}

bool MainWindow::isValidRectanglesPair(QRect firstRectangle, QRect secondRectangle)
{
    float ratio = RectanglesHelper::GetAreasRatio(firstRectangle.intersected(secondRectangle), firstRectangle);
    return (ratio > 0.6) || (ratio == 0);

}

bool MainWindow::isValidCircleRectanglePair(Circle circle, QRect rectangle)
{
    return circle.Contains(rectangle) || !circle.Intersects(rectangle);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    drawGrass();
    if(_isPoolSetup)
        drawPool();
    drawPots();
}

void MainWindow::drawGrass()
{
    QPainter painter(this);

    QBrush brush(QImage(":/landshaft/grass.png"));
    painter.setBrush(brush);

    painter.drawRect(contentsRect());
}

void MainWindow::drawPool()
{
    QPainter painter(this);

    QBrush brush(QImage(":/landshaft/water.png"));
    painter.setBrush(brush);

    painter.drawRect(_pool);
}

void MainWindow::drawPots()
{
    QPainter painter(this);

    QBrush brush(QImage(":/landshaft/flower.png"));
    painter.setBrush(brush);

    for(int i = 0; i < _circlePots.size(); i++)
        painter.drawEllipse(_circlePots[i].GetCenter(), _circlePots[i].GetRadius(), _circlePots[i].GetRadius());
    painter.drawRects(_rectanglePots);
}
