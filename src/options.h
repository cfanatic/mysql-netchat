#ifndef OPTIONS_H
#define OPTIONS_H

#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QCheckBox>
#include <QDebug>
#include <QDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QHostInfo>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QVBoxLayout>
#include "configuration.h"

class Options : public QDialog
{
    Q_OBJECT

public:
    explicit Options(QWidget *parent = 0);
    ~Options();

    void load(Configuration * const config);

    QGridLayout *gridLayout;
    QGroupBox *groupDatabase;
    QGridLayout *gridLayout_3;
    QLabel *labelPort;
    QLineEdit *lineUser;
    QLineEdit *linePort;
    QLineEdit *lineAddress;
    QLabel *labelTable;
    QLabel *labelUser;
    QLineEdit *lineTable;
    QLabel *labelAddress;
    QGroupBox *groupEncryption;
    QGridLayout *gridLayout_2;
    QVBoxLayout *layoutEncryption;
    QLabel *labelEncryption;
    QLineEdit *lineEncryption;
    QSpacerItem *spacerEncryption;
    QCheckBox *checkEncryption;
    QSpacerItem *spacerMain2;
    QSpacerItem *spacerMain3;
    QSpacerItem *spacerMain1;
    QPushButton *buttonOptions;

};

#endif // OPTIONS_H
