#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPoint>
#include <QString>
#include <cmath>
#define M_PI 3.14

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

bool MainWindow::IsFlashlighSet()
{
    return !(_flashlightStart.isNull() || _flashlightEnd.isNull());
}

float MainWindow::GetFlashlightRangeRadians()
{
    return M_PI * _flashlightRangeAngle / 180;
}

QPoint MainWindow::GetFlashlightDirection()
{
    return _flashlightEnd - _flashlightStart;
}

QPoint MainWindow::GetNormalizedFlashlightDirection()
{
    QPoint flashlightDirection = GetFlashlightDirection();

    return flashlightDirection / getVectorLength(flashlightDirection);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->modifiers() & Qt::ControlModifier)
    {
        if(_flashlightStart.isNull())
            _flashlightStart = event->pos();
        else
            _flashlightEnd = event->pos();
    }
    else if(IsFlashlighSet())
    {
        handlePoint(event->pos());
    }
    repaint();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    if(!IsFlashlighSet())
        return;
    QPainter painter(this);

    painter.drawLine(_flashlightStart, _flashlightEnd);

    QPoint flashlightDirection = GetFlashlightDirection();
    float defaultAngle = getAngleBetweenVectorAndXAxis(flashlightDirection);
    float angleAddition = GetFlashlightRangeRadians()/2;

    QPoint firstRay(100 * cos(defaultAngle + angleAddition), 100 * sin(defaultAngle + angleAddition));
    QPoint secondRay(100 * cos(defaultAngle - angleAddition), 100 * sin(defaultAngle - angleAddition));

    painter.drawLine(_flashlightEnd, _flashlightEnd + firstRay);
    painter.drawLine(_flashlightEnd, _flashlightEnd + secondRay);

}

void MainWindow::handlePoint(QPoint point)
{
    qDebug("\nNew point info");
    QPoint flashlightDirection= GetFlashlightDirection();
    QPoint pointDirection = point - _flashlightEnd;

    float flashlightAngle = getAngleBetweenVectorAndXAxis(flashlightDirection);
    float pointAngle = getAngleBetweenVectorAndXAxis(pointDirection);

    float localPointAngle = pointAngle - flashlightAngle;

    QString result("Your click was %1 and on the %2 side.%3");



    if(cos(localPointAngle) < 0)
        result = result.arg("behind the view");
    else
        result = result.arg("in front of view");
    if(sin(localPointAngle) < 0)
        result = result.arg("left");
    else
        result = result.arg("right");

    if(cos(localPointAngle) > cos(GetFlashlightRangeRadians()/2))
        result = result.arg("Also, your point was in view of flashlight");
    else
        result = result.arg("");

    QMessageBox msgBox;
    msgBox.information(this, "Your data", result);
}

float MainWindow::getCosinusBetweenVectors(QPoint first, QPoint second)
{
    float production = (float) QPoint::dotProduct(first, second);
    float cosinus = production /  (getVectorLength(first) * getVectorLength(second));

    return cosinus;
}

float MainWindow::getAngleBetweenVectorAndXAxis(QPoint vector)
{
    float result = atan2(vector.y(), vector.x());

    return result;
}

float MainWindow::getVectorLength(QPoint point)
{
    int x = point.x();
    int y = point.y();

    return sqrt(x*x + y*y);
}


