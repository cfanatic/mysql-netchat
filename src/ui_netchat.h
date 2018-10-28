/********************************************************************************
** Form generated from reading UI file 'netchat.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NETCHAT_H
#define UI_NETCHAT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Netchat
{
public:
    QGridLayout *gridLayout;
    QScrollArea *scrollMessageLog;
    QWidget *scrollAreaWidgetContents;
    QHBoxLayout *horizontalLayout;
    QLineEdit *textMessage;
    QPushButton *buttonOptions;
    QGridLayout *layoutMessageLog;

    void setupUi(QWidget *Netchat)
    {
        if (Netchat->objectName().isEmpty())
            Netchat->setObjectName(QStringLiteral("Netchat"));
        Netchat->resize(584, 354);
        gridLayout = new QGridLayout(Netchat);
        gridLayout->setSpacing(5);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(10, 10, 10, 0);

        // Manual implementation
        scrollMessageLog = new QScrollArea();
        layoutMessageLog = new QGridLayout();
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setStyleSheet("background-color: white");
        scrollAreaWidgetContents->setLayout(layoutMessageLog);
        layoutMessageLog->setAlignment(Qt::AlignTop);
        scrollMessageLog->setWidgetResizable(true);
        scrollMessageLog->setGeometry(10, 10, 480, 170);
        scrollMessageLog->setWidget(scrollAreaWidgetContents);
        //scrollMessageLog->setStyleSheet("border-radius: 5px");

        gridLayout->addWidget(scrollMessageLog, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(10);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetFixedSize);
        horizontalLayout->setContentsMargins(0, 0, 0, 10);
        textMessage = new QLineEdit(Netchat);
        textMessage->setObjectName(QStringLiteral("textMessage"));
        textMessage->setStyleSheet(QLatin1String("border-radius: 8px; padding: 3px; border: 1px solid grey;"));

        horizontalLayout->addWidget(textMessage);

        buttonOptions = new QPushButton(Netchat);
        buttonOptions->setObjectName(QStringLiteral("buttonOptions"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(buttonOptions->sizePolicy().hasHeightForWidth());
        buttonOptions->setSizePolicy(sizePolicy);
        buttonOptions->setStyleSheet(QStringLiteral(""));
        buttonOptions->setAutoDefault(false);
        buttonOptions->setFlat(false);

        horizontalLayout->addWidget(buttonOptions);

        horizontalLayout->setStretch(0, 1);

        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);

        QWidget::setTabOrder(textMessage, buttonOptions);
        QWidget::setTabOrder(buttonOptions, scrollMessageLog);

        retranslateUi(Netchat);

        buttonOptions->setDefault(false);


        QMetaObject::connectSlotsByName(Netchat);
    } // setupUi

    void retranslateUi(QWidget *Netchat)
    {
        Netchat->setWindowTitle(QApplication::translate("Netchat", "Netchat", 0));
        buttonOptions->setText(QApplication::translate("Netchat", "Options", 0));
    } // retranslateUi

};

namespace Ui {
    class Netchat: public Ui_Netchat {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NETCHAT_H
