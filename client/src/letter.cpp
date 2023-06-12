#include "letter.h"
#include "qwidget.h"
#include <random>
#include <QDateTime>

QList<QString> alphabet = {"A", "A", "A", "A", "A", "A", "A", "A", "A", "B", "B", "C", "C",
                           "D", "D", "D", "D", "E", "E", "E", "E", "E", "E", "E", "E", "E",
                           "E", "E", "E", "F", "F", "G", "G", "G", "H", "H", "I", "I", "I",
                           "I", "I", "I", "I", "I", "I", "J", "K", "L", "L", "L", "L", "M",
                           "M", "N", "N", "N", "N", "N", "N", "O", "O", "O", "O", "O", "O",
                           "O", "O", "P", "P", "Q", "R", "R", "R", "R", "R", "R", "S", "S",
                           "S", "S", "T", "T", "T", "T", "T", "T", "U", "U", "U", "U", "V",
                           "V", "W", "W", "X", "Y", "Y", "Z"};

letter::letter(QObject *parent) :
    QObject(parent)
{
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, 97);


    index = distribution(generator);
}

letter::~letter()
{
}

QRectF letter::boundingRect() const{
    return QRectF(-10,-10,30,30);
}

void letter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    QFont font = painter->font() ;
    font.setPointSize(font.pointSize() * 1.6);
    painter->setFont(font);

    painter->setPen(Qt::black);
    painter->setBrush(QBrush(qRgb(194,228,255)));

    painter->drawRect(-10,-10,30,30);
    painter->drawText(boundingRect(), Qt::AlignCenter | Qt::AlignTop, alphabet[index]);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void letter::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if (!changed){
        this->setPos(mapToScene(event->pos()));
    }
}


void letter::mousePressEvent(QGraphicsSceneMouseEvent *event){
    this->setCursor(QCursor(Qt::ClosedHandCursor));
    Q_UNUSED(event);
}

int calculate_x(int x){
    return (x/WIDTH_CELL+1) * WIDTH_CELL - 6;
}

int calculate_y(int y){
    return (y/HEIGHT_CELL+1) * HEIGHT_CELL - 2;
}


void letter::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    this->setCursor(QCursor(Qt::ArrowCursor));
    if (WIDTH_FIELD + X_FIELD >= this->x() && HEIGHT_FIELD + Y_FIELD >= this->y()){
       this->setPos(calculate_x(this->x()), calculate_y(this->y()));
       //change.append(this);
       changed = true;
       emit itemMoved(this);
       setFlag(QGraphicsItem::ItemIsMovable, false);
       setFlag(QGraphicsItem::ItemIsSelectable, false);

    }
    Q_UNUSED(event);
}



