#include "blueplat.h"

BluePlat::BluePlat():BasePlat (":/res/p-blue.png")
{
    mx = 1;
}
void BluePlat::special()
{
    if(x()+pixmap().width()>=500)
        mx = -1;
    else if(x()<=0)
        mx = 1;
    setX(x()+mx);
}

