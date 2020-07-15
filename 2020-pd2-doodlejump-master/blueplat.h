#ifndef BLUEPLAT_H
#define BLUEPLAT_H
#include "baseplat.h"
class BluePlat:public BasePlat
{
    Q_OBJECT
public:
    BluePlat();
private:
    int mx;
public slots:
    void special();
};

#endif // BLUEPLAT_H
