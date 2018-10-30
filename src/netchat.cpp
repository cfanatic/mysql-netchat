#include "netchat.h"
#include "ui_netchat.h"
#include "iostream"

const QString Netchat::m_appName = "Netchat";

Netchat::Netchat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Netchat),
    m_config(new Configuration(this)),
    m_options(new Options(this)),
    m_polling(new QTimer(this)),
    m_typing(new QTimer(this))
{
    // Initialize main window
    ui->setupUi(this);
    ui->textMessage->setAttribute(Qt::WA_MacShowFocusRect, 0);
    m_scrollMessageBar = ui->scrollMessageLog->verticalScrollBar();
    this->setFocusPolicy(Qt::StrongFocus);

    // Initialize timers for message polling and typing status
    m_polling->setInterval(1000);
    m_typing->setInterval(800);

    // Initialize configuration
    m_options->load(m_config);

    // Initialize database
    m_database = new Database(this, m_config);

    // Initialize encryption
    m_encryption = new Encryption(this, m_config);

#ifdef Q_OS_WIN
    // Configure the notification system for Windows platforms
    m_systemTray = new QSystemTrayIcon(this);
    m_systemTray->setIcon(QIcon(":/res/icon.png"));
    connect(m_systemTray, SIGNAL(messageClicked()), this, SLOT(resetNotification()));
#endif

    // Connect signals with slots
    connect(ui->buttonOptions, SIGNAL(clicked(bool)), this, SLOT(options()));
    connect(ui->textMessage, SIGNAL(returnPressed()), this, SLOT(sendMessage()));
    connect(ui->textMessage, SIGNAL(textEdited(QString)), this, SLOT(setTyping(QString)));
    connect(m_scrollMessageBar, SIGNAL(rangeChanged(int,int)), this, SLOT(moveScrollBarToBottom(int, int)));
    connect(m_polling, SIGNAL(timeout()), this, SLOT(getMessage()));
    connect(m_typing, SIGNAL(timeout()), SLOT(resetTyping()));
    connect(this, SIGNAL(execMessage(QString, QString)), this, SLOT(setNotification(QString, QString)));
    connect(qApp, SIGNAL(focusChanged(QWidget*, QWidget*)), this, SLOT(resetNotification()));

    // Check if configuration file is available and valid
    if (m_config->valid() == true)
    {
        // Connect to database and calculate encryption data
        this->init();
    }
    else
    {
        // Show options window
        this->options();
    }
}

Netchat::~Netchat()
{
    delete ui;
}

void Netchat::init()
{
    try
    {
        // Open database connection
        bool success;
        m_database->init();
        success = m_database->open();

        // Calculate encryption data and begin to pull messages
        if (success == true)
        {
            m_encryption->init();
            m_encryption->prepare();
            m_polling->start();
        }
    }
    catch (std::exception& e)
    {
        QLabel *text = new QLabel(QString("\nEncryption Initialization Error:\n%1!\n").arg(e.what()), this);
        setMessage(Error, text);
    }
    catch (QString error)
    {
        m_database->close();
        QLabel *text = new QLabel(QString("\n%1\n").arg(error), this);
        setMessage(Error, text);
    }
}

void Netchat::options()
{
    // Show options window
    if (m_options->exec() == QDialog::Accepted)
    {
        // Load configuration parameters
        m_options->load(m_config);

        // Close database connection and stop pulling messages
        m_database->close();
        m_polling->stop();

        // Save configuration file
        m_config->save();

        // Open database connection and calculate encryption data
        this->init();
    }
}

void Netchat::sendMessage(const QString message)
{
    const quint8 enc = true;
    try
    {
        quint8 aux = false;
        if (m_database->m_db.isOpen() == true)
        {
            if (!ui->textMessage->text().isEmpty() || !message.isEmpty())
            {
                // Set message text according to current action use case
                QString text;
                if (message.isEmpty())
                {
                    // Use workaround for carriage return bug
                    text = ui->textMessage->text();
                    ui->textMessage->clear();
                }
                else if (message == "TYPING")
                {
                    aux = 1;
                    text = QString("<TYPING>");
                }
                else if (message == "NOT_TYPING")
                {
                    aux = 1;
                    text = QString("<NOT_TYPING>");
                }

                // Encrypt text message using AES-256 and send MySQL query
                text = m_encryption->encrypt(text);
                m_database->sendQuery(m_encryption->m_saltData, text, aux, enc);
            }
        }
        else
        {
            throw QString("MySQL Error:\nMySQL connection has not been established!");
        }
    }
    catch (std::exception& e)
    {
        QLabel *text = new QLabel(QString("\nEncryption Error:\n%1!\n").arg(e.what()), this);
        setMessage(Error, text);
    }
    catch (QString error)
    {
        QLabel *text = new QLabel(QString("\n%1\n").arg(error), this);
        setMessage(Error, text);
    }
}

void Netchat::getMessage()
{
    if (m_database->m_db.isOpen() == true)
    {
        try
        {
            QVector<Database::Message> response = m_database->getQuery();
            for (QVector<Database::Message>::iterator it = response.begin(); it != response.end(); ++it)
            {
                // Get message data
                Database::Message message = *it;

                // Extract user name
                QString user;
                user = message.name;

                // Extract salt size and salt data
                Botan::SecureVector<quint8> salt;
                QStringList saltData;
                quint8 saltSize;
                saltSize = message.salt.mid(0, 2).toInt();
                saltData = message.salt.mid(2, saltSize).split('.');

                // Convert vector containing numbers from data type string to int
                for (QStringList::iterator it = saltData.begin(); it != saltData.end(); ++it)
                    salt.push_back((*it).toInt());

                // Generate a new symmetric key for the user
                if (m_encryption->m_aes256_keys.contains(salt) == false)
                {
                    m_encryption->key(salt);
                }

                // Decrypt text message using AES-256
                QString text;
                if (message.enc == true)
                {
                    text = m_encryption->decrypt(salt, message.msg);
                }
                else
                {
                    text = message.msg;
                }

                if (message.aux == false)
                {
                    qDebug() << "N::getMessage():\t" << user << saltSize << salt << message.msg << endl << "\t\t" << text;
                }

                // Update window title according to the typing status, otherwise print the text message
                if (text == "<TYPING>")
                {
                    if (user != m_database->m_user)
                    {
                        this->setWindowTitle(m_appName + " - " + user + " typing...");
                    }
                }
                else if (text == "<NOT_TYPING>")
                {
                    if (user != m_database->m_user)
                    {
                        this->setWindowTitle(m_appName);
                    }
                }
                else
                {
                    // Set mouse tooltip information for the text message
                    QLabel *label = new QLabel(QString("  %1  ").arg(text), this);
                    QString msg = (message.msg.length() < 45) ? message.msg : message.msg.left(42) + "...";
                    label->setToolTip("[" + message.date.toString("dd.MM.yy - hh:mm") + "]  " + user + "  #" + QString::number(message.id) + "\n\n" + msg);
                    label->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);

    #if defined(Q_OS_UNIX)
                    QFont font("Helvetica", 13);
    #elif defined(Q_OS_WIN)
                    QFont font("MS Shell Dlg 2", 8);
    #endif
                    QFontMetrics fontText(font);
                    qint32 lengthText = fontText.width(label->text());

                    // Word wrap if text message width exceeds 75% of the message log window width
                    if ((lengthText + 20) > (0.75 * ui->scrollMessageLog->width()))
                    {
                        const quint16 blockSize = 50;
                        QString blockText = label->text();
                        for (quint16 i = blockSize; i < blockText.length(); i += blockSize)
                        {
                            // TODO: This needs to be fixed, because it crashes if there is only one word which is too long!
                            while (blockText[i] != ' ')
                                i--;
                            blockText.insert(i, " \n ");
                        }
                        label->setText(blockText);
                    }

                    // Set text message style depending on whether the message was sent from the host user or not
                    if (user == m_database->m_user)
                    {
                        setMessage(Local, label);
                    }
                    else
                    { 
                        setMessage(Remote, label);
                    }

                    // Set vertical scroll bar to the bottom
                    ui->scrollMessageLog->verticalScrollBar()->setValue(ui->scrollMessageLog->verticalScrollBar()->value() + 10);

                    // Use host computer notification system that a new message has been received
                    emit execMessage(user, text);
                }
            }
        }
        catch (std::exception& e)
        {
            QLabel *text = new QLabel(QString("\nDecryption Error:\n%1!\n").arg(e.what()), this);
            setMessage(Error, text);
        }
        catch (QString error)
        {
            m_database->close();
            QLabel *text = new QLabel(QString("\n%1\n").arg(error), this);
            setMessage(Error, text);
        }
    }
}

void Netchat::setMessage(const MessageType type, QLabel *label)
{
    switch(type)
    {
        case Local:
        {
            // Print text message as a grey bubble on the left window side
            label->setStyleSheet("border-radius: 6px; background-color: #E5E5EA; color: black;");
            label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            ui->layoutMessageLog->addWidget(label);
            ui->layoutMessageLog->setAlignment(label, Qt::AlignLeft);
            break;
        }
        case Remote:
        {
            // Print text message as a blue bubble on the right window side
            label->setStyleSheet("border-radius: 6px; background-color: #0B93F6; color: white;");
            label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            ui->layoutMessageLog->addWidget(label);
            ui->layoutMessageLog->setAlignment(label, Qt::AlignRight);
            break;
        }
        case Error:
        {
            // Print text message as a red bubble in the window center
            label->setStyleSheet("border-radius: 6px; background-color: #cc0000; color: white;");
            label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            label->setWordWrap(true);
            ui->layoutMessageLog->addWidget(label);
            ui->layoutMessageLog->setAlignment(label, Qt::AlignCenter);
            break;
        }
    }
}

void Netchat::setTyping(const QString text)
{
    Q_UNUSED(text);

    // Update typing status since host user started typing
    if (m_database->m_db.isOpen() == true)
    {
        if (!m_typing->isActive())
        {
            sendMessage("TYPING");
        }
        // Typing status will be reset if host user has not been typing for more than <interval>
        m_typing->start();
    }
}

void Netchat::resetTyping()
{
    // Update typing status since host user stopped typing
    m_typing->stop();
    sendMessage("NOT_TYPING");
}

void Netchat::setNotification(const QString user, const QString text)
{
    // Set desktop notification for macOS/Windows which shows that a new message has been received
#if defined(Q_OS_UNIX)
    Q_UNUSED(user);
    Q_UNUSED(text);
    if (m_database->m_db.isOpen() == true)
    {
        if (this->isActiveWindow() == true)
        {
            QtMac::setBadgeLabelText("");
        }
        else
        {
            quint16 msgCounter;
            msgCounter = QtMac::badgeLabelText().toInt();
            msgCounter++;
            QtMac::setBadgeLabelText(QString::number(msgCounter));
        }
    }
#elif defined(Q_OS_WIN)
    if (m_database->m_db.isOpen() == true)
    {
        if (this->isActiveWindow() == true)
        {
            // TODO: Implement code. Message box shall be deactivated once a new message arrives.
        }
        else
        {
            m_systemTray->setVisible(true);
            m_systemTray->showMessage("<" + user + ">", text);
        }
    }
#endif
}

void Netchat::resetNotification() const
{
    // Disable desktop notification if either the notification message was clicked or the application focus was reset
#if defined(Q_OS_UNIX)
    if (m_database->m_db.isOpen() == true)
    {
        if (this->isActiveWindow() == true)
        {
            QtMac::setBadgeLabelText("");
        }
    }
#elif defined(Q_OS_WIN)
    if (m_database->m_db.isOpen() == true)
    {
        m_systemTray->setVisible(false);
    }
#endif
}

void Netchat::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        // Close application if escape button is pressed
        case Qt::Key_Escape:
        {
            while (m_typing->remainingTime() > 0);
            qApp->quit();
        }
        // Set focus to the text field if carriage return is pressed
        case Qt::Key_Return:
        {
            ui->textMessage->setFocus();
        }
    }
}

void Netchat::mousePressEvent(QMouseEvent *event)
{
    // Set focus to the text field if application window is pressed by the mouse
    event->button();
    ui->textMessage->setFocus();
}

void Netchat::moveScrollBarToBottom(const int min, const int max)
{
    Q_UNUSED(min);

    // Scroll to bottom window when a new message has been received
    ui->scrollMessageLog->verticalScrollBar()->setValue(max);
}

QString Netchat::getOsName() const
{
    // Determine the operating system of the host computer
#if defined(Q_OS_ANDROID)
    return QLatin1String("android");
#elif defined(Q_OS_BLACKBERRY)
    return QLatin1String("blackberry");
#elif defined(Q_OS_IOS)
    return QLatin1String("ios");
#elif defined(Q_OS_MACOS)
    return QLatin1String("macos");
#elif defined(Q_OS_TVOS)
    return QLatin1String("tvos");
#elif defined(Q_OS_WATCHOS)
    return QLatin1String("watchos");
#elif defined(Q_OS_WINCE)
    return QLatin1String("wince");
#elif defined(Q_OS_WIN)
    return QLatin1String("windows");
#elif defined(Q_OS_LINUX)
    return QLatin1String("linux");
#elif defined(Q_OS_UNIX)
    return QLatin1String("unix");
#else
    return QLatin1String("unknown");
#endif
}
