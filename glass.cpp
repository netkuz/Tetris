#include "glass.h"
#include <QMessageBox>

Glass::Glass(QWidget *parent) : QWidget(parent)
{
    gameOn = false;
    idTimer = -1;
    timerInterval = 500;

    cur = new Figure(W);
    cur->makeRandomColors();
    next = new Figure(W);
    next->makeRandomColors();

    connect( this, SIGNAL(signalGlassInit()), this, SLOT(slotGlassInit()), Qt::QueuedConnection);
    emit signalGlassInit();
}

Glass::~Glass()
{
    delete cur;
    delete next;
}

void Glass::clearGlass()
{
    QVector<QColor> temp = QVector<QColor>(m_rows);
    temp.fill(QColor(emptyCell));
    glassArray.fill(temp);
    score = 0;
}

bool Glass::stopFall(uint i, uint j)
{
    if (j == (m_rows)) { return true; }
    if (glassArray[i][j] == QColor(emptyCell)) { return false; }
    return true;
}

void Glass::acceptColors(uint i, uint j)
{
    for (int n = 0; n < 3; n++)
        glassArray[i][j + n] = cur->getColor(n);

    checkGlass();

    if (glassArray[m_columns/2][2] != emptyCell)
    {
        gameOn = false;
        killTimer(idTimer);
        QMessageBox::information(this, QObject::tr("Game over"), QObject::tr("You lose"), QMessageBox::Cancel);
        return;
    }

    Figure *temp = cur;
    cur = next;
    next = temp;
    cur->setI(m_columns / 2);
    next->makeRandomColors();
    next->setI(0);
    next->setJ(0);

    emit signalNext(next);
    repaint();

}

void Glass::checkGlass()
{
    if (checkH() || checkV())
        checkGlass();
}

bool Glass::checkH()
{
    for (uint i = 0; i < m_rows; i++) {
         uint f = 0;
         for (uint j = 0; j < m_columns; j++) {
             if (glassArray[j][i] == QColor(emptyCell)) {
                 f = j;
             } else {
                 if (glassArray[f][i] == glassArray[j][i]) {
                     if ((j - f) > 1) {
                         deleteH(i, j, f);
                         return true;
                     }
                 } else f = j;
             }
         }
    }
    return false;
}

void Glass::deleteH(uint i, uint j, uint f)
{
    int count = j - f;
    for (; i > 0; i--)
        for (uint m = f; m <= j; m++)
            glassArray[m][i] = glassArray[m][i - 1];

    score += count;
    emit setScore(score);
}

bool Glass::checkV()
{
    uint f = 0;
    for (uint i = 0; i < m_columns; i++) {
         for (uint j = 0; j < m_rows; j++) {
             if (glassArray[i][j] == QColor(emptyCell))
                 f = j;
             else {
                 if (glassArray[i][j] != glassArray[i][f]) {
                     if ((j - f) >= 3) {
                         deleteV(i, j, f);
                         return true;
                     }
                     else f = j;
                 }
             }
         }
         if ((m_rows - f) >= 3)
         {
                deleteV(i, m_rows, f);
                return true;
         }
    }
    return false;
}

void Glass::deleteV(uint i, uint j, uint f)
{
    int count = j - f;
    for (int m = (j - 1); m >= count; m--)
        glassArray[i][m] = glassArray[i][m - count];
    for (int m = 0; m <= count; m++)
         glassArray[i][m] = QColor(emptyCell);

    score += count;
    emit setScore(score);
}


void Glass::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    for (uint i = 0; i < m_columns; i++)
         for (uint j = 0; j < m_rows; j++) {
             painter.setPen(QPen("#cccccc"));
             painter.setBrush(QBrush(glassArray[i][j]));
             painter.drawRect(i * W, j * W, W, W);
         }

    if(gameOn)
       cur->paintFigure(painter);
}

void Glass::keyPressEvent(QKeyEvent *event)
{
    if(gameOn) {
        uint x = cur->getI();

        switch(event->key())
        {
            case Qt::Key_Left:
                {
                    if (x == 0)
                        break;
                    if ((glassArray[x - 1][cur->getJ()] != QColor(emptyCell)) || glassArray[x - 1][cur->getJ() + 2] != QColor(emptyCell))
                        break;
                     x > 0 ? x-- : x; cur->setI(x);
                    break;
                }
            case Qt::Key_Right:
                {
                    if (x == m_columns-1)
                        break;
                    if ((glassArray[x + 1][cur->getJ()] != QColor(emptyCell)) || glassArray[x + 1][cur->getJ() + 2] != QColor(emptyCell))
                        break;
                    x < (m_columns-1) ? x++ : x; cur->setI(x);
                    break;
                }
            case Qt::Key_Up:
                cur->rotateColors(true);
                break;
            case Qt::Key_Down:
                cur->rotateColors(false);
                break;
            case Qt::Key_Space:
                {
                    uint row = 0;
                    for (uint j = 0; j < m_rows; j++)
                        if (glassArray[x][j] == emptyCell)
                            row = j;
                    row -= 2;
                    acceptColors(x, row);
                    break;
                }
            default:
                QWidget::keyPressEvent(event);
                break;
        }
    } else {
        QWidget::keyPressEvent(event);
    }

    setFocus();
    repaint();
}

void Glass::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    if (!stopFall(cur->getI(), (cur->getJ() + 3) > m_rows ? m_rows : (cur->getJ() + 3))) {
        uint y = cur->getJ();
        y < (m_rows - 3) ? y++ : y; cur->setJ(y);
        repaint();
    } else {
        acceptColors(cur->getI(), cur->getJ());
    }
}

void Glass::slotGlassInit()
{
    glassArray.resize(m_rows * m_columns);

    QVector<QColor> temp = QVector<QColor>(m_rows);
    temp.fill(QColor(emptyCell));
    glassArray.fill(temp, m_columns);

    QSize s(m_columns * W, m_rows * W);
    setFixedSize(s);
    clearGlass();
    score = 0;
    emit setScore(score);
}

void Glass::slotNewGame()
{
    gameOn = false;
    if (idTimer != -1)
        killTimer(idTimer);

    clearGlass();
    score = 0;
    emit setScore(score);

    cur->makeRandomColors();
    cur->setI(m_columns / 2);
    next->makeRandomColors();
    next->setI(0);
    next->setJ(0);
    emit signalNext(next);

    idTimer = startTimer(timerInterval);
    gameOn = true;

    setFocus();
    repaint();
}
