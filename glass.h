#ifndef GLASS_H
#define GLASS_H

#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QVector>
#include "figure.h"

#define emptyCell QColor(150,150,150)

class Glass : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(unsigned int rows READ rows WRITE setRows)
    Q_PROPERTY(unsigned int columns READ columns WRITE setColumns)

private:
    uint m_rows;
    uint m_columns;
    static const uint W = 20;
    uint timerInterval;
    int idTimer;
    bool gameOn;
    uint score;
    QVector<QVector<QColor>> glassArray = QVector<QVector<QColor>>(0, QVector<QColor>(0));
    Figure *cur;
    Figure *next;

    void clearGlass();
    bool stopFall(uint i, uint j);
    void acceptColors(uint i, uint j);
    void checkGlass();
    bool checkH();
    void deleteH(uint i, uint j, uint f);
    bool checkV();
    void deleteV(uint i, uint j, uint f);

public:
    explicit Glass(QWidget *parent = nullptr);
    ~Glass();
    uint rows() const { return m_rows; }
    uint columns() const { return m_columns; }


protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void timerEvent(QTimerEvent *event) override;

signals:
    void signalGlassInit();
    void setScore(int);
    void signalNext(Figure *next);

public slots:
    void setRows(uint rows) { m_rows = rows; }
    void setColumns(uint columns) { m_columns = columns; }
    void slotGlassInit();
    void slotNewGame();
};

#endif // GLASS_H
