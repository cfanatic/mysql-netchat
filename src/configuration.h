#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QDebug>
#include <QFileInfo>
#include <QSettings>
#include <QStandardPaths>

class Configuration : public QObject
{
    Q_OBJECT

public:
    struct Settings
    {
        QString db_name;
        QString db_address;
        QString db_port;
        QString db_table;
        QString db_login;
        QString db_password;
        QString enc_user;
        QString enc_key;
    };

    enum Fields
    {
        NAME,
        ADDRESS,
        PORT,
        TABLE,
        LOGIN,
        PASSWORD,
        USER,
        KEY
    };

    explicit Configuration(QObject *parent = 0);
    ~Configuration();

private:
    QSettings *m_settings;
    Settings m_parameters;
    QString m_file;

public slots:
    void load(Settings &config);
    void load();
    void save();
    bool exist();
    bool valid();
    QString get(const Fields &parameter);
    void set(const Fields &parameter, QString value);
    void debug();

private slots:

signals:

};

#endif // SETTINGS_H
