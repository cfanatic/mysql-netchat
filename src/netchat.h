#ifndef NETCHAT_H
#define NETCHAT_H

#ifndef Q_OS_WIN
#include <QtMac>
#endif
#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QScrollBar>
#include <QString>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QWidget>
#include "configuration.h"
#include "database.h"
#include "encryption.h"
#include "options.h"

namespace Ui {
class Netchat;
}

class Netchat : public QWidget
{
    Q_OBJECT

    enum MessageType
    {
        Local,
        Remote,
        Error
    };

public:
    explicit Netchat(QWidget *parent = 0);
    ~Netchat();

private:
    Ui::Netchat *ui;
    Configuration *m_config;
    Database *m_database;
    Encryption *m_encryption;
    Options *m_options;
    QSystemTrayIcon *m_systemTray;
    QTimer *m_polling;
    QTimer *m_typing;
    QScrollBar *m_scrollMessageBar;
    static const QString m_appName;

public slots:

private slots:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    void init();
    void options();
    void sendMessage(const QString message = "");
    void getMessage();
    void setMessage(const MessageType type, QLabel *label);
    void setTyping(const QString text);
    void resetTyping();
    void setNotification(const QString user, const QString text);
    void resetNotification() const;
    void moveScrollBarToBottom(const int min, const int max);
    QString getOsName() const;

signals:
    void execMessage(QString user, QString text);

};

#endif // NETCHAT_H
