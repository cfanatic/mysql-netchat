#include "configuration.h"

Configuration::Configuration(QObject *parent) : QObject(parent)
{
#if defined(Q_OS_UNIX)
    QString path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + QString("/Library/Netchat/config.ini");
#elif defined(Q_OS_WIN)
    // TODO: Implement function call for Windows
#endif
    m_settings = new QSettings(path, QSettings::IniFormat);
    this->load();
}

Configuration::~Configuration()
{
    delete m_settings;
}

void Configuration::load(Configuration::Settings &config)
{
    config.db_name = m_settings->value("Database/name").toString();
    config.db_address = m_settings->value("Database/address").toString();
    config.db_port = m_settings->value("Database/port").toString();
    config.db_table = m_settings->value("Database/table").toString();
    config.db_login = m_settings->value("Database/login").toString();
    config.db_password = m_settings->value("Database/password").toString();
    config.enc_user = "";
    config.enc_key = "";
    m_parameters = config;
}

void Configuration::load()
{
    load(m_parameters);
}

void Configuration::save()
{
    m_settings->setValue("Database/name", m_parameters.db_name);
    m_settings->setValue("Database/address", m_parameters.db_address);
    m_settings->setValue("Database/port", m_parameters.db_port);
    m_settings->setValue("Database/table", m_parameters.db_table);
    m_settings->setValue("Database/login", m_parameters.db_login);
    m_settings->setValue("Database/password", m_parameters.db_password);
}

bool Configuration::exist()
{
    return QFileInfo::exists(m_settings->fileName()) && QFileInfo(m_settings->fileName()).isFile();
}

bool Configuration::valid()
{
    bool check = m_parameters.db_name.isEmpty() || m_parameters.db_address.isEmpty() ||
                   m_parameters.db_port.isEmpty() || m_parameters.db_table.isEmpty() ||
                   m_parameters.db_login.isEmpty() || m_parameters.db_password.isEmpty();

    return !check;
}

QString Configuration::get(const Fields &parameter)
{
    QString value;

    switch (parameter)
    {
        case NAME: value = m_parameters.db_name; break;
        case ADDRESS: value = m_parameters.db_address; break;
        case PORT: value = m_parameters.db_port; break;
        case TABLE: value = m_parameters.db_table; break;
        case LOGIN: value = m_parameters.db_login; break;
        case PASSWORD: value = m_parameters.db_password; break;
        case USER: value = m_parameters.enc_user; break;
        case KEY: value = m_parameters.enc_key; break;
    }

    return value;
}

void Configuration::set(const Fields &parameter, QString value)
{
    switch (parameter)
    {
        case NAME: m_parameters.db_name = value; break;
        case ADDRESS: m_parameters.db_address = value; break;
        case PORT: m_parameters.db_port = value; break;
        case TABLE: m_parameters.db_table = value; break;
        case LOGIN: m_parameters.db_login = value; break;
        case PASSWORD: m_parameters.db_password = value; break;
        case USER: m_parameters.enc_user = value; break;
        case KEY: m_parameters.enc_key = value; break;
    }
}

void Configuration::debug()
{
    qDebug() << "db_name:\t\t" << m_parameters.db_name;
    qDebug() << "db_address:\t\t" << m_parameters.db_address;
    qDebug() << "db_port:\t\t" << m_parameters.db_port;
    qDebug() << "db_table:\t\t" << m_parameters.db_table;
    qDebug() << "db_login:\t\t" << m_parameters.db_login;
    qDebug() << "db_password:\t" << m_parameters.db_password;
    qDebug() << "enc_user:\t\t" << m_parameters.enc_user;
    qDebug() << "enc_key:\t\t" << m_parameters.enc_key;
}
