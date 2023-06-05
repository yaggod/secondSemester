#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPoint>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    bool IsFlashlighSet();
    float GetFlashlightRangeRadians();
    QPoint GetFlashlightDirection();
    QPoint GetNormalizedFlashlightDirection();
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void handlePoint(QPoint point);

    QPoint _flashlightStart;
    QPoint _flashlightEnd;

    float _flashlightRangeAngle = 90;

    static float getCosinusBetweenVectors(QPoint first, QPoint second);
    static float getAngleBetweenVectorAndXAxis(QPoint vector);
    static float getVectorLength(QPoint point);



};
#endif // MAINWINDOW_H
