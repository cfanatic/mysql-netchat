#ifndef DATABASE_H
#define DATABASE_H

#include <QDateTime>
#include <QDebug>
#include <QLineEdit>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QString>
#include <QVector>
#include "configuration.h"

class Database : public QObject
{
    Q_OBJECT

public:
    struct Message
    {
        quint32 id;
        QString name;
        QDateTime date;
        QString salt;
        QString msg;
        quint8 aux;
        quint8 enc;
    };

    explicit Database(QObject *parent, Configuration *config);
    ~Database();

    void init();
    bool open();
    void close();
    bool table();
    bool sendQuery(const QByteArray salt, const QString text, const quint8 aux, const quint8 enc);
    bool updateQuery(const quint32 id, const QString name);
    QVector<Database::Message> getQuery();

    QSqlDatabase m_db;
    QString m_name;
    QString m_address;
    QString m_port;
    QString m_table;
    QString m_user;
    QString m_login;
    QString m_password;

private:
    Configuration *m_config;

public slots:

private slots:

signals:

};

#endif // DATABASE_H
