#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTcpSocket>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void startv();
    void stop();
    void recv_show();

    void go_forward();
    void go_backward();
    void go_left();
    void go_right();
    void stop_roll();
    void go_trailing();
    void go_normal();

    void uplook();
    void downlook();
    void leftlook();
    void rightlook();

private:
    QLabel *lbvideo, *lbip, *lbport, *lbmotor, *lbcam;
    QLineEdit *leip, *leport;
    QPushButton *btstart, *btstarght, *btgoback, *btgoleft, *btgoright, *btstop;
    QPushButton *btcamup, *btcamdown, *btcamleft, *btcamright;
    QPushButton *btmod;

    QTcpSocket *sockv, *sockcmd;
    bool readsize;
    int picsize;

    unsigned char m_UpDownBase;
    unsigned char m_LiftRightBase;
};

#endif // WIDGET_H
