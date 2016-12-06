#include "wavewidget.h"
#include "ui_wavewidget.h"

WaveWidget::WaveWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WaveWidget)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    color = Qt::red;
}

WaveWidget::~WaveWidget()
{
    delete ui;
    delete scene;
}

void WaveWidget::displayBuffer(vector<int> &v, double xratio, double yratio, int highlightStart, int highlightEnd)
{
    scene->clear();

    buffer = v;

    this->xratio = xratio;
    this->yratio = yratio;

    this->highlightStart = highlightStart;
    this->highlightEnd = highlightEnd;

    ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QVector<QPointF> points;

    double density = 2;

    bool line = false;
    bool fill = true;

    // [YZ] when xratio is big, lower the density
    if (xratio > 0.5) {
        density = 0.5 / xratio;
        line = true;
        fill = false;
    }

    for (int i = 0; i < (int) (density * xratio * v.size()); i++)
        points.append(QPointF(i / (double) density, v[(int)((double)i / (density * xratio))] * yratio));

    double start = xratio * highlightStart;
    double end = xratio * highlightEnd;

    for (int i = 0; i < points.size() - 1; i++)
    {
        if (line)
        {
            scene->addLine(points[i].x(), points[i].y(), points[i+1].x(), points[i+1].y());
        }
        if (fill)
        {
            QGraphicsRectItem *item = new QGraphicsRectItem(points[i].x(), 0, 1.0 / (double) density, points[i].y());
            item->setPen(Qt::NoPen);
            if (start <= points[i].x() && points[i].x() <= end)
                item->setBrush(QBrush(color.darker()));
            else
                item->setBrush(QBrush(color));
            scene->addItem(item);
        }
    }

    // [YZ] force update
    ui->graphicsView->viewport()->update();
}

void WaveWidget::displayBuffer(vector<int> &v, double xratio, double yratio)
{
    displayBuffer(v, xratio, yratio, 0, 0);
    /*
    scene->clear();

    QVector<QPointF> points;

    int density = 2;

    for(int i = 0; i < (int) density * xratio * v.size(); i++)
        points.append(QPointF(i / (double) density, v[i / (density * xratio)] * yratio));

    bool line = false;
    bool fill = true;

    for(int i = 0; i < points.size() - 1; i++)
    {
        if (line)
            scene->addLine(points[i].x(), points[i].y(), points[i+1].x(), points[i+1].y());
        if (fill)
        {
            QGraphicsRectItem *item = new QGraphicsRectItem(points[i].x(), 0, 1.0 / (double) density, points[i].y());
            item->setPen(Qt::NoPen);
            item->setBrush(QBrush(Qt::red));
            scene->addItem(item);
        }
    }

    // [YZ] force update
    ui->graphicsView->viewport()->update();
    */
}

void WaveWidget::clear()
{
    scene->clear();
    ui->graphicsView->viewport()->update();
}

void WaveWidget::displayFunction(double (*function) (double))
{
    scene->clear();

    QVector<QPointF> points;

    int numT = 2;
    int xratio = 50;
    int yratio = 50;
    bool line = false;
    bool fill = true;

    int density = 5;

    // Fill in points with n number of points
    for(int i = 0; i < 6.28 * xratio * density; i++)
        points.append(QPointF(i/(double) density, yratio * function(numT * i/(double)(xratio * density))));

    for(int i = 0; i < points.size() - 1; i++)
    {
        if (line)
            scene->addLine(points[i].x(), points[i].y(), points[i+1].x(), points[i+1].y());
        if (fill)
        {
            QGraphicsRectItem *item = new QGraphicsRectItem(points[i].x(), 0, 1.0/(double)density, -points[i].y());
            item->setPen(Qt::NoPen);
            item->setBrush(QBrush(Qt::red));
            scene->addItem(item);
        }
    }

    // [YZ] force update
    ui->graphicsView->viewport()->update();
}

void WaveWidget::setColor(QColor color)
{
    this->color = color;
    displayBuffer(buffer, xratio, yratio, highlightStart, highlightEnd);
}

QGraphicsView *WaveWidget::getQGraphicsView()
{
    return ui->graphicsView;
}

QGraphicsScene *WaveWidget::getQGraphicsScene()
{
    return scene;
}
