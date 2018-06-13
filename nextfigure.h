#ifndef NEXTFIGURE_H
#define NEXTFIGURE_H

#include <QWidget>
#include "figure.h"

class NextFigure : public QWidget
{
    Q_OBJECT
private:
    Figure *next;
public:
    explicit NextFigure(QWidget *parent = nullptr);

public slots:
    void slotPaintEvent(Figure * next);

protected:
    virtual void paintEvent(QPaintEvent *event);
};

#endif // NEXTFIGURE_H
