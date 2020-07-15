#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsTextItem>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(500, 733);
    mainwidget = new QWidget;
    setCentralWidget(mainwidget);
    graphicsView = new QGraphicsView(mainwidget);
    graphicsView->setFocusPolicy(Qt::NoFocus);
    scene = new QGraphicsScene(0, 0, 500, 700);
    graphicsView->setScene(scene);
    backgroundMusic->setMedia(QUrl("qrc:/res/background.mp3"));
    List->addMedia(QUrl("qrc:/res/background.mp3"));
    List->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    backgroundMusic->play();
    qsrand(uint(QTime::currentTime().msec()));
    connect(this, SIGNAL(gamestart()), this, SLOT(game()));
    connect(this, SIGNAL(gamepause()), this, SLOT(pause()));
    connect(this, SIGNAL(gameresume()), this, SLOT(pause()));
    connect(this, SIGNAL(gamemenu()), this, SLOT(menu()));
    menu();
}
void MainWindow::menu()
{
    scene->clear();
    gamestate = MENU;
    player = new QGraphicsPixmapItem;
    scoreItem = new QGraphicsTextItem;
    finalscoreItem = new QGraphicsTextItem;
    pauseItem = new QGraphicsTextItem;
    resumebutton = new QGraphicsPixmapItem;
    finalscoreItem = new QGraphicsTextItem;
    plat = new BasePlat*[15];
    for(int i = 0; i < 15; i++)
    {
        plat[i] = nullptr;
    }
    back = new QGraphicsPixmapItem;
    back->setPixmap(QPixmap(":/res/menuback.jpg").scaled(500,733));
    back->setZValue(-1);
    scene->addItem(back);
    QGraphicsPixmapItem *playbutton = new QGraphicsPixmapItem(QPixmap(":/res/play.png").scaled(150, 60));
    playbutton->setPos(80, 200);
    scene->addItem(playbutton);
}
MainWindow::~MainWindow()
{
    delete ui;
    delete player;
    for(int i = 0; i < 15; i++)
    {
        delete plat[i];
    }
    delete [] plat;

    delete back;
    delete scoreItem;
    delete finalscoreItem;
    delete monster;
    delete rocket;
    delete cap;
    delete hazard;
    delete pauseItem;
    delete timer;
    delete scene;
    delete graphicsView;
    delete  mainwidget;

}
void MainWindow::game()
{
    scene->clear();
    lcount = 0;
    rcount = 0;
    upcount = 0;
    downcount = 0;
    lasty = 0;
    score = 0;
    iskey=false;
    gamestate = GAME;
    isshoot = false;
    ismonster = false;
    ishole = false;
    isrocket= false;
    iscap = false;
    equipecap = false;
    equiperocket = false;
    timer = new QTimer;
    timer->start(10);
    back = new QGraphicsPixmapItem;
    back->setPixmap(QPixmap(":/res/gameback.jpg").scaled(500,733));
    scene->addItem(back);
    QGraphicsPixmapItem *topbar = new QGraphicsPixmapItem;
    topbar->setPixmap(QPixmap(":/res/topbar.png").scaledToWidth(500));
    topbar->setZValue(5);
    scene->addItem(topbar);
    scoreItem->setPlainText(QString::number(score));
    scoreItem->setFont(QFont("Doodle Jump", 22, 75));
    scoreItem->setZValue(5);
    scoreItem->setPos(0,-4);
    scene->addItem(scoreItem);
    pauseItem->setPlainText("| |");
    pauseItem->setFont(QFont("Doodle Jump", 12, 75));
    pauseItem->setZValue(5);
    pauseItem->setPos(470, 5);
    scene->addItem(pauseItem);
    plat[0] = new GreenPlat;
    plat[0]->setPos(250 - plat[0]->pixmap().width()/2, 700 - plat[0]->pixmap().height());
    scene->addItem(static_cast<QGraphicsPixmapItem*>(plat[0]));
    int r;
    for(int i = 1; i < 15; i++)
    {
        r = qrand()%5 + 1;
        if(r == 1)
        {
            plat[i] = new BluePlat;
        }
        else {
            plat[i] = new GreenPlat;
        }
        connect(timer, SIGNAL(timeout()), plat[i], SLOT(special()));
        plat[i]->setPos(qrand()%(500-plat[i]->pixmap().width()), plat[i-1]->y()-qrand()%60-plat[i]->pixmap().height());
        scene->addItem(static_cast<QGraphicsPixmapItem*>(plat[i]));
    }
    player = new QGraphicsPixmapItem;
    player->setPixmap(QPixmap(":/res/doodle_right.png").scaled(60,60));
    player->setPos(plat[0]->x()+plat[0]->pixmap().width()/2-player->pixmap().width()/2, plat[0]->y()-player->pixmap().height());
    player->setZValue(3);
    connect(timer, SIGNAL(timeout()), this, SLOT(fall()));
    scene->addItem(player);

}
void MainWindow::pause()
{
    if(gamestate == GAME)
    {
        gamestate = PAUSE;
        back->setPixmap(QPixmap(":/res/pauseback.jpg").scaled(500,700));
        back->setZValue(4);
        resumebutton->setPixmap( QPixmap(":/res/resume.png").scaled(150,60));
        resumebutton->setPos(300, 600);
        resumebutton->setZValue(4);
        scene->addItem(resumebutton);
    }
    else if(gamestate == PAUSE)
    {
        gamestate = GAME;
        back->setPixmap(QPixmap(":/res/gameback.jpg").scaled(500,700));
        back->setZValue(-1);
        scene->removeItem(resumebutton);
    }
}

void MainWindow::fall()
{
    if(gamestate==GAME || gamestate == OVER)
    {
        if(gamestate==GAME)
        {
            if(upcount == 0)
            {
                downcount++;
                player->setY(player->y()+6*downcount*0.01);
                for(int i = 0; i < 15; i++)
                {
                    if((player->x()+player->pixmap().width()>plat[i]->x())&&(player->x()<(plat[i]->x()+plat[i]->pixmap().width()))&&
                            (player->y()+player->pixmap().height()>plat[i]->y())&&(player->y()+player->pixmap().height()<(plat[i]->y()+plat[i]->pixmap().height())))
                    {
                        if(plat[i]->y() < lasty)
                        {
                            lasty = plat[i]->y();
                            scoreup = true;
                        }
                        jumpSound->setMedia(QUrl("qrc:/res/jump.mp3"));
                        jumpSound->play();
                        connect(timer, SIGNAL(timeout()), this, SLOT(jump()));
                    }
                }
            }

            if(ismonster)
            {
                if((player->x()+player->pixmap().width()>monster->x())&&(player->x()<(monster->x()+monster->pixmap().width()))&&
                        (player->y()+player->pixmap().height()>monster->y())&&(player->y()<(monster->y()+monster->pixmap().height())))
                {
                    gamestate = OVER;
                }
            }
            if(ishole)
            {
                if((player->x()+player->pixmap().width()>hazard->x())&&(player->x()<(hazard->x()+hazard->pixmap().width()))&&
                        (player->y()+player->pixmap().height()>hazard->y())&&(player->y()<(hazard->y()+hazard->pixmap().height())))
                {
                    gamestate = OVER;
                }
            }
            if(iscap)
            {
                if((player->x()+player->pixmap().width()>cap->x())&&(player->x()<(cap->x()+cap->pixmap().width()))&&
                        (player->y()+player->pixmap().height()>cap->y())&&(player->y()<(cap->y()+cap->pixmap().height())) && !equipecap && !equiperocket && !isshoot)
                {
                    equipecap = true;
                    scoreup = true;
                    cap->setZValue(player->zValue()+0.1);
                    cap->setPixmap(QPixmap(":/res/cap1.png").scaled(50, 30));
                    if(dir == RIGHT)
                        cap->setPos(player->x()-5, player->y()+20-cap->pixmap().height());
                    else {
                        cap->setPos(player->x()+player->pixmap().width()-cap->pixmap().width()+5, player->y()+20-cap->pixmap().height());
                    }
                    upcount=0;
                    downcount = 0;
                    disconnect(timer, SIGNAL(timeout()), this, SLOT(jump()));
                    disconnect(timer, SIGNAL(timeout()), this, SLOT(fall()));
                    capSound->setMedia(QUrl("qrc:/res/cap.wav"));
                    capSound->play();
                    connect(timer, SIGNAL(timeout()), this, SLOT(capfly()));
                }
            }
            if(isrocket)
            {
                if((player->x()+player->pixmap().width()>rocket->x())&&(player->x()<(rocket->x()+rocket->pixmap().width()))&&
                        (player->y()+player->pixmap().height()>rocket->y())&&(player->y()<(rocket->y()+rocket->pixmap().height())) && !equipecap && !equiperocket && !isshoot)
                {
                    equiperocket = true;
                    scoreup = true;
                    rocket->setZValue(player->zValue()+0.1);
                    if(dir == RIGHT)
                    {
                        rocket->setPixmap(QPixmap(":/res/r_right.png").scaled(56, 100));
                        rocket->setPos(player->x()-rocket->pixmap().width()+17, player->y()-10);
                    }
                    else {
                        rocket->setPixmap(QPixmap(":/res/r_left.png").scaled(56, 100));
                        rocket->setPos(player->x()+player->pixmap().width()-17, player->y()-10);
                    }
                    upcount=0;
                    downcount = 0;
                    disconnect(timer, SIGNAL(timeout()), this, SLOT(jump()));
                    disconnect(timer, SIGNAL(timeout()), this, SLOT(fall()));
                    rocketSound->setMedia(QUrl("qrc:/res/rocket.wav"));
                    rocketSound->play();
                    connect(timer, SIGNAL(timeout()), this, SLOT(rocketfly()));
                }
            }
        }
        else if(gamestate == OVER)
        {
            downcount++;
            player->setY(player->y()+6*downcount*0.01);
        }
        if(player->x()<0-player->pixmap().width())
        {
            player->setX(500);
        }
        if(player->x()>500)
        {
            player->setX(0-player->pixmap().width());
        }
        if(player->y()>700)
        {
                gamestate = OVER;
                for(int i = 0; i < 15; i++)
                {
                    scene->removeItem(plat[i]);
                }
                if(ismonster)
                {
                    ismonster = false;
                    scene->removeItem(monster);
                }
                if(ishole)
                {
                    ishole = false;
                    scene->removeItem(hazard);
                }
                if(isrocket)
                {
                    isrocket = false;
                    scene->removeItem(rocket);
                }
                if(iscap)
                {
                    iscap = false;
                    scene->removeItem(cap);
                }
                player->setY(0);
                disconnect(timer, SIGNAL(timeout()), this, SLOT(fall()));
                disconnect(timer, SIGNAL(timeout()), this, SLOT(left()));
                disconnect(timer, SIGNAL(timeout()), this, SLOT(right()));
                timer->stop();
                scene->removeItem(player);
                back->setPixmap(QPixmap(":/res/gameoverback.png").scaled(500,700));
                finalscoreItem = new QGraphicsTextItem;
                finalscoreItem->setPlainText(QString::number(score));
                finalscoreItem->setFont(QFont("Doodle Jump", 30, 75));
                finalscoreItem->setPos(310, 440);
                scene->addItem(finalscoreItem);
        }
    }
}
void MainWindow::jump()
{
    if(gamestate == GAME)
    {
        downcount=0;
        upcount++;

        if((5.3-4.9*upcount*0.01)>0)
        {
            player->setY(player->y()-(5.3-4.9*upcount*0.01));
            if(scoreup)
            {
                score += (5.3-4.9*upcount*0.01);
                scoreItem->setPlainText(QString::number(score));
            }
        }
        if(player->y()<350)
        {
            player->setY(350);
            lasty += 5.3-4.9*upcount*0.01;
            if(lasty>700)
                lasty = 700;
            int dice = qrand()%1000;
            if(dice == 40)
            {
                if(!ismonster)
                {
                    ismonster = true;
                    monster = new QGraphicsPixmapItem;
                    monster->setPixmap(QPixmap(":/res/bat1.png"));
                    monster->setPos(qrand()%(400-monster->pixmap().width()), 0);
                    monster->setZValue(3);
                    scene->addItem(monster);
                }
            }
            else if(dice == 50)
            {
                if(!ishole)
                {
                    ishole = true;
                    hazard = new QGraphicsPixmapItem;
                    hazard->setPixmap(QPixmap(":/res/hole.png").scaled(QPixmap(":/res/hole.png").size()*0.6));
                    hazard->setPos(qrand()%(400-hazard->pixmap().width()), 0);
                    hazard->setZValue(2);
                    scene->addItem(hazard);
                }
            }

            if(ismonster)
            {
                monster->setY(monster->y()+5.3-4.9*upcount*0.01);
                if(monster->y()>700)
                {
                    scene->removeItem(monster);
                    delete monster;
                    monster = nullptr;
                    ismonster = false;
                }
            }
            if(ishole)
            {
                hazard->setY(hazard->y()+5.3-4.9*upcount*0.01);
                if(hazard->y()>700)
                {
                    scene->removeItem(hazard);
                    delete hazard;
                    hazard = nullptr;
                    ishole = false;
                }
            }
            if(isrocket && !equiperocket)
            {
                rocket->setY(rocket->y()+5.3-4.9*upcount*0.01);
                if(rocket->y()>700)
                {
                    scene->removeItem(rocket);
                    delete rocket;
                    rocket = nullptr;
                    isrocket = false;
                }
            }
            if(iscap && !equipecap)
            {
                cap->setY(cap->y()+5.3-4.9*upcount*0.01);
                if(cap->y()>700)
                {
                    scene->removeItem(cap);
                    delete cap;
                    cap = nullptr;
                    iscap = false;
                }
            }
            int render;
            for(int i = 0; i < 15; i++)
            {

                plat[i]->setY(plat[i]->y()+5.3-4.9*upcount*0.01);
                if(plat[i]->y()>700)
                {
                    render = qrand()%5 + 1;
                    scene->removeItem(plat[i]);
                    delete plat[i];
                    if(render == 1)
                    {
                        plat[i] = new BluePlat;
                    }
                    else {
                        plat[i] = new GreenPlat;
                    }
                    connect(timer, SIGNAL(timeout()), plat[i], SLOT(special()));
                    if(i == 0)
                        plat[i]->setPos(qrand()%(500-plat[i]->pixmap().width()), plat[14]->y()-qrand()%60-plat[i]->pixmap().height());
                    else {
                        plat[i]->setPos(qrand()%(500-plat[i]->pixmap().width()), plat[i - 1]->y()-qrand()%60-plat[i]->pixmap().height());

                    }
                    scene->addItem(static_cast<QGraphicsPixmapItem*>(plat[i]));
                    if(dice>=25 && dice <= 30)
                    {
                        if(!isrocket && !equiperocket && !equipecap && render != 1)
                        {
                            isrocket = true;
                            rocket = new QGraphicsPixmapItem;
                            rocket->setPixmap(QPixmap(":/res/rocket.png").scaled(42,56));
                            rocket->setPos(plat[i]->x()+plat[i]->pixmap().width()-rocket->pixmap().width(), plat[i]->y()-rocket->pixmap().height());
                            rocket->setZValue(1);
                            scene->addItem(rocket);
                        }
                    }
                    else if(dice >= 10 && dice <= 20)
                    {
                        if(!iscap && !equiperocket && !equipecap && render != 1)
                        {
                            iscap = true;
                            cap = new QGraphicsPixmapItem;
                            cap->setPixmap(QPixmap(":/res/cap.png").scaled(42,35));
                            cap->setPos(plat[i]->x()+plat[i]->pixmap().width()-cap->pixmap().width(), plat[i]->y()-cap->pixmap().height());
                            cap->setZValue(1);
                            scene->addItem(cap);
                        }
                    }
                }
            }
        }
        if((5.3-4.9*upcount*0.01)<0)
        {
            scoreup = false;
            upcount=0;
            jumpSound->stop();
            disconnect(timer, SIGNAL(timeout()), this, SLOT(jump()));
        }
    }
}
void MainWindow::rocketfly()
{
    if(gamestate == GAME)
    {
        upcount++;

        if(upcount<500)
        {
            player->setY(player->y()-8);
            rocket->setY(rocket->y()-8);
            if(scoreup)
            {
                score += 8;
                scoreItem->setPlainText(QString::number(score));
            }
        }
        if(player->x()<0-player->pixmap().width())
        {
            player->setX(500);

        }
        if(player->x()>500)
        {
            player->setX(0-player->pixmap().width());
        }
        if(player->y()<350)
        {
            player->setY(350);
            rocket->setY(350);
            int dice = qrand()%1000;
            if(dice == 40)
            {
                if(!ismonster)
                {
                    ismonster = true;
                    monster = new QGraphicsPixmapItem;
                    monster->setPixmap(QPixmap(":/res/bat1.png"));
                    monster->setPos(qrand()%(400-monster->pixmap().width()), 0);
                    monster->setZValue(3);
                    scene->addItem(monster);
                }
            }
            else if(dice == 50)
            {
                if(!ishole)
                {
                    ishole = true;
                    hazard = new QGraphicsPixmapItem;
                    hazard->setPixmap(QPixmap(":/res/hole.png").scaled(QPixmap(":/res/hole.png").size()*0.6));
                    hazard->setPos(qrand()%(400-hazard->pixmap().width()), 0);
                    hazard->setZValue(2);
                    scene->addItem(hazard);
                }
            }

            if(ismonster)
            {
                monster->setY(monster->y()+8);
                if(monster->y()>700)
                {
                    scene->removeItem(monster);
                    delete monster;
                    monster = nullptr;
                    ismonster = false;
                }
            }
            if(ishole)
            {
                hazard->setY(hazard->y()+8);
                if(hazard->y()>700)
                {
                    scene->removeItem(hazard);
                    delete hazard;
                    hazard = nullptr;
                    ishole = false;
                }
            }
            if(iscap)
            {
                cap->setY(cap->y()+8);
                if(cap->y()>700)
                {
                    scene->removeItem(cap);
                    delete cap;
                    cap = nullptr;
                    iscap = false;
                }
            }
            int render;
            for(int i = 0; i < 15; i++)
            {

                plat[i]->setY(plat[i]->y()+8);
                if(plat[i]->y()>700)
                {
                    render = qrand()%5 + 1;

                    scene->removeItem(plat[i]);
                    delete plat[i];
                    if(render == 1)
                    {
                        plat[i] = new BluePlat;
                    }
                    else {
                        plat[i] = new GreenPlat;
                    }
                    connect(timer, SIGNAL(timeout()), plat[i], SLOT(special()));
                    if(i == 0)
                        plat[i]->setPos(qrand()%(500-plat[i]->pixmap().width()), plat[14]->y()-qrand()%60-plat[i]->pixmap().height());
                    else {
                        plat[i]->setPos(qrand()%(500-plat[i]->pixmap().width()), plat[i - 1]->y()-qrand()%60-plat[i]->pixmap().height());

                    }
                    scene->addItem(static_cast<QGraphicsPixmapItem*>(plat[i]));
                }
            }
        }
        if(upcount>500)
        {
            scoreup = false;
            disconnect(timer, SIGNAL(timeout()), this, SLOT(rocketfly()));
            equiperocket = false;
            upcount = 0;
            isrocket = false;
            scene->removeItem(rocket);
            delete rocket;
            rocket = nullptr;
            connect(timer, SIGNAL(timeout()), this, SLOT(fall()));
        }
    }
}
void MainWindow::capfly()
{
    if(gamestate == GAME)
    {
        upcount++;

        if(upcount<300)
        {
            player->setY(player->y()-5);
            cap->setY(cap->y()-5);
            if(upcount%30==0)
            {
                cap->setPixmap(QPixmap(":/res/cap3.png").scaled(50,30));
            }
            else if(upcount%20==0){
                cap->setPixmap(QPixmap(":/res/cap2.png").scaled(50,30));

            }
            else if(upcount%10==0){
                cap->setPixmap(QPixmap(":/res/cap1.png").scaled(50,30));

            }
            if(scoreup)
            {
                score += 5;
                scoreItem->setPlainText(QString::number(score));
            }
        }
        if(player->x()<0-player->pixmap().width())
        {
            player->setX(500);

        }
        if(player->x()>500)
        {
            player->setX(0-player->pixmap().width());
        }
        if(player->y()<350)
        {
            player->setY(350);
            cap->setY(370-cap->pixmap().height());
            int dice = qrand()%1000;
            if(dice == 40)
            {
                if(!ismonster)
                {
                    ismonster = true;
                    monster = new QGraphicsPixmapItem;
                    monster->setPixmap(QPixmap(":/res/bat1.png"));
                    monster->setPos(qrand()%(400-monster->pixmap().width()), 0);
                    monster->setZValue(3);
                    scene->addItem(monster);
                }
            }
            else if(dice == 50)
            {
                if(!ishole)
                {
                    ishole = true;
                    hazard = new QGraphicsPixmapItem;
                    hazard->setPixmap(QPixmap(":/res/hole.png").scaled(QPixmap(":/res/hole.png").size()*0.6));
                    hazard->setPos(qrand()%(400-hazard->pixmap().width()), 0);
                    hazard->setZValue(2);
                    scene->addItem(hazard);
                }
            }

            if(ismonster)
            {
                monster->setY(monster->y()+5);
                if(monster->y()>700)
                {
                    scene->removeItem(monster);
                    delete monster;
                    monster = nullptr;
                    ismonster = false;
                }
            }
            if(ishole)
            {
                hazard->setY(hazard->y()+5);
                if(hazard->y()>700)
                {
                    scene->removeItem(hazard);
                    delete hazard;
                    hazard = nullptr;
                    ishole = false;
                }
            }
            if(isrocket)
            {
                rocket->setY(rocket->y()+5);
                if(rocket->y()>700)
                {
                    scene->removeItem(rocket);
                    delete rocket;
                    rocket = nullptr;
                    isrocket = false;
                }
            }
            int render;
            for(int i = 0; i < 15; i++)
            {

                plat[i]->setY(plat[i]->y()+5);
                if(plat[i]->y()>700)
                {
                    render = qrand()%5 + 1;

                    scene->removeItem(plat[i]);
                    delete plat[i];
                    if(render == 1)
                    {
                        plat[i] = new BluePlat;
                    }
                    else {
                        plat[i] = new GreenPlat;
                    }
                    connect(timer, SIGNAL(timeout()), plat[i], SLOT(special()));

                    if(i == 0)
                        plat[i]->setPos(qrand()%(500-plat[i]->pixmap().width()), plat[14]->y()-qrand()%60-plat[i]->pixmap().height());
                    else {
                        plat[i]->setPos(qrand()%(500-plat[i]->pixmap().width()), plat[i - 1]->y()-qrand()%60-plat[i]->pixmap().height());

                    }
                    scene->addItem(static_cast<QGraphicsPixmapItem*>(plat[i]));
                }
            }
        }
        if(upcount>300)
        {
            scoreup = false;
            disconnect(timer, SIGNAL(timeout()), this, SLOT(capfly()));
            equipecap = false;
            upcount = 0;
            iscap = false;
            scene->removeItem(cap);
            delete cap;
            cap = nullptr;
            connect(timer, SIGNAL(timeout()), this, SLOT(fall()));
        }
    }
}

void MainWindow::left()
{
    if(gamestate == GAME)
    {
        lcount++;
        player->setX(player->x() - 3);
        if(equipecap)
        {
            cap->setX(player->x() + player->pixmap().width() + 5 - cap->pixmap().width());
        }
        else if(equiperocket)
        {
            rocket->setX(player->x()+player->pixmap().width()-17);
        }
        if(!iskey && lcount >= 1)
        {
            lcount = 0;
            disconnect(timer, SIGNAL(timeout()), this, SLOT(left()));
        }
    }
}
void MainWindow::right()
{
    if(gamestate == GAME)
    {
        rcount++;
        player->setX(player->x() + 3);
        if(equipecap)
        {
            cap->setX(player->x()-5);
        }
        else if(equiperocket)
        {
            rocket->setX(player->x()-player->pixmap().width()+17);
        }
        if(!iskey && rcount >= 1)
        {
            rcount = 0;
            disconnect(timer, SIGNAL(timeout()), this, SLOT(right()));
        }
    }
}
void MainWindow::mousePressEvent(QMouseEvent *e)
{
    switch (gamestate) {
    case MENU:
        if(e->x()>=80&&e->x()<=230&&e->y()>=200&&e->y()<=260)
        {
            clickSound->setMedia(QUrl("qrc:/res/click.mp3"));
            clickSound->play();
            emit gamestart();
        }
        break;
    case GAME:
        if(e->x()>=470&&e->x()<=500&&e->y()>=0&&e->y()<=40)
        {
            clickSound->setMedia(QUrl("qrc:/res/click.mp3"));
            clickSound->play();
            emit gamepause();
        }
        break;
    case PAUSE:
        if(e->x()>=300&&e->x()<=450&&e->y()>=600&&e->y()<=660)
        {
            clickSound->setMedia(QUrl("qrc:/res/click.mp3"));
            clickSound->play();
            emit gameresume();
        }
        break;
    case OVER:
        if(e->x()>=80&&e->x()<=230&&e->y()>=200&&e->y()<=260)
        {
            clickSound->setMedia(QUrl("qrc:/res/click.mp3"));
            clickSound->play();
            emit gamemenu();
        }
        break;
    default:
        break;
    }
}
void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    if(gamestate==GAME)
    {
        switch(e->key())
        {
        case Qt::Key_A:
        case Qt::Key_Left:
            iskey = false;
            break;
        case Qt::Key_D:
        case Qt::Key_Right:
            iskey = false;
            break;
        default:
            break;
        }
    }
}
void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if(gamestate == GAME && !iskey)
    {
        switch(e->key())
        {
        case Qt::Key_A:
        case Qt::Key_Left:
            if(lcount == 0)
            {
                player->setPixmap(QPixmap(":/res/doodle_left.png").scaled(60,60));
                if(equiperocket)
                {
                    rocket->setPixmap(QPixmap(":/res/r_left.png").scaled(56,100));
                }
                dir = LEFT;
                connect(timer, SIGNAL(timeout()), this, SLOT(left()));
            }
            iskey = true;
            break;
        case Qt::Key_D:
        case Qt::Key_Right:
            if(rcount == 0)
            {
                player->setPixmap(QPixmap(":/res/doodle_right.png").scaled(60,60));
                if(equiperocket)
                {
                    rocket->setPixmap(QPixmap(":/res/r_right.png").scaled(56,100));
                }
                dir = RIGHT;
                connect(timer, SIGNAL(timeout()), this, SLOT(right()));
            }
            iskey = true;
            break;
        default:
            break;
        }
    }
    if(gamestate == GAME && !isshoot && !equipecap && !equiperocket)
    {
        switch(e->key())
        {
        case Qt::Key_Space:
        case Qt::Key_Up:
            clickSound->setMedia(QUrl("qrc:/res/click.mp3"));
            clickSound->play();
            if(!isshoot)
            {
                player->setPixmap(QPixmap(":/res/doodleshoot.png").scaled(40,60));
                bullet = new QGraphicsPixmapItem(QPixmap(":/res/bullet.png"));
                bullet->setPos(player->x()+player->pixmap().width()/2-bullet->pixmap().width()/2, player->y());
                scene->addItem(bullet);
                connect(timer, SIGNAL(timeout()), this, SLOT(shoot()));
            }
            isshoot = true;
            break;
        default:
            break;
        }
    }
}
void MainWindow::shoot()
{
    bullet->setPos(bullet->x(), bullet->y()-10);
    if(ismonster)
    {
        if((bullet->x()+bullet->pixmap().width()>monster->x())&&(bullet->x()<(monster->x()+monster->pixmap().width()))&&(bullet->y()+bullet->pixmap().height()>monster->y())&&(bullet->y()+bullet->pixmap().height()<(monster->y()+monster->pixmap().height())))
        {
            scene->removeItem(monster);
            delete monster;
            monster=nullptr;
            ismonster = false;
        }
    }
    if(bullet->y()<0)
    {
        player->setPixmap(QPixmap(":/res/doodle_right.png").scaled(60,60));
        scene->removeItem(bullet);
        delete bullet;
        isshoot = false;
        disconnect(timer, SIGNAL(timeout()), this, SLOT(shoot()));
    }

}

