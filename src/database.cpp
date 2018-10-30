#include "database.h"

Database::Database(QObject *parent, Configuration *config) :
    m_config(config),
    QObject(parent)
{
    // Initialize database
    this->init();
}

Database::~Database()
{
}

void Database::init()
{
    // Set configuration parameters
    m_name = m_config->get(Configuration::NAME);
    m_address = m_config->get(Configuration::ADDRESS);
    m_port = m_config->get(Configuration::PORT);
    m_table = m_config->get(Configuration::TABLE);
    m_login = m_config->get(Configuration::LOGIN);
    m_password = m_config->get(Configuration::PASSWORD);
    m_user = m_config->get(Configuration::USER);
}

bool Database::open()
{
    // Open database connection
    m_db = QSqlDatabase::addDatabase("QMYSQL");
    m_db.setHostName(m_address);
    m_db.setPort(m_port.toInt());
    m_db.setDatabaseName(m_name);
    m_db.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=3");
    bool result = m_db.open(m_login, m_password);

    // Create database table if unavailable on the server
    if (result == true)
    {
        this->table();
    }
    else
    {
        throw QString("MySQL Error:\n" + m_db.lastError().databaseText());
    }

    return result;
}

void Database::close()
{
    // Close database connection
    if (m_db.isOpen() == true)
    {
        QString db = m_db.connectionName();
        m_db.close();
        m_db = QSqlDatabase();
        m_db.removeDatabase(db);
    }
}

bool Database::table()
{
    // Create database table if unavailable on the server
    QSqlQuery query;
    query.prepare(QString("CREATE TABLE IF NOT EXISTS %1 (\
                                id INT NOT NULL AUTO_INCREMENT,\
                                name TEXT NOT NULL,\
                                date DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,\
                                salt TEXT NOT NULL,\
                                msg TEXT NOT NULL,\
                                read_local TINYINT UNSIGNED NOT NULL,\
                                read_remote TINYINT UNSIGNED NOT NULL,\
                                aux TINYINT UNSIGNED NOT NULL,\
                                enc TINYINT UNSIGNED NOT NULL,\
                                PRIMARY KEY (id))").arg(m_table));

    bool success = false;

    if (query.exec() == true)
    {
        success = true;
    }
    else
    {
        throw QString("MySQL Error:\n" + query.lastError().databaseText());
    }

    return success;
}

bool Database::sendQuery(const QByteArray salt, const QString text, const quint8 aux, const quint8 enc)
{
    // Insert new message into the database
    QSqlQuery query;
    query.prepare(QString("INSERT INTO %1 (name, date, salt, msg, aux, enc) VALUES (:name, :time, :salt, :msg, :aux, :enc)").arg(m_table));
    query.bindValue(":name", m_user);
    query.bindValue(":time", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    query.bindValue(":salt", salt);
    query.bindValue(":msg", text);
    query.bindValue(":aux", aux);
    query.bindValue(":enc", enc);

    bool success = false;

    if (query.exec() == true)
    {
        success = true;
    }
    else
    {
        throw QString("MySQL Error:\n" + query.lastError().databaseText());
    }

    return success;
}

bool Database::updateQuery(const quint32 id, const QString name)
{
    // Update read status for a specific message in the database
    QSqlQuery query;
    if (name == m_user)
    {
        query.prepare(QString("UPDATE %1 SET read_local = 1 WHERE id = (:id)").arg(m_table));
    }
    else
    {
        query.prepare(QString("UPDATE %1 SET read_remote = 1 WHERE id = (:id)").arg(m_table));
    }
    query.bindValue(":id", id);

    bool success = false;

    if (query.exec() == true)
    {
        success = true;
    }
    else
    {
        throw QString("MySQL Error:\n" + query.lastError().databaseText());
    }

    return success;
}

QVector<Database::Message> Database::getQuery()
{
    // Get all unread messages from the database
    QVector<Message> response;
    QSqlQuery query;
    query.prepare(QString("SELECT * FROM %1 WHERE (read_local = (:readstatus) AND name = (:user)) OR (read_remote = (:readstatus) AND name <> (:user))").arg(m_table));
    query.bindValue(":readstatus", 0);
    query.bindValue(":user", m_user);

    if (query.exec() == true)
    {
        while (query.next())
        {
            // Save unread message in custom data format
            Message message;
            message.id = query.value("id").toInt();
            message.name = query.value("name").toString();
            message.date = QDateTime::fromString(query.value("date").toString().replace("T", " "), "yyyy-MM-dd hh:mm:ss");
            message.salt = query.value("salt").toString();
            message.msg = query.value("msg").toString();
            message.aux = query.value("aux").toBool();
            message.enc = query.value("enc").toBool();
            response.push_back(message);

            // Update read status for the message in the database
            updateQuery(message.id, message.name);
        }
    }
    else
    {
        throw QString("MySQL Error:\n" + query.lastError().databaseText());
    }

    return response;
}
