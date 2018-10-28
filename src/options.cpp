#include "options.h"

/*
 *   !!! WARNING !!!
 *   ---------------
 *   It is not recommended to use this pre-defined encryption key!
 *   Change the key within the application, before you start texting.
 *
*/

#define ENCRYPTION_KEY  "!change-this-key!"

Options::Options(QWidget *parent) :
    QDialog(parent)
{
    // Initialize options window
    this->resize(300, 355);
    gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(6);
    gridLayout->setContentsMargins(11, 11, 11, 11);
    gridLayout->setObjectName(QStringLiteral("gridLayout"));
    gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
    groupDatabase = new QGroupBox(this);
    groupDatabase->setObjectName(QStringLiteral("groupDatabase"));
    gridLayout_3 = new QGridLayout(groupDatabase);
    gridLayout_3->setSpacing(6);
    gridLayout_3->setContentsMargins(11, 11, 11, 11);
    gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
    labelPort = new QLabel(groupDatabase);
    labelPort->setObjectName(QStringLiteral("labelPort"));
    gridLayout_3->addWidget(labelPort, 0, 1, 1, 1);
    lineUser = new QLineEdit(groupDatabase);
    lineUser->setObjectName(QStringLiteral("lineUser"));
    gridLayout_3->addWidget(lineUser, 3, 1, 1, 1);
    linePort = new QLineEdit(groupDatabase);
    linePort->setObjectName(QStringLiteral("linePort"));
    gridLayout_3->addWidget(linePort, 1, 1, 1, 1);
    lineAddress = new QLineEdit(groupDatabase);
    lineAddress->setObjectName(QStringLiteral("lineAddress"));
    gridLayout_3->addWidget(lineAddress, 1, 0, 1, 1);
    labelTable = new QLabel(groupDatabase);
    labelTable->setObjectName(QStringLiteral("labelTable"));
    gridLayout_3->addWidget(labelTable, 2, 0, 1, 1);
    labelUser = new QLabel(groupDatabase);
    labelUser->setObjectName(QStringLiteral("labelUser"));
    gridLayout_3->addWidget(labelUser, 2, 1, 1, 1);
    lineTable = new QLineEdit(groupDatabase);
    lineTable->setObjectName(QStringLiteral("lineTable"));
    gridLayout_3->addWidget(lineTable, 3, 0, 1, 1);
    labelAddress = new QLabel(groupDatabase);
    labelAddress->setObjectName(QStringLiteral("labelAddress"));
    gridLayout_3->addWidget(labelAddress, 0, 0, 1, 1);
    gridLayout->addWidget(groupDatabase, 3, 0, 1, 1);
    groupEncryption = new QGroupBox(this);
    groupEncryption->setObjectName(QStringLiteral("groupEncryption"));
    gridLayout_2 = new QGridLayout(groupEncryption);
    gridLayout_2->setSpacing(6);
    gridLayout_2->setContentsMargins(11, 11, 11, 11);
    gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
    layoutEncryption = new QVBoxLayout();
    layoutEncryption->setSpacing(6);
    layoutEncryption->setObjectName(QStringLiteral("layoutEncryption"));
    labelEncryption = new QLabel(groupEncryption);
    labelEncryption->setObjectName(QStringLiteral("labelEncryption"));
    layoutEncryption->addWidget(labelEncryption);
    lineEncryption = new QLineEdit(groupEncryption);
    lineEncryption->setObjectName(QStringLiteral("lineEncryption"));
    layoutEncryption->addWidget(lineEncryption);
    spacerEncryption = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    layoutEncryption->addItem(spacerEncryption);
    checkEncryption = new QCheckBox(groupEncryption);
    checkEncryption->setObjectName(QStringLiteral("checkEncryption"));
    checkEncryption->setEnabled(false);
    layoutEncryption->addWidget(checkEncryption);
    gridLayout_2->addLayout(layoutEncryption, 0, 0, 1, 1);
    gridLayout->addWidget(groupEncryption, 1, 0, 1, 1);
    spacerMain2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    gridLayout->addItem(spacerMain2, 2, 0, 1, 1);
    spacerMain3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    gridLayout->addItem(spacerMain3, 4, 0, 1, 1);
    spacerMain1 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    gridLayout->addItem(spacerMain1, 0, 0, 1, 1);
    buttonOptions = new QPushButton(this);
    buttonOptions->setObjectName(QStringLiteral("buttonOptions"));
    gridLayout->addWidget(buttonOptions, 5, 0, 1, 1);

    // Set default values for widget items
    this->setWindowTitle(QApplication::translate("Dialog", "Options", 0));
    groupDatabase->setTitle(QApplication::translate("Dialog", "MySQL", 0));
    labelPort->setText(QApplication::translate("Dialog", "Port:", 0));
    lineUser->setText(QApplication::translate("Dialog", "n/a", 0));
    linePort->setText(QApplication::translate("Dialog", "n/a", 0));
    lineAddress->setText(QApplication::translate("Dialog", "n/a", 0));
    labelTable->setText(QApplication::translate("Dialog", "Table:", 0));
    labelUser->setText(QApplication::translate("Dialog", "User:", 0));
    lineTable->setText(QApplication::translate("Dialog", "n/a", 0));
    labelAddress->setText(QApplication::translate("Dialog", "Address:", 0));
    groupEncryption->setTitle(QApplication::translate("Dialog", "Encryption", 0));
    labelEncryption->setText(QApplication::translate("Dialog", "AES-256 Key:", 0));
    lineEncryption->setText(QApplication::translate("Dialog", ENCRYPTION_KEY, 0));
    checkEncryption->setText(QApplication::translate("Dialog", "Disable", 0));
    buttonOptions->setText(QApplication::translate("Dialog", "Open Connection", 0));

    // Set tab order for widget items
    QWidget::setTabOrder(lineEncryption, checkEncryption);
    QWidget::setTabOrder(checkEncryption, lineAddress);
    QWidget::setTabOrder(lineAddress, linePort);
    QWidget::setTabOrder(linePort, lineTable);
    QWidget::setTabOrder(lineTable, lineUser);
    QWidget::setTabOrder(lineUser, buttonOptions);

    // Set user name to hostname of the host computer
    lineUser->setText(QHostInfo::localHostName());

    connect(buttonOptions, SIGNAL(clicked()), this, SLOT(accept()));
}

Options::~Options()
{

}

void Options::load(Configuration * const config)
{
    if (config->valid() == true)
    {
        linePort->setText(config->get(Configuration::PORT));
        lineAddress->setText(config->get(Configuration::ADDRESS));
        lineTable->setText(config->get(Configuration::TABLE));
        config->set(Configuration::USER, lineUser->text());
        config->set(Configuration::KEY, lineEncryption->text());
    }
}
