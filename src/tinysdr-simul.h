#ifndef SENDER_H
#define SENDER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
    class QDialogButtonBox;
    class QLabel;
    class QPushButton;
    class QTimer;
    class QUdpSocket;
QT_END_NAMESPACE


class Sender : public QWidget
{
    Q_OBJECT

public:
    Sender(QWidget *parent = 0);

private slots:
    void startBroadcasting();
    void broadcastDatagram();

private:
    QLabel *statusLabel;
    QPushButton *startButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;

    QUdpSocket *udpSocket;
    QTimer *timer;

    char *datagram; // IQ audio data to send

    // Used for tone debugging
    qint64 sinIndex;
    qint16 *sampleBuffer;
};

#endif
