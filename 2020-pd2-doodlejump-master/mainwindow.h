#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QTime>
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>
#include "baseplat.h"
#include "greenplat.h"
#include "blueplat.h"
#include <QMediaPlayer>
#include <QMediaPlaylist>
enum{RIGHT = 0, LEFT = 1};
enum{MENU = 0, GAME = 1, PAUSE = 2, OVER = 3};
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void mousePressEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
signals:
    void gamestart();
    void gamepause();
    void gameresume();
    void gamemenu();
public slots:
    void menu();
    void game();
    void fall();
    void jump();
    void left();
    void right();
    void rocketfly();
    void capfly();
    void pause();
    void shoot();
private:
    Ui::MainWindow *ui;
    QWidget *mainwidget = nullptr;
    QGraphicsPixmapItem *player = nullptr, *bullet = nullptr;
    BasePlat **plat = nullptr;
    QGraphicsScene *scene = nullptr;
    QGraphicsView *graphicsView = nullptr;
    QGraphicsPixmapItem *back = nullptr, *resumebutton = nullptr, *monster = nullptr, *hazard = nullptr, *rocket = nullptr, *cap = nullptr;
    QGraphicsTextItem *scoreItem = nullptr, *pauseItem = nullptr, *finalscoreItem = nullptr;
    QTimer *timer = nullptr;
    int upcount = 0, downcount = 0, lcount = 0, rcount = 0, scount = 0;
    QMediaPlayer *backgroundMusic = new QMediaPlayer;
    QMediaPlaylist *List = new QMediaPlaylist;
    QMediaPlayer *clickSound = new QMediaPlayer;
    QMediaPlayer *jumpSound = new QMediaPlayer;
    QMediaPlayer *rocketSound = new QMediaPlayer;
    QMediaPlayer *capSound = new QMediaPlayer;
    int score = 0;
    int dir = RIGHT;
    int gamestate = MENU;
    double lasty = 0;
    bool iskey = false, isshoot = false, scoreup = true, ismonster = false, ishole = false;
    bool isrocket = false, iscap = false, equiperocket = false, equipecap = false;
};

#endif // MAINWINDOW_H
