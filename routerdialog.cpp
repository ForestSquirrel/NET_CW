#include "routerdialog.h"

#include <QtWidgets>

//! [0]
RouterDialog::RouterDialog(QWidget *parent)
    : QDialog(parent),
      IdText(new QLineEdit),
      Ptext(new QLineEdit),
      log(new QListWidget)

{
    auto IdLabel = new QLabel(tr("Router ID"));
    auto pLabel = new QLabel(tr("Router Priority"));
    auto okButton = new QPushButton(tr("OK"));
    auto cancelButton = new QPushButton(tr("Cancel"));

    log->scrollBarWidgets(Qt::AlignLeft);
    auto gLayout = new QGridLayout;
    gLayout->setColumnStretch(0, 3);
    gLayout->addWidget(IdLabel, 0, 0);
    gLayout->addWidget(IdText, 0, 1);
    gLayout->addWidget(pLabel, 1, 0);
    gLayout->addWidget(Ptext, 1, 1);
    gLayout->addWidget(okButton, 3, 0);
    gLayout->addWidget(cancelButton,3,1);

    //auto buttonLayout = new QHBoxLayout;
    //buttonLayout->addWidget(okButton);
    //buttonLayout->addWidget(cancelButton);

    //gLayout->addLayout(buttonLayout, 3, 0, Qt::AlignRight);
    log->adjustSize();
    auto mainLayout = new QHBoxLayout;
    mainLayout->setStretch(0, 1);
    mainLayout->addLayout(gLayout, 0);
    mainLayout->addWidget(log, 1, Qt::AlignRight);
    setLayout(mainLayout);
    connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);

    setWindowTitle(tr("Router"));
    this->setFixedWidth(500);
}
unsigned int RouterDialog::Id() const{
    return IdText->text().toUInt();
}
unsigned int RouterDialog::p() const{
    return Ptext->text().toUInt();
}
void RouterDialog::setFieldText(const unsigned int &Id, const unsigned int &p, QVector<BPDU> logVec){
    IdText->setText(QString::number(Id));
    Ptext->setText(QString::number(p));
    log->clear();
    foreach (BPDU item, logVec){
            log->insertItem(0, item.toString());
    }
}
