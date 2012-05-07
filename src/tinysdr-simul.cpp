#include <QtGui>
#include <QtNetwork>
#include <QtCore/qmath.h>
#include <QtCore/qendian.h>

#include "tinysdr-simul.h"

#define EXTIO_SR 48000
#define EXTIO_BLOCKZ 4096
#define EXTIO_BLOCKZSIZE (EXTIO_BLOCKZ*2*sizeof(qint16))

Sender::Sender(QWidget *parent)
    : QWidget(parent)
{
    statusLabel = new QLabel(tr("Ready to broadcast datagrams on port 45454"));
    statusLabel->setWordWrap(true);

    startButton = new QPushButton(tr("&Start"));
    quitButton = new QPushButton(tr("&Quit"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(startButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    timer = new QTimer(this);

    udpSocket = new QUdpSocket(this);

    connect(startButton, SIGNAL(clicked()), this, SLOT(startBroadcasting()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(timer, SIGNAL(timeout()), this, SLOT(broadcastDatagram()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Broadcast Sender"));

    sinIndex = 0;
    datagram = new char[EXTIO_BLOCKZ*2*sizeof(qint16)];
}

void Sender::startBroadcasting() {
    startButton->setEnabled(false);
    timer->start((EXTIO_BLOCKZ*1000)/EXTIO_SR);
}

void Sender::broadcastDatagram()
{
    statusLabel->setText(tr("Now broadcasting datagrams"));

    // Fill the stereo buffer with a simple IQ tone
    qint16 *sampleBuffer = reinterpret_cast<qint16 *>(datagram);
    for(int i=0; i<EXTIO_BLOCKZ*2;) {
        sampleBuffer[i++] = (qint16)(qCos(2*M_PI*0.01*sinIndex)*10000);
        sampleBuffer[i++] = (qint16)(qSin(2*M_PI*0.01*sinIndex)*10000);
        sinIndex++;
    }
    // Send the datas
    udpSocket->writeDatagram(datagram, EXTIO_BLOCKZSIZE ,QHostAddress::Broadcast, 45454);
}
