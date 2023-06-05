#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Circle.h>
#include <QMainWindow>
#include <QMouseEvent>
#include <qevent.h>
#include <qevent.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void addRectanglePot(QPoint position);
    void addCirclePot(QPoint position);
    bool isValidRectangle(QRect rectangle);
    bool isValidCircle(Circle circle);
    bool isValidRectanglesPair(QRect firstRectangle, QRect secondRectangle);
    bool isValidCircleRectanglePair(Circle circle, QRect rectangle);

    void paintEvent(QPaintEvent *event);
    void drawGrass();
    void drawPool();
    void drawPots();

    QRect _pool;
    QVector<QRect> _rectanglePots;
    QVector<Circle> _circlePots;

    float _rectanglePoolModifier = 0.1f;
    float _rectanglePoolAllowedArea = 0.6f;
    int _potsRadius = 16;
    bool _isPoolSetup = false;

};
#endif // MAINWINDOW_H
