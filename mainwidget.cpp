#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFileDialog>
#include <QFileInfo>
#include <QFileDialog>
#include <QDateTime>

mainWidget::mainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainWidget)
{
    ui->setupUi(this);
//    setFixedSize(800,480);
//    Py_SetPythonHome("C:\\Python27");
//    Py_Initialize();
//    PyObject* pModule = PyImport_ImportModule("text");
//    PyObject* pFunhello=PyObject_GetAttrString(pModule,"hello");
//    PyObject_CallFunction(pFunhello,NULL);
    tcpserver =new QTcpServer(this);
    tcpsocket1=new QTcpSocket(this);
    tcpsocket2=new QTcpSocket(this);
    tcpsocket3=new QTcpSocket(this);
    tcptemp=new  QTcpSocket(this);
    timer1=new QTimer(this);
    timer2=new QTimer(this);
    timer3=new QTimer(this);
    time1=new QTimer(this);
    time2=new QTimer(this);
    time3=new QTimer(this);
    over1=true;
    over2=true;
    over3=true;
    live1=false;
    live2=false;
    live3=false;
    accessManager=new QNetworkAccessManager(this);
    connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedSlot(QNetworkReply*)));
    connect(timer1,&QTimer::timeout,
            [=]()
    {
        static int i;
        i++;
        if(i==10){
            i=0;
            if(recvsize1<fileSize1){
                ui->textEdit->append("1file time out");
                over1=1;
                timer1->stop();
            }
        }
    }
            );
    connect(timer2,&QTimer::timeout,
            [=]()
    {
        static int i;
        i++;
        if(i==10){
            i=0;
            if(recvsize2<fileSize2){
                ui->textEdit->append("2file time out");
                over2=1;
                timer2->stop();
            }
        }
    }
            );
    connect(timer3,&QTimer::timeout,
            [=]()
    {
        static int i;
        i++;
        if(i==10){
            i=0;
            if(recvsize3<fileSize3){
                ui->textEdit->append("3file time out");
                over3=1;
                timer3->stop();
            }
        }
    }
            );
    connect(time1,&QTimer::timeout,
            [=]()
    {
        static int i;
        i++;
        if(i==10){
            i=0;
            if(live1==false){
            ui->textEdit->append("1 long time no data!rebuild");
                  tcpsocket1->disconnect();
                  tcpsocket1->close();
                  time1->stop();
            }
            live1=false;
            }
        }

            );
    connect(time2,&QTimer::timeout,
            [=]()
    {
        static int i;
        i++;
        if(i==10){
            i=0;
            if(live2==false){
            ui->textEdit->append("1 long time no data!rebuild");
                  tcpsocket2->disconnect();
                  tcpsocket2->close();
                  time2->stop();
            }
            live2=false;
        }
        }

            );
    connect(time3,&QTimer::timeout,
            [=]()
    {
        static int i;
        i++;
        if(i==10){
            i=0;
            ui->textEdit->append("3 long time no data!rebuild");
            tcpsocket3->disconnect();
            tcpsocket3->close();
            time3->stop();
            }

    }
            );

    ui->label_funny->setPixmap(QPixmap("/home/pi/0.jpg"));
    ui->label_funny->setScaledContents(true);
    tcpserver->listen(QHostAddress::Any,8080);
    setWindowTitle("养老数据中继");
    connect(tcpserver,&QTcpServer::newConnection,
            [=]()
                    {

                       if(tcpsocket1->isOpen()==true)
                           {
                               ui->textEdit->append("1 is buzy!");
                           }
                       else
                           {
                               qDebug()<< tcpsocket1->open(QIODevice::ReadWrite);
                               live1=1;
                                tcpsocket1=tcpserver->nextPendingConnection();
                                QString ip = tcpsocket1->peerAddress().toString();
                                quint16 port =tcpsocket1->peerPort();
                                QString str =QString("[%1:%2] connect succeed").arg(ip).arg(port);
                                ui->textEdit->append(str);
                                connect(tcpsocket1,&QTcpSocket::readyRead,this,&mainWidget::dataaccept1  );
                                connect(tcpsocket1,&QTcpSocket::disconnected,
                                        [=]()
                                {
                                ui->textEdit->append("1 out!");
                                tcpsocket1->disconnect();
                                tcpsocket1->close();
                                }
                                );
                                return;
                             }
                       if(tcpsocket2->isOpen()==true)
                           {
                               ui->textEdit->append("2 is buzy!");
                           }
                       else
                           {
                               qDebug()<< tcpsocket2->open(QIODevice::ReadWrite);
                                tcpsocket2=tcpserver->nextPendingConnection();
                                QString ip = tcpsocket2->peerAddress().toString();
                                quint16 port =tcpsocket2->peerPort();
                                QString str =QString("[%1:%2] connect succeed").arg(ip).arg(port);
                                ui->textEdit->append(str);
                                connect(tcpsocket2,&QTcpSocket::readyRead,this,&mainWidget::dataaccept2  );
                                connect(tcpsocket2,&QTcpSocket::disconnected,
                                        [=]()
                                {
                                ui->textEdit->append("2 out!");
                                tcpsocket2->disconnect();
                                tcpsocket2->close();
                                }
                                );
                                return;
                             }
                       if(tcpsocket3->isOpen()==true)
                           {
                               ui->textEdit->append("3 is buzy!");
                           }
                       else
                           {
                               qDebug()<< tcpsocket3->open(QIODevice::ReadWrite);
                                tcpsocket3=tcpserver->nextPendingConnection();
                                QString ip = tcpsocket3->peerAddress().toString();
                                quint16 port =tcpsocket3->peerPort();
                                QString str =QString("[%1:%2] connect succeed").arg(ip).arg(port);
                                ui->textEdit->append(str);
                                connect(tcpsocket3,&QTcpSocket::readyRead,this,&mainWidget::dataaccept3  );
                                connect(tcpsocket3,&QTcpSocket::disconnected,
                                        [=]()
                                {
                                ui->textEdit->append("3 out!");
                                tcpsocket3->disconnect();
                                tcpsocket3->close();
                                }
                                );
                                return;
                             }
                       tcptemp->open(QIODevice::ReadWrite);
                       tcptemp = tcpserver->nextPendingConnection();
                       ui->textEdit->append("can't in!");
                       tcptemp->disconnect();
                       tcptemp->close();
                       }
                  );

}

mainWidget::~mainWidget()
{
    delete ui;
    delete tcpserver;
    tcpsocket1->disconnect();
    tcpsocket1->close();
    tcpsocket2->disconnect();
    tcpsocket2->close();
    tcpsocket3->disconnect();
    tcpsocket3->close();
    delete tcpsocket1;
    delete tcpsocket2;
    delete tcpsocket3;
    delete tcptemp;

}
void mainWidget::dataaccept1()
{
    if(time1->isActive()==false)
     time1->start(3000);
    live1=true;
    QByteArray buf= tcpsocket1->readAll();
    qint64 len;
    int flag=0;
    qDebug()<<"in"<<buf.length();
    len=buf.length();

    if (over1==true)
    {
        if(file1.isOpen()==true){
           file1.close();
        }
        QString comm=QString(buf).left(len);
        qDebug()<<comm.length();
        qDebug()<<"fuck";
     //  QByteArray buf= tcpsocket->readAll();
cheak:
    if ( comm.indexOf("DATA:")!=-1&&comm.indexOf("file123:")!=-1){
        comm.indexOf("file123")>comm.indexOf("DATA:")? flag=1:flag=2;
    }
    else{
        if(comm.indexOf("DATA:")!=-1)
            flag=1;
        else if (comm.indexOf("file123:")!=-1)
            flag=2;
        else
            flag=0;
    }

    if(flag==1)
       {
              int headP= comm.indexOf("DATA:");
              comm=comm.mid(headP+5);
       //       qDebug()<<QString(comm);
              ui->textEdit->append("1data accept!");
              QString temp =comm.section("//",0,0);
              ui->lineEdit_Temp->setText(temp);
              QString humi =comm.section("//",1,1);
              ui->lineEdit_Humi->setText(humi);
              QString gus =comm.section("//",2,2);
              ui->lineEdit_Gus->setText(gus);
              QString position =comm.section("//",3,3);
              ui->label_Place1->setText(position);
              QString endF =comm.section("//",4,4);
              if(endF=="endF"){
                 ui->textEdit->append("1data accept over!");
                 QByteArray postData;
                 QNetworkRequest request;
                 QString temp,humi,gus,inf,place,position;
                 temp=ui->lineEdit_Temp->text();
                 humi=ui->lineEdit_Humi->text();
                 gus=ui->lineEdit_Gus->text();
                 position=ui->label_Place1->text();
                 if(position=="Badroom"){
                     place='1';
                     ui->label_Place1->setText("卧室");
                 }
                 else if (position=="Kitchen") {
                     place='2';
                     ui->label_Place1->setText("厨房");
                 }
                 else if (position=="Saloon") {
                     place='3';
                     ui->label_Place1->setText("客厅");
                 }
                 else{
                 ui->textEdit->append("position not in");
                 return;
                 }
                 inf='1';
                 QString data;
                 data=QString("{ \"deviceId\":%1,\"temp\":%2, \"humi\":%3, \"gus\":%4,\"inf\":%5 }").arg(place).arg(temp).arg(humi).arg(gus).arg(inf);
                 qDebug()<<data;
                 postData.append(data);
                 request.setUrl(QUrl("http://strend.iok.la/device/add_logs.do"));
                 request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
                 request.setRawHeader("Content-Type","application/json");
                 accessManager->post(request, postData);
              }
              else
                  ui->textEdit->append("1data err");
            int in= comm.indexOf("endF");
            comm=comm.mid(in+6);
             goto cheak;
       }
    if(flag==2)
    {
            int filehdP= comm.indexOf("file123:");
            comm=comm.mid(filehdP+8);
            fileName =comm.section("//",0,0);
            if(-1!=fileName.indexOf("sound")){
                 QDateTime datetime=QDateTime::currentDateTime();
                 QString cur=datetime.toString("yy.MM.dd.hh.mm.ss");
                 fileName.append(cur);
                 fileName.append(".wav");
                 fileName=QString("/home/pi/pension/01/WAV/%1").arg(fileName);
                 // /home/pi/pension/01/WAV/
            }
            if(fileName=="picture"){
                QDateTime datetime=QDateTime::currentDateTime();
                QString cur=datetime.toString("yy.MM.dd.hh.mm.ss");
                fileName.append(cur);
                fileName.append(".bmp");
                fileName=QString("C:\\Users\\hasee\\Desktop\\picture\\bmp%1").arg(fileName);
            }// /home/pi/pension/01/BMP/
            ui->textEdit->append(fileName);
            fileSize1 =comm.section("//",1,1).toInt();
            QString size=QString("%1").arg(fileSize1);
            ui->textEdit->append(size);
            QString endF =comm.section("//",2,2);
            if(endF=="endF"){

                ui->textEdit->append("1filehead accept over!");
                recvsize1=0;
                file1.setFileName(fileName);
                bool isOK =file1.open(QIODevice::WriteOnly);
                if(isOK==false){
                    ui->textEdit->append("1file new false!");
                    return;
                }
                else{
                    timer1->start(2000);
                    ui->textEdit->append("1file new succeed!");
                    int in= comm.indexOf("endF");
                    comm=comm.mid(in+6);
                    if(comm.length()>=fileSize1){
                        char* bufmid;
                        bufmid=comm.toUtf8().data();
                        file1.write(bufmid,fileSize1);
                        recvsize1+=fileSize1;
                        ui->textEdit->append("1file accept succeed!");
                        over1=true;
                        file1.close();
                        if(-1!=fileName.indexOf("picture")){
                        QMatrix angle;
                        angle.rotate(90);
                        ui->label_Image_1->setPixmap(QPixmap(fileName).transformed(angle,Qt::FastTransformation));
                        ui->label_Image_1->setScaledContents(true);
                        }
                        comm=comm.mid(fileSize1);
                        goto cheak;
                    }
                    else {

                        char* bufmid;
                            bufmid=comm.toUtf8().data();
                            len =file1.write(bufmid,comm.length()>fileSize1-recvsize1?fileSize1-recvsize1:comm.length());
                            recvsize1+=len;
                            qDebug()<<recvsize1;
                            if(fileSize1>recvsize1)
                                over1=false;
                            else {  over1=true;
                                 ui->textEdit->append("1file accept succeed!");
                                  file1.close();
                           }
                     }
                 }
           }
            else{
                ui->textEdit->append("1head err!");
               return;
           }
    }
   else
       return;
}
    else{
            len =file1.write(buf,buf.length()>fileSize1-recvsize1?fileSize1-recvsize1:buf.length());
            recvsize1+=len;
            qDebug()<<buf.length();
            qDebug()<<len<<"p"<<recvsize1;
            if(fileSize1>recvsize1)
                over1=false;
            else {
                over1=true;
                ui->textEdit->append("1file accept succeed!");
                file1.close();
            }
            if(over1==true&&(-1!=fileName.indexOf(".bmp"))){
                QMatrix angle;
                angle.rotate(90);
                ui->label_Image_1->setPixmap(QPixmap(fileName).transformed(angle,Qt::FastTransformation));
                ui->label_Image_1->setScaledContents(true);
            }

    }
}
void mainWidget::dataaccept2()
{
    if(time2->isActive()==false)
    time2->start(2500);
    live2=true;
     QByteArray buf= tcpsocket2->readAll();
     qint64 len;
     int flag=0;
     qDebug()<<"in"<<buf.length();
     len=buf.length();

     if (over2==true)
     {
         if(file2.isOpen()==true){
            file2.close();
         }
         QString comm=QString(buf).left(len);
         qDebug()<<comm.length();
         qDebug()<<"fuck";
      //  QByteArray buf= tcpsocket->readAll();
cheak:
     if ( comm.indexOf("DATA:")!=-1&&comm.indexOf("file123:")!=-1){
         comm.indexOf("file123")>comm.indexOf("DATA:")? flag=1:flag=2;
     }
     else{
         if(comm.indexOf("DATA:")!=-1)
             flag=1;
         else if (comm.indexOf("file123:")!=-1)
             flag=2;
         else
             flag=0;
     }

     if(flag==1)
        {
               int headP= comm.indexOf("DATA:");
               comm=comm.mid(headP+5);
        //       qDebug()<<QString(comm);
               ui->textEdit->append("2data accept!");
               QString temp =comm.section("//",0,0);
               ui->lineEdit_Temp_2->setText(temp);
               QString humi =comm.section("//",1,1);
               ui->lineEdit_Humi_2->setText(humi);
               QString gus =comm.section("//",2,2);
               ui->lineEdit_Gus_2->setText(gus);
               QString position =comm.section("//",3,3);
               ui->label_Place2->setText(position);
               QString endF =comm.section("//",4,4);
               if(endF=="endF"){
                  ui->textEdit->append("2data accept over!");
                  QByteArray postData;
                  QNetworkRequest request;
                  QString temp,humi,gus,inf,place,position;
                  temp=ui->lineEdit_Temp_2->text();
                  humi=ui->lineEdit_Humi_2->text();
                  gus=ui->lineEdit_Gus_2->text();
                  position=ui->label_Place2->text();
                  if(position=="Badroom"){
                      place='1';
                      ui->label_Place2->setText("卧室");
                  }
                  else if (position=="Kitchen") {
                      place='2';
                      ui->label_Place2->setText("厨房");
                  }
                  else if (position=="Saloon") {
                      place='3';
                      ui->label_Place2->setText("客厅");
                  }
                  else{
                  ui->textEdit->append("position not in");
                  return;
                  }
                  inf='1';
                  QString data;
                  data=QString("{ \"deviceId\":%1,\"temp\":%2, \"humi\":%3, \"gus\":%4,\"inf\":%5 }").arg(place).arg(temp).arg(humi).arg(gus).arg(inf);
                  qDebug()<<data;
                  postData.append(data);
                  request.setUrl(QUrl("http://strend.iok.la/device/add_logs.do"));
                  request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
                  request.setRawHeader("Content-Type","application/json");
                  accessManager->post(request, postData);
               }
               else
                   ui->textEdit->append("2data err");
             int in= comm.indexOf("endF");
             comm=comm.mid(in+6);
              qDebug()<<comm;
              goto cheak;
        }
     if(flag==2)
     {
             int filehdP= comm.indexOf("file123:");
             comm=comm.mid(filehdP+8);
             fileName2 =comm.section("//",0,0);
             if(-1!=fileName2.indexOf("sound")){
                  QDateTime datetime=QDateTime::currentDateTime();
                  QString cur=datetime.toString("yy.mm.dd.hh.mm.ss");
                  fileName2.append(cur);
                  fileName2.append(".wav");
                  fileName2=QString("/home/pi/pension/02/WAV/%1").arg(fileName2);
             }
             if(fileName2=="picture"){
                 QDateTime datetime=QDateTime::currentDateTime();
                 QString cur=datetime.toString("yy.mm.dd.hh.mm.ss");
                 fileName2.append(cur);
                 fileName2.append(".bmp");
                 fileName2=QString("C:\\Users\\hasee\\Desktop\\picture\\bmp%1").arg(fileName2);
             }      // home/pi/pension/02/BMP/
             ui->textEdit->append(fileName2);
             fileSize2 =comm.section("//",1,1).toInt();
             QString size=QString("%1").arg(fileSize2);
             ui->textEdit->append(size);
             QString endF =comm.section("//",2,2);
             if(endF=="endF"){

                 ui->textEdit->append("2filehead accept over!");
                 recvsize2=0;
                 file2.setFileName(fileName2);
                 bool isOK =file2.open(QIODevice::WriteOnly);
                 if(isOK==false){
                     ui->textEdit->append("2file new false!");
                     return;
                 }
                 else{
                     timer2->start(1000);
                     ui->textEdit->append("2file new succeed!");
                     int in= comm.indexOf("endF");
                     comm=comm.mid(in+6);
                     if(comm.length()>=fileSize2){
                         char* bufmid;
                         bufmid=comm.toUtf8().data();
                         file2.write(bufmid,fileSize2);
                         recvsize2+=fileSize2;
                         ui->textEdit->append("2file accept succeed!");
                         over2=true;
                         file2.close();
                         if(-1!=fileName2.indexOf("picture")){
                         QMatrix angle;
                         angle.rotate(90);
                         ui->label_Image_2->setPixmap(QPixmap(fileName2).transformed(angle,Qt::FastTransformation));
                         ui->label_Image_2->setScaledContents(true);
                         }
                         comm=comm.mid(fileSize2);
                         goto cheak;
                     }
                     else {

                         char* bufmid;
                             bufmid=comm.toUtf8().data();
                             len =file2.write(bufmid,comm.length()>fileSize2-recvsize2?fileSize2-recvsize2:comm.length());
                             recvsize2+=len;
                             qDebug()<<recvsize2;
                             if(fileSize2>recvsize2)
                                 over2=false;
                             else {  over2=true;
                                  ui->textEdit->append("2file accept succeed!");
                                   file2.close();
                            }
                      }
                  }
            }
             else{
                 ui->textEdit->append("2head err!");
                return;
            }
     }
    else
        return;
}
     else{
             len =file2.write(buf,buf.length()>fileSize2-recvsize2?fileSize2-recvsize2:buf.length());
             recvsize2+=len;
             qDebug()<<buf.length();
             qDebug()<<len<<"p"<<recvsize2;
             if(fileSize2>recvsize2)
                 over2=false;
             else { over2=true;
                 ui->textEdit->append("2file accept succeed!");
                 file2.close();
             }
             if(over2==true&&(-1!=fileName2.indexOf(".bmp"))){
                 QMatrix angle;
                 angle.rotate(90);
                 ui->label_Image_2->setPixmap(QPixmap(fileName2).transformed(angle,Qt::FastTransformation));
                 ui->label_Image_2->setScaledContents(true);
             }

     }
}
void mainWidget::dataaccept3()
{
    if(time3->isActive()==false)
     time3->start(2500);
     live3=true;
     QByteArray buf= tcpsocket3->readAll();
     qint64 len;
     int flag=0;
     qDebug()<<"in"<<buf.length();
     len=buf.length();

     if (over3==true)
     {
         if(file3.isOpen()==true){
            file3.close();
         }
         QString comm=QString(buf).left(len);
         qDebug()<<comm.length();
         qDebug()<<"fuck";
      //  QByteArray buf= tcpsocket->readAll();
cheak:
     if ( comm.indexOf("DATA:")!=-1&&comm.indexOf("file123:")!=-1){
         comm.indexOf("file123")>comm.indexOf("DATA:")? flag=1:flag=2;
     }
     else{
         if(comm.indexOf("DATA:")!=-1)
             flag=1;
         else if (comm.indexOf("file123:")!=-1)
             flag=2;
         else
             flag=0;
     }

     if(flag==1)
        {
               int headP= comm.indexOf("DATA:");
               comm=comm.mid(headP+5);
        //       qDebug()<<QString(comm);
               ui->textEdit->append("1data accept!");
               QString temp =comm.section("//",0,0);
               ui->lineEdit_Temp_3->setText(temp);
               QString humi =comm.section("//",1,1);
               ui->lineEdit_Humi_3->setText(humi);
               QString gus =comm.section("//",2,2);
               ui->lineEdit_Gus_3->setText(gus);
               QString position =comm.section("//",3,3);
               ui->label_Place3->setText(position);
               QString endF =comm.section("//",4,4);
               if(endF=="endF"){
                  ui->textEdit->append("3data accept over!");
                  QByteArray postData;
                  QNetworkRequest request;
                  QString temp,humi,gus,inf,place,position;
                  temp=ui->lineEdit_Temp_3->text();
                  humi=ui->lineEdit_Humi_3->text();
                  gus=ui->lineEdit_Gus_3->text();
                  position=ui->label_Place3->text();
                  if(position=="Badroom"){
                      place='1';
                      ui->label_Place3->setText("卧室");
                  }
                  else if (position=="Kitchen") {
                      place='2';
                      ui->label_Place3->setText("厨房");
                  }
                  else if (position=="Saloon") {
                      place='3';
                      ui->label_Place3->setText("客厅");
                  }
                  else{
                  ui->textEdit->append("position not in");
                  return;
                  }
                  inf='1';
                  QString data;
                  data=QString("{ \"deviceId\":%1,\"temp\":%2, \"humi\":%3, \"gus\":%4,\"inf\":%5 }").arg(place).arg(temp).arg(humi).arg(gus).arg(inf);
                  qDebug()<<data;
                  postData.append(data);
                  request.setUrl(QUrl("http://strend.iok.la/device/add_logs.do"));
                  request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
                  request.setRawHeader("Content-Type","application/json");
                  accessManager->post(request, postData);

                  int in= comm.indexOf("endF");
                  comm=comm.mid(in+6);
                   qDebug()<<comm;
                   goto cheak;
               }
               else
                   ui->textEdit->append("3data err");
        }
     if(flag==2)
     {
             int filehdP= comm.indexOf("file123:");
             comm=comm.mid(filehdP+8);
             fileName3 =comm.section("//",0,0);
             if(-1!=fileName3.indexOf("sound")){
                  QDateTime datetime=QDateTime::currentDateTime();
                  QString cur=datetime.toString("yy.MM.dd.hh.mm.ss");
                  fileName3.append(cur);
                  fileName3.append(".wav");
                  fileName3=QString("/home/pi/pension/03/WAV/%1").arg(fileName3);
             }
             if(fileName3=="picture"){
                 QDateTime datetime=QDateTime::currentDateTime();
                 QString cur=datetime.toString("yy.MM.dd.hh.mm.ss");
                 fileName3.append(cur);
                 fileName3.append(".bmp");
                 fileName3=QString("C:\\Users\\hasee\\Desktop\\picture\\bmp%1").arg(fileName3);
             }
             ui->textEdit->append(fileName3);
             fileSize3 =comm.section("//",1,1).toInt();
             QString size=QString("%1").arg(fileSize3);
             ui->textEdit->append(size);
             QString endF =comm.section("//",2,2);
             if(endF=="endF"){

                 ui->textEdit->append("3filehead accept over!");
                 recvsize3=0;
                 file3.setFileName(fileName3);
                 bool isOK =file3.open(QIODevice::WriteOnly);
                 if(isOK==false){
                     ui->textEdit->append("3file new false!");
                     return;
                 }
                 else{
                     timer3->start(600);
                     ui->textEdit->append("3file new succeed!");
                     int in= comm.indexOf("endF");
                     comm=comm.mid(in+6);
                     if(comm.length()>=fileSize3){
                         char* bufmid;
                         bufmid=comm.toUtf8().data();
                         file3.write(bufmid,fileSize3);
                         recvsize3+=fileSize3;
                         ui->textEdit->append("3file accept succeed!");
                         over3=true;
                         file3.close();
                         if(-1!=fileName3.indexOf("picture")){
                         QMatrix angle;
                         angle.rotate(90);
                         ui->label_Image_3->setPixmap(QPixmap(fileName3).transformed(angle,Qt::FastTransformation));
                         ui->label_Image_3->setScaledContents(true);
                         }
                         comm=comm.mid(fileSize3);
                         goto cheak;
                     }
                     else {

                         char* bufmid;
                             bufmid=comm.toUtf8().data();
                             len =file3.write(bufmid,comm.length()>fileSize3-recvsize3?fileSize3-recvsize3:comm.length());
                             recvsize3+=len;
                             qDebug()<<recvsize3;
                                 over3=false;
                      }
                  }
            }
             else{
                 ui->textEdit->append("3head err!");
                return;
            }
     }
    else
        return;
}
     else{
             len =file3.write(buf,buf.length()>fileSize3-recvsize3?fileSize3-recvsize3:buf.length());
             recvsize3+=buf.length();
             qDebug()<<buf.length();
             qDebug()<<len<<"p"<<recvsize3;
             if(fileSize3>recvsize3)
                 over3=false;
             else { over3=true;
                 ui->textEdit->append("3file accept succeed!");
                 file3.close();
             }
             if(over3==true&&(-1!=fileName3.indexOf(".bmp"))){
                 QMatrix angle;
                 angle.rotate(90);
                 ui->label_Image_3->setPixmap(QPixmap(fileName3).transformed(angle,Qt::FastTransformation));
                 ui->label_Image_3->setScaledContents(true);
             }

     }
}

void mainWidget::on_ButtonClear_clicked()
{
    ui->textEdit->clear();
}
void mainWidget::on_ButtonClose_clicked()
{
    mainWidget::close();
}

void mainWidget::on_Button1DS_clicked()
{
tcpsocket1->write("apply");
}

void mainWidget::on_Button2DS_clicked()
{
tcpsocket2->write("apply");
}

void mainWidget::on_Button3DS_clicked()
{
tcpsocket3->write("apply");
}

void mainWidget::finishedSlot(QNetworkReply* reply)
{
     if (reply->error() == QNetworkReply::NoError)
     {
         QByteArray bytes = reply->readAll();
         qDebug() << bytes;
         if(bytes.indexOf("0"))
             ui->textEdit->append("web data succeed");
     }
     else
     {
         qDebug() << "finishedSlot errors here";
         qDebug( "found error .... code: %d\n", (int)reply->error());
         qDebug(qPrintable(reply->errorString()));
         QByteArray bytes = reply->readAll();
         qDebug() << bytes;
     }
     reply->deleteLater();
}

void mainWidget::on_pushButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,"open","../");
    QFile file;
    sendfileName.clear();
    QFileInfo info(filePath);
    qDebug( )<<filePath;
    sendfileName = info.fileName();
    file.setFileName(filePath);
    QNetworkRequest request;
    file.open(QIODevice::ReadOnly);
    //读取所有文件
    QByteArray fileContent = file.readAll();
    //边界的时候这里加两杠
    QString boundary = "----weasdashDsdesd";
    //起始边界
    QString startBoundary = "--" + boundary;
    //结束边界
    QString endBoundary = "\r\n--" + boundary + "--\r\n";
    //设置传输类型
    QString qContentType = "multipart/form-data; boundary=" + boundary;
    //要发送的内容
    QByteArray content;

    QString tempStr = startBoundary;

    tempStr += "\r\nContent-Disposition: form-data; name=\"deviceId\"; \r\n\r\n";
    tempStr+='1';
    tempStr+="\r\n";
    content.append(tempStr);
    tempStr = startBoundary;
    tempStr += "\r\nContent-Disposition: form-data; name=\"file\"; filename=\" ";
    tempStr+=sendfileName;
    tempStr+= "\r\n";
    tempStr += "Content-Type: text/plain\r\n\r\n";
    content.append(tempStr);
    content.append(fileContent);
    content.append(endBoundary);
    request.setUrl(QUrl("http://strend.iok.la/device/upload_file.do"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, qContentType.toLatin1());
    accessManager->post(request,content);
}

