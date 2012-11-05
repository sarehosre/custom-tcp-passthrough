#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    defHost = "109.234.78.19";
    defPort = "10001";
    reachedServer = false;
    loopActEd = false;

    connect(ui->textBr_1, SIGNAL(anchorClicked(QUrl)),
            this, SLOT(processLink(QUrl)));
    connect(ui->textBr_2, SIGNAL(anchorClicked(QUrl)),
            this, SLOT(processLink(QUrl)));
    connect(ui->textBr_3a, SIGNAL(anchorClicked(QUrl)),
            this, SLOT(processLink(QUrl)));
    connect(ui->textBr_3b, SIGNAL(anchorClicked(QUrl)),
            this, SLOT(processLink(QUrl)));


    ui->stackedWidget->setCurrentIndex(0);


    //ui->lineEditHost->setText("109.234.74.22");109.234.78.19
    ui->lineEditHost->setText(defHost);
    ui->lineEditPort->setText(defPort);


    LogFile = new QFile("ctp_log.txt");
    LogFile->open(QIODevice::WriteOnly | QIODevice::Text);

    log("", "started");

}


MainWindow::~MainWindow()
{
    log("", "shutting down");
    if (loopActEd) setupLoopback(false); //false != activate -> false == deactivate
    LogFile->close();
    delete ui;
}




void MainWindow::manageInQuery() {
    log("textBr_3a", "<span>It seems to work.</span>");

    Server->blockSignals(true);
    //disconnect(Server,SIGNAL(newConnection()));
    log("textBr_3b", "a client connected");

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

    log("textBr_3b", "a client disconnected");
    log("textBr_3a", "<span><b>You can now close this application.</b></span>");
    ui->pushB_exit->setText("Exit");
    ui->pushB_exit->setEnabled(true);

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










void MainWindow::on_pushB_startNetAnal_clicked() // :)
{
    ui->pushB_startNetAnal->setEnabled(false);
    ui->pushB_startNetAnal->setText("Testing. Please wait ..");
    ui->centralWidget->setCursor(Qt::BusyCursor);
    log("textBr_0", "Testing connection to CAE version server .. ");
    QTcpSocket* TestSocket = new QTcpSocket();
    connect(TestSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(processNasState(QAbstractSocket::SocketState)));
    connect(TestSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(processNasError(QAbstractSocket::SocketError)));
    TestSocket->connectToHost(QHostAddress(defHost), 10001);

}



void MainWindow::processNasState(QAbstractSocket::SocketState state) {
    QString msg = "";
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(QObject::sender());

    switch(state)
    {
    case QAbstractSocket::UnconnectedState:
        if (socket->error() == -1) {
            msg = "Not connected.";
        } else {
            msg = ("\n\nCould not connect to the server! There propably is a maintenance.\n");
            ui->centralWidget->setCursor(Qt::ArrowCursor);
            ui->pushB_startNetAnal->setEnabled(true);
            ui->pushB_startNetAnal->setText("Start network analysis");
            ui->stackedWidget->setCurrentIndex(1);
        }
        break;

    case QAbstractSocket::HostLookupState: msg = "Looking up host ..";
        break;

    case QAbstractSocket::ConnectingState: msg = "Connecting to server ..";
        break;

    case QAbstractSocket::ConnectedState: msg = "Successfully connected to server!";
        reachedServer = true;
        socket->disconnectFromHost();
        socket->deleteLater();
        ui->centralWidget->setCursor(Qt::ArrowCursor);
        ui->stackedWidget->setCurrentIndex(2);
        break;

    case QAbstractSocket::BoundState: msg = "BoundState: if you see this, something went wrong";
        break;

    case QAbstractSocket::ClosingState: msg = "Closing connection ..";
        break;

    case QAbstractSocket::ListeningState: msg = "ListeningState. if you see this, something went wrong";
        break;

    default: msg = "Trying to connect ..";
    }

    msg.append(QString(reachedServer?" (1)":" (0)"));
    if (socket->error() != -1) msg.append(" | error: " + QString::number(socket->error()) + " = " + socket->errorString() + "\n\n\n");
    log("textBr_0", msg);


}


void MainWindow::processNasError(QAbstractSocket::SocketError error) {
    QString msg = "";
    switch(error) {

    case QAbstractSocket::ConnectionRefusedError: msg= "ConnectionRefused" ;
    break;
    case QAbstractSocket::RemoteHostClosedError: msg= "RemoteHostClosed" ;
    break;
    case QAbstractSocket::HostNotFoundError: msg= "HostNotFound" ;
    break;
    case QAbstractSocket::SocketAccessError: msg= "SocketAccess" ;
    break;
    case QAbstractSocket::SocketResourceError: msg= "SocketResource" ;
    break;

    case QAbstractSocket::SocketTimeoutError: msg= "SocketTimeout" ;
    break;
    case QAbstractSocket::DatagramTooLargeError: msg= "DatagramTooLarge" ;
    break;
    case QAbstractSocket::NetworkError: msg= "Network" ;
    break;
    case QAbstractSocket::AddressInUseError: msg= "AddressInUse" ;
    break;
    case QAbstractSocket::SocketAddressNotAvailableError: msg= "SocketAddressNotAvailable" ;
    break;

    case QAbstractSocket::UnsupportedSocketOperationError: msg= "UnsupportedSocketOperation" ;
    break;
    case QAbstractSocket::ProxyAuthenticationRequiredError: msg= "ProxyAuthenticationRequired" ;
    break;
    case QAbstractSocket::SslHandshakeFailedError: msg= "SslHandshakeFailed" ;
    break;
    case QAbstractSocket::UnfinishedSocketOperationError: msg= "UnfinishedSocketOperation" ;
    break;
    case QAbstractSocket::ProxyConnectionRefusedError: msg= "ProxyConnectionRefused" ;
    break;

    case QAbstractSocket::ProxyConnectionClosedError: msg= "ProxyConnectionClosed" ;
    break;
    case QAbstractSocket::ProxyConnectionTimeoutError: msg= "ProxyConnectionTimeout" ;
    break;
    case QAbstractSocket::ProxyNotFoundError: msg= "ProxyNotFound" ;
    break;
    case QAbstractSocket::ProxyProtocolError: msg= "ProxyProtocol" ;
    break;
    case QAbstractSocket::UnknownSocketError: msg= "UnknownSocket" ;
    break;

    }
    msg.prepend("Error encountered while connecting: ");
}




void MainWindow::processLink(QUrl url) {
    if (url.toString().left(5) == "goto:") {
        ui->stackedWidget->setCurrentIndex(url.toString().remove(0, 5).toInt());
    } else {
        QDesktopServices::openUrl(url);
    }
}






void MainWindow::on_pushB_fixError_clicked()
{
    ui->pushB_fixError->setEnabled(false);
    runLocalServer();
}

void MainWindow::runLocalServer() {
    Server = new QTcpServer(this);
    if (Server->listen(QHostAddress::Any, 10001)) {
        log("textBr_3b", "server now listening on port 10001");
        internalSocket = new QTcpSocket();
        externalSocket = new QTcpSocket();

        connect(Server, SIGNAL(newConnection()), this, SLOT(manageInQuery()));
        connect(internalSocket, SIGNAL(disconnected()), this, SLOT(manageDisco()));

        setupLoopback(true);

    } else {
        log("textBr_3b", "Error: server can't listen on port 10001, probably another programme is already listening on that port.");
        log("textBr_3b", "Try restarting this programme.");
        log("textBr_3b", "If that doesn't help, perform a clean boot and start this programme again.");
    }
}


void MainWindow::setupLoopback(bool install) {


    if (install) {
        // add addr "Loop" 109.234.74.22/32 store=active
        // (/32 means MASK 255.255.255.255)
        log("textBr_3a", "Adding 109.234.74.22 to loopback device.");


        QProcess* SetupProcess = new QProcess(this);
        SetupProcess->start("netsh", QStringList() << "int" << "ip" << "add" << "address" << "\"Loop\"" << "109.234.74.22/32" << "store=active");
        log("textBr_3a", "Running netsh int ip add addr \"Loop\" 109.234.74.22/32 store=active");
        connect(SetupProcess, SIGNAL(finished(int)), this, SLOT(finishLoopbackAct(int)));

    } else {
        // delete address "Loop" addr=109.234.74.22
        log("textBr_3a", "Removing 109.234.74.22 from loopback device.");


        QProcess* SetupProcess = new QProcess(0); // 0 as parent so process will continue running after this prog is closed
        SetupProcess->start("netsh", QStringList() << "int" << "ip" << "delete" << "address" << "\"Loop\"" << "addr=109.234.74.22");
        log("textBr_3a", "Running netsh int ip delete address \"Loop\" addr=109.234.74.22");
        connect(SetupProcess, SIGNAL(finished(int)), this, SLOT(finishLoopbackDeAct(int)));
    }

}

void MainWindow::finishLoopbackAct(int status) {
    log("textBr_3a", "Act: " + QString::number(status));
    loopActEd = true;
    if (status == 0) {
        log("textBr_3a", "<span><b>Try launching CAE now.</b></span>");
    } else {
        log("textBr_3a", "<span><b>Sorry. Could not fix error automatically. Consider reinstalling CAE: <a href=\"http://download.nexoneu.com/cba/fullversion/Combatarms_eu.exe\">download.nexoneu.com/cba/fullversion/Combatarms_eu.exe</a></b></span>");
    }

}
void MainWindow::finishLoopbackDeAct(int status) {
    log("textBr_3a", "DeAct: " + QString::number(status));

}

void MainWindow::on_pushB_exit_clicked()
{
    this->close();
}


void MainWindow::log(QString loc, QString msg) {

    if (loc == "textBr_0") {
        ui->textBr_0->append(msg);

    } else if (loc == "textBr_1") {
        ui->textBr_1->append(msg);

    } else if (loc == "textBr_2") {
        ui->textBr_2->append(msg);

    } else if (loc == "textBr_3a") {
        ui->textBr_3a->append(msg);

    } else if (loc == "textBr_3b") {
        ui->textBr_3b->append(msg);
    }


    QDate date = QDate::currentDate();

    QTime time = QTime::currentTime();


    LogFile->write(QString(date.toString("yyyy-MM-dd") + " " + time.toString("hh-mm-ss") + " " + loc + ": " + msg + "\n").toUtf8());
    qDebug() << QString(date.toString("yyyy-MM-dd") + " " + time.toString("hh-mm-ss") + " " + loc + ": " + msg);
}






