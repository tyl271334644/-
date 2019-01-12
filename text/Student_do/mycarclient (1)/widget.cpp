#include "widget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHostAddress>
#include <QPixmap>
#include <QByteArray>
#include "cmd.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    lbvideo = new QLabel;
    lbvideo->setFixedSize(320,240);
    lbvideo->setScaledContents(true);
    lbip = new QLabel("IP");
    lbport = new QLabel("port");

    m_UpDownBase = 0x3e;
    m_LiftRightBase = 0x46;

    leip = new QLineEdit("192.168.1.1");
    leport = new QLineEdit("8888");

    btstart = new QPushButton("START");

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(lbip);
    hbox->addWidget(leip);
    hbox->addWidget(lbport);
    hbox->addWidget(leport);
    hbox->addWidget(btstart);

    lbmotor = new QLabel("MOTOR");
    btstarght = new QPushButton("↑");
    btgoback  = new QPushButton("↓");
    btgoleft  = new QPushButton("←");
    btgoright = new QPushButton("→");

    btmod = new QPushButton("TRAIL");

    QHBoxLayout *hbox_b = new QHBoxLayout;
    hbox_b->addWidget(lbmotor);
    hbox_b->addWidget(btstarght);
    hbox_b->addWidget(btgoback);
    hbox_b->addWidget(btgoleft);
    hbox_b->addWidget(btgoright);
    hbox_b->addWidget(btmod);

    lbcam = new QLabel("STEERING ENGINE");
    btcamup = new QPushButton("up");
    btcamdown = new QPushButton("down");
    btcamleft = new QPushButton("left");
    btcamright = new QPushButton("right");

    QHBoxLayout *hbox_c = new QHBoxLayout;
    hbox_c->addWidget(lbcam);
    hbox_c->addWidget(btcamup);
    hbox_c->addWidget(btcamdown);
    hbox_c->addWidget(btcamleft);
    hbox_c->addWidget(btcamright);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addLayout(hbox);
    vbox->addWidget(lbvideo);
    vbox->addLayout(hbox_b);
    vbox->addLayout(hbox_c);

    setLayout(vbox);
    connect(btstart, SIGNAL(clicked(bool)), this, SLOT(startv()));

    sockcmd = new QTcpSocket;
    sockcmd->connectToHost("192.168.1.1",2001);

    connect(btstarght, SIGNAL(pressed()), this, SLOT(go_forward()));
    connect(btgoback, SIGNAL(pressed()), this, SLOT(go_backward()));
    connect(btgoleft, SIGNAL(pressed()), this, SLOT(go_left()));
    connect(btgoright, SIGNAL(pressed()), this, SLOT(go_right()));

    connect(btstarght, SIGNAL(released()), this, SLOT(stop_roll()));
    connect(btgoback, SIGNAL(released()), this, SLOT(stop_roll()));
    connect(btgoleft, SIGNAL(released()), this, SLOT(stop_roll()));
    connect(btgoright, SIGNAL(released()), this, SLOT(stop_roll()));

    connect(btmod, SIGNAL(clicked(bool)), this, SLOT(go_normal()));

    connect(btcamup, SIGNAL(pressed()), this, SLOT(uplook()));
    connect(btcamdown, SIGNAL(pressed()), this, SLOT(downlook()));
    connect(btcamleft, SIGNAL(pressed()), this, SLOT(leftlook()));
    connect(btcamright, SIGNAL(pressed()), this, SLOT(rightlook()));
}

void Widget::startv()
{
    readsize = true;
    sockv = new QTcpSocket(this);
    connect(sockv, SIGNAL(readyRead()), this, SLOT(recv_show()));
    sockv->connectToHost(QHostAddress(leip->text()), atoi(leport->text().toStdString().c_str()));
    btstart->setText("STOP");
    leip->setDisabled(true);
    leport->setDisabled(true);

    disconnect(btstart, SIGNAL(clicked(bool)), this, SLOT(startv()));
    connect(btstart, SIGNAL(clicked(bool)), this, SLOT(stop()));
}

void Widget::stop()
{
    sockv->close();
    sockv->deleteLater();

    disconnect(btstart, SIGNAL(clicked(bool)), this, SLOT(stop()));
    connect(btstart, SIGNAL(clicked(bool)), this, SLOT(startv()));

    leip->setDisabled(false);
    leport->setDisabled(false);
    btstart->setText("START");
}

void Widget::recv_show()
{
    if(readsize)
    {
        char buf[10] = {0};
        sockv->read(buf,10);
        picsize = atoi(buf);
        readsize = false;
    }
    else
    {
        if(sockv->bytesAvailable() < picsize)
        {
            return;
        }
        char buf[320*240*3];
        sockv->read(buf, picsize);
        QPixmap pix;
        pix.loadFromData((uchar *)buf, picsize,"jpeg");
        lbvideo->setPixmap(pix);
        readsize = true;
    }
}

void Widget::go_forward()
{
    sockcmd->write(CMD_ROLL_FORWARD, CMD_SIZE);
}

void Widget::go_backward()
{
    sockcmd->write(CMD_ROLL_BACKWARD, CMD_SIZE);
}

void Widget::go_left()
{
    sockcmd->write(CMD_ROLL_LEFT, CMD_SIZE);
}

void Widget::go_right()
{
    sockcmd->write(CMD_ROLL_RIGHT, CMD_SIZE);
}

void Widget::stop_roll()
{
    sockcmd->write(CMD_ROLL_STOP, CMD_SIZE);
}

void Widget::go_normal()
{
    btmod->setText("TRAIL");
    sockcmd->write(CMD_NORMAL_MODE, CMD_SIZE);
    disconnect(btmod, SIGNAL(clicked(bool)), this, SLOT(go_normal()));
    connect(btmod, SIGNAL(clicked(bool)), this, SLOT(go_trailing()));
}

void Widget::go_trailing()
{
    btmod->setText("NORMAL");
    sockcmd->write(CMD_TRAILING_MODE, CMD_SIZE);
    disconnect(btmod, SIGNAL(clicked(bool)), this, SLOT(go_trailing()));
    connect(btmod, SIGNAL(clicked(bool)), this, SLOT(go_normal()));
}

void Widget::uplook()
{
    QByteArray Qbtata;
    m_UpDownBase -= 1;

    unsigned char cmd[5] = {0xff,0x01,0x07,0x0F,0xff}; //uplook
    cmd[3] =  m_UpDownBase;
    int i=0;
    for(i=0;i<5;i++)
    {
           Qbtata.append(cmd[i]);
    }
    sockcmd->write(Qbtata);
}

void Widget::downlook()
{
    QByteArray Qbtata;
    m_LiftRightBase += 1;

    unsigned char cmd[5] = {0xff,0x01,0x07,0x0F,0xff}; //uplook
    cmd[3] =  m_LiftRightBase;
    int i=0;
    for(i=0;i<5;i++)
    {
           Qbtata.append(cmd[i]);
    }
    sockcmd->write(Qbtata);
}

void Widget::leftlook()
{
    QByteArray Qbtata;
    m_LiftRightBase -= 1;

    unsigned char cmd[5] = {0xff,0x01,0x08,0x0F,0xff}; //uplook
    cmd[3] =  m_LiftRightBase;
    int i=0;
    for(i=0;i<5;i++)
    {
           Qbtata.append(cmd[i]);
    }
    sockcmd->write(Qbtata);
}

void Widget::rightlook()
{
    QByteArray Qbtata;
    m_LiftRightBase += 1;

    unsigned char cmd[5] = {0xff,0x01,0x08,0x0F,0xff}; //uplook
    cmd[3] =  m_LiftRightBase;
    int i=0;
    for(i=0;i<5;i++)
    {
           Qbtata.append(cmd[i]);
    }
    sockcmd->write(Qbtata);
}

Widget::~Widget()
{

}
