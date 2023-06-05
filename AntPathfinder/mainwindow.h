#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <PositionData.h>
#include <QMainWindow>

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
    QPoint adjustPosition(QPoint point);
    void paintEvent(QPaintEvent *event);
    void drawGrid();
    void resizeEvent(QResizeEvent *event);
    QVector<QPoint> getOptimalPath();
    void recalculatePath();
    bool isWeightOnIndexesEquals(QPoint indexes, int expectedWeight);
    void recalculateWeights();
    void initializePositionsMatrix();
    void restorePath();
    void trySettingPositionData(QPoint indexes, PositionData data);
    bool isNotInsideTheTreeIndexes(QPoint indexes);
    bool isValidIndexesPair(QPoint indexes);
    QPoint getIndexesFromPoint(QPoint point);
    QPoint getPointFromIndexes(int x, int y);
    QPoint getPointFromIndexes(QPoint indexes);

    QVector<QVector<PositionData>> _positionsMatrix;
    void setPositionDataOnIndexes(QPoint indexes, PositionData dataToSet);
    PositionData getPositionDataOnIndexes(QPoint indexes);

    QVector<QPoint> _calculatedPath;
    bool isPathCalculated = false;

    int _gridSize = 32;
    QPoint _antPosition = QPoint(320, 160);
    QPoint _antDestination;
    QPoint _treeCenter;
    int _treeRadius = 0;
};
#endif // MAINWINDOW_H
