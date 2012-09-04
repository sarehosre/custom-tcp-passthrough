#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->lineEditHost->setText("109.234.74.22");109.234.78.19
    ui->lineEditHost->setText("109.234.78.19");
    ui->lineEditPort->setText("10001");

    Server = new QTcpServer(this);
    if (Server->listen(QHostAddress::Any, 10001)) {
        ui->logLineEdit->appendPlainText("server now listening on port 10001");
        internalSocket = new QTcpSocket();
        externalSocket = new QTcpSocket();

        connect(Server, SIGNAL(newConnection()), this, SLOT(manageInQuery()));
        connect(internalSocket, SIGNAL(disconnected()), this, SLOT(manageDisco()));

    } else {
        ui->logLineEdit->appendPlainText("error: server can't listen on port 10001, probably another programme is already listening on that port");
        ui->logLineEdit->appendPlainText("try restarting this programme");
        ui->logLineEdit->appendPlainText("if that doesn't help, perform a clean boot and start this programme again");
    }


}


MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::manageInQuery() {
    qDebug() << "manageInQuery()";

    Server->blockSignals(true);
    //disconnect(Server,SIGNAL(newConnection()));
    ui->logLineEdit->appendPlainText("a client connected");

    internalSocket = Server->nextPendingConnection();
    internalSocket->blockSignals(false);
    connect(internalSocket, SIGNAL(disconnected()), this, SLOT(manageDisco()));

    connect(externalSocket, SIGNAL(connected()), this, SLOT(connectedToExt()));
    externalSocket->connectToHost(ui->lineEditHost->text(), ui->lineEditPort->text().toInt());

    if (internalSocket->isWritable()) internalSocket->write(externalSocket->readAll());
    connect(internalSocket, SIGNAL(readyRead()), this, SLOT(readIntSocket()));

}

void MainWindow::connectedToExt() {
    if (externalSocket->isWritable()) externalSocket->write(internalSocket->readAll());
    connect(externalSocket, SIGNAL(readyRead()), this, SLOT(readExtSocket()));
}


void MainWindow::manageDisco() {
    internalSocket->blockSignals(true);
    //disconnect(internalSocket, SIGNAL(disconnected()));
    internalSocket->abort();
    //internalSocket = new QTcpSocket();
    externalSocket->abort();
    //externalSocket = new QTcpSocket();
    ui->logLineEdit->appendPlainText("a client disconnected");
    if (Server->hasPendingConnections()) {
        qDebug() << "jawohl";
        manageInQuery();
    } else {
        Server->blockSignals(false);
    }
}



void MainWindow::readIntSocket() {
    if (!externalSocket->isWritable()) return;
    qDebug() << "readIntSocket()";
    externalSocket->write(internalSocket->readAll());
}


void MainWindow::readExtSocket() {
    if (!internalSocket->isWritable()) return;
    qDebug() << "readExtSocket()";
    internalSocket->write(externalSocket->readAll());
}















