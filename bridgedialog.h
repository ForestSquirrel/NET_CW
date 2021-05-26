#ifndef BRIDGEDIALOG_H
#define BRIDGEDIALOG_H

#include <QObject>
#include <QWidget>

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QLineEdit;
QT_END_NAMESPACE

//! [0]
class BridgeDialog : public QDialog
{
    Q_OBJECT

public:
    BridgeDialog(QWidget *parent = nullptr);

    unsigned int Id() const;
    /*unsigned int priority() const;*/
    unsigned int PathCost() const;
    void setFieldText(const unsigned int &x, const unsigned int &y/*,const unsigned int &z*/);

private:
    QLineEdit *IdText;
    /*QLineEdit *PText;*/
    QLineEdit *PCText;
};

#endif // BRIDGEDIALOG_H
