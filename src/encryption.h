#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <QDebug>
#include <QLineEdit>
#include <QMap>
#include <QObject>
#include "botan/init.h"
#include "botan/auto_rng.h"
#include "botan/pbkdf.h"
#include "botan/aes.h"
#include "botan/pipe.h"
#include "botan/filters.h"
#include "configuration.h"

class Encryption : public QWidget
{
    Q_OBJECT

public:
    explicit Encryption(QWidget *parent, Configuration *config);
    ~Encryption();

    QString m_user;
    QString m_key;
    Botan::PBKDF *m_pbkdf;
    Botan::SecureVector<quint8> m_salt;
    QByteArray m_saltData;
    QMap<Botan::SecureVector<quint8>, Botan::SymmetricKey> m_aes256_keys;

private:
    Configuration *m_config;

public slots:
    void init();
    void prepare();
    void key(const Botan::SecureVector<quint8> salt);
    QString encrypt(const QString text);
    QString decrypt(const Botan::SecureVector<quint8> salt, const QString text);

private slots:

signals:

};

#endif // ENCRYPTION_H
