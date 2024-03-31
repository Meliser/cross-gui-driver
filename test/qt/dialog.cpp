#include "dialog.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

CustomDialog::CustomDialog(QWidget *parent) : QDialog(parent)
{
    QVBoxLayout *vbox = new QVBoxLayout(this);
    QLabel* label = new QLabel("Hello");
    vbox->addWidget(label);
    QPushButton* button = new QPushButton("PUsh me");
    button->setObjectName("btn1");
    vbox->addWidget(button);
}
