#include "figure.h"
#include <QDebug>
#include <QPainter>
#include <ctime>

Figure::Figure(uint W)
{
    srand(time(0));
    m_W = W;
    if (colours.length() == 0){
        colours.push_back(QColor(Qt::black));
        colours.push_back(QColor(Qt::blue));
        colours.push_back(QColor(Qt::cyan));
        colours.push_back(QColor(Qt::green));
        colours.push_back(QColor(Qt::magenta));
        colours.push_back(QColor(Qt::red));
        colours.push_back(QColor(Qt::yellow));
        colours.push_back(QColor(Qt::white));
    }
}

Figure::~Figure(){}

QColor Figure::getRandomColor()
{
    return colours[rand() % colours.length()];
}

void Figure::rotateColors(bool rotation)
{
    if (rotation) {
        QColor temp = colours[0];
        int i;
        for(i = 0; i < 2; i++)
            colours[i] = colours[i+1];

        colours[i] = temp;
        return;
    }

    QColor temp = colours[2];
    int i;
    for(i = 2; i > 0; i--)
        colours[i] = colours[i-1];

    colours[i] = temp;
}

void Figure::makeRandomColors()
{
    for (int i = 0; i < 3; i++)
        colours[i] = getRandomColor();
}

void Figure::paintFigure(QPainter &painter)
{
    for (int i = 0; i < 1; i++)
        for (int j = 0; j < 3; j++) {
            painter.setPen(QPen("#000000"));
            painter.setBrush(QBrush(getColor(j)));
            painter.drawRect((m_i + i) * m_W, (m_j + j) * m_W, m_W, m_W);
        }
}
