#include "baseplat.h"

BasePlat::BasePlat(QString str)
{
    setPixmap(QPixmap(str).scaled(QPixmap(str).width()*1.3, QPixmap(str).height()*1.2));
}
