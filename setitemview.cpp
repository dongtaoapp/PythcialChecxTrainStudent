#include "setitemview.h"
#include "ui_setitemview.h"
#include "define.h"
#include <qDebug>
#include <QDir>
#include <QMessageBox>
SetItemView::SetItemView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetItemView)
{
    ui->setupUi(this);
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    this->setWindowTitle(QStringLiteral("ϵͳ����ѡ��"));
    Apppath=QCoreApplication::applicationDirPath();
    pag1_radiobtn=-1;
    pag2_radiobtn=-1;
    configInit = new QSettings(APPPATH, QSettings::IniFormat);
    this->Init();
    connect(this,SIGNAL(changPag(int)),ui->stackedWidget,SLOT(setCurrentIndex(int)));
    connect(ui->btn1,SIGNAL(clicked(bool)),this,SLOT(onBtn1()));
    connect(ui->btn2,SIGNAL(clicked(bool)),this,SLOT(onBtn2()));
    connect(ui->btn3,SIGNAL(clicked(bool)),this,SLOT(onBtn3()));
    connect(ui->cancelBtn,SIGNAL(clicked(bool)),this,SLOT(oncancelBtn()));
    connect(ui->okBtn,SIGNAL(clicked(bool)),this,SLOT(onOkbtn()));
}

SetItemView::~SetItemView()
{
    delete ui;
    delete configInit;
    qDebug()<<__FUNCTION__;
}
void SetItemView::onBtn1()
{
    emit changPag(0);
}
void SetItemView::onBtn2()
{
    emit changPag(1);
}
void SetItemView::onBtn3()
{
    emit changPag(2);
}

void SetItemView::Init()
{
    pag1_radiobtn= configInit->value("System_Param/HeartAuscultateType").toInt();
    pag2_radiobtn=configInit->value("System_Param/LoudSoundType").toInt();
    switch(pag1_radiobtn)
    {
    case 0:
        ui->radioButton->setChecked(true);
        break;
    case 1:
        ui->radioButton_2->setChecked(true);
        break;
    default:
        ui->radioButton->setChecked(true);
        break;
    }
    switch(pag2_radiobtn)
    {
    case 0:
        ui->radioButton_3->setChecked(true);
        break;
    case 1:
        ui->radioButton_4->setChecked(true);
        break;
    default:
        ui->radioButton_3->setChecked(true);
        break;
    }
    ui->studentID->setText(configInit->value("System_Param/deskID").toString());
    ui->web_ip->setText(configInit->value("System_Param/WebIP").toString());
    ui->trachIP->setText(configInit->value("Client/ServerIP").toString());
}
void SetItemView::radioButtonInit()
{
    if(ui->radioButton->isChecked())
    {
        configInit->setValue("System_Param/HeartAuscultateType","0");
    }
    else if(!ui->radioButton->isChecked())
    {
        configInit->setValue("System_Param/HeartAuscultateType","1");
    }
    else if(ui->radioButton_3->isChecked())
    {
        configInit->setValue("System_Param/LoudSoundType","0");
    }
    else if(!ui->radioButton_3->isChecked())
    {
        configInit->setValue("System_Param/LoudSoundType","1");
    }

}


/******д�������ļ�*******/
void SetItemView::onOkbtn()
{
        if(configInit->value("System_Param/deskID").toString()==ui->studentID->text()
                &&configInit->value("System_Param/WebIP").toString()==ui->web_ip->text()
                &&configInit->value("Client/ServerIP").toString()==ui->trachIP->text())
        {
            radioButtonInit();
            this->close();
        }
        else
        {
            configInit->setValue("System_Param/deskID",ui->studentID->text());
            configInit->setValue("System_Param/WebIP",ui->web_ip->text());
            configInit->setValue("Client/ServerIP",ui->trachIP->text());
            radioButtonInit();
            this->close();
            QMessageBox::information(NULL,QStringLiteral("�޸���ʾ"),QStringLiteral("����̨�� ��ʦ��IP webIP�Ѿ��޸��������������"),QMessageBox::Yes);
            qApp->quit();
        }
}

void SetItemView::oncancelBtn()
{
    this->close();
}
