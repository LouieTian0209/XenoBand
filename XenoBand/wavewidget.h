#ifndef WAVEWIDGET_H
#define WAVEWIDGET_H

#include "core.h"

namespace Ui {
class WaveWidget;
}

class WaveWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WaveWidget(QWidget *parent = 0);
    ~WaveWidget();

    void displayBuffer(vector<int> &v, double xratio, double yratio);
    void displayBuffer(vector<int> &v, double xratio, double yratio, int highlightStart, int highlightEnd);

    void displayFunction(double (*function) (double));

    void clear();

    void setColor(QColor color);

    QGraphicsView *getQGraphicsView();
    QGraphicsScene *getQGraphicsScene();

    double xratio;
    double yratio;

    inline void zoom(double xmultiple, double ymultiple) { displayBuffer(buffer, xratio * xmultiple, yratio * ymultiple); }

    vector<int> buffer;

private:
    Ui::WaveWidget *ui;

    QColor color;

    int highlightStart;
    int highlightEnd;

    QGraphicsScene *scene;
};

#endif // WAVEWIDGET_H
