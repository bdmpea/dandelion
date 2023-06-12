#ifndef LETTER_H
#define LETTER_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>


static int X_FIELD = 22;
static int Y_FIELD = 25;
static int WIDTH_FIELD = 513;
static int HEIGHT_FIELD = 458;
static int WIDTH_CELL = 51;
static int HEIGHT_CELL = 46;


class letter : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    bool changed = false;

    explicit letter(QObject *parent = nullptr);
    ~letter();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

signals:
  void itemMoved(letter *);

private:
    QRectF boundingRect() const;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    int index;
};

#endif // LETTER_H
