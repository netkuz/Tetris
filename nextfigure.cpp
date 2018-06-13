#include "nextfigure.h"

NextFigure::NextFigure(QWidget *parent) : QWidget(parent)
{
    next = nullptr;
}

void NextFigure::slotPaintEvent(Figure *n)
{
    next = n;
    repaint();
}

void NextFigure::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    if (next != nullptr) {
            QPainter painter(this);
            painter.translate(QPoint(50,0));
            next->paintFigure(painter);
    }
}
