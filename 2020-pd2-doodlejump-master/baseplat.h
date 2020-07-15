#ifndef BASEPLAT_H
#define BASEPLAT_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
class BasePlat : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit BasePlat(QString);
signals:

public slots:
    virtual void special() = 0;
};

#endif // BASEPLAT_H
