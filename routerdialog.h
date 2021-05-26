#ifndef ROUTERDIALOG_H
#define ROUTERDIALOG_H

#include <QObject>
#include <QWidget>

#include <QDialog>

#include <QVector>
#include <QString>
#include "BPDU.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QLineEdit;
class QListWidget;
QT_END_NAMESPACE

//! [0]
class RouterDialog : public QDialog
{
    Q_OBJECT

public:
    RouterDialog(QWidget *parent = nullptr);

    unsigned int Id() const;
    unsigned int p() const;
    void setFieldText(const unsigned int &Id, const unsigned int &p, QVector<BPDU> logVec);

private:
    QLineEdit *IdText;
    QLineEdit *Ptext;
    QListWidget *log;
};
#endif // ROUTERDIALOG_H
