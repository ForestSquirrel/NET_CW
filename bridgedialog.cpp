#include "bridgedialog.h"

#include <QtWidgets>

//! [0]
BridgeDialog::BridgeDialog(QWidget *parent)
    : QDialog(parent),
      IdText(new QLineEdit),
      /*PText(new QLineEdit),*/
      PCText(new QLineEdit)
{
    auto IdLabel = new QLabel(tr("Bridge ID"));
    /*auto PLabel = new QLabel(tr("Priority"));*/
    auto PCLabel = new QLabel(tr("Path Cost"));
    auto okButton = new QPushButton(tr("OK"));
    auto cancelButton = new QPushButton(tr("Cancel"));

    auto gLayout = new QGridLayout;
    gLayout->setColumnStretch(1, 2);
    gLayout->addWidget(IdLabel, 0, 0);
    gLayout->addWidget(IdText, 0, 1);

   /* gLayout->addWidget(PLabel, 1, 0);
    gLayout->addWidget(PText, 1, 1);*/

    gLayout->addWidget(PCLabel, 2, 0);
    gLayout->addWidget(PCText, 2, 1);

    auto buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    gLayout->addLayout(buttonLayout, 3, 1, Qt::AlignRight);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addLayout(gLayout);
    setLayout(mainLayout);
    connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);

    setWindowTitle(tr("Bridge"));
}
unsigned int BridgeDialog::Id() const{
    return IdText->text().toUInt();
}

/*unsigned int BridgeDialog::priority() const{
    return PText->text().toUInt();
}*/

unsigned int BridgeDialog::PathCost() const{
    return PCText->text().toUInt();
}

void BridgeDialog::setFieldText(const unsigned int &Id, /*const unsigned int &p,*/ const unsigned int &pc){
    IdText->setText(QString::number(Id));
    /*PText->setText(QString::number(p));*/
    PCText->setText(QString::number(pc));
}
