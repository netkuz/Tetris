#ifndef FIGURE_H
#define FIGURE_H
#include <QColor>
#include <QPainter>

class Figure
{
    private:
        uint m_i = 0;
        uint m_j = 0;
        uint m_W;

        QVector<QColor> colours;
        QColor getRandomColor();

    public:
        Figure(uint W);
        ~Figure();
        uint getI() const { return m_i; }
        uint getJ() const { return m_j; }
        QColor getColor(int i) const { return colours[i]; }
        void setI(uint i) { m_i = i; }
        void setJ(uint j) { m_j = j; }
        void rotateColors(bool rotation);
        void makeRandomColors();
        void paintFigure(QPainter &painter);
};

#endif // FIGURE_H
