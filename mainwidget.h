#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QTimer>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
namespace Ui {
class mainWidget;
}

class mainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit mainWidget(QWidget *parent = 0);
    ~mainWidget();
    void dataaccept1();
    void dataaccept2();
    void dataaccept3();
private slots:
    void on_ButtonClear_clicked();

    void on_ButtonClose_clicked();

    void on_Button1DS_clicked();

     void  finishedSlot(QNetworkReply* reply);
     void on_Button2DS_clicked();

     void on_Button3DS_clicked();

     void on_pushButton_clicked();


     void on_textEdit_copyAvailable(bool b);

private:
    Ui::mainWidget *ui;
    QNetworkAccessManager *accessManager;
    QTcpServer *tcpserver;
    QTcpSocket *tcpsocket1;
    QTcpSocket *tcpsocket2;
    QTcpSocket *tcpsocket3;
    QTcpSocket *tcptemp;
    QFile file1;
    QFile file2;
    QFile file3;
    QString fileName;
    QString fileName2;
    QString fileName3;
    QString sendfileName;
    bool over1;
    bool over2;
    bool over3;
    qint64 sendfilesize;
    qint64 sendfilesizeO;
    qint64 fileSize1;
    qint64 recvsize1;
    qint64 fileSize2;
    qint64 recvsize2;
    qint64 fileSize3;
    qint64 recvsize3;
    QTimer *timer1;
    QTimer *timer2;
    QTimer *timer3;
    QTimer *time1;
    QTimer *time2;
    QTimer *time3;
    bool live1;
    bool live2;
    bool live3;

};

#endif // MAINWIDGET_H
