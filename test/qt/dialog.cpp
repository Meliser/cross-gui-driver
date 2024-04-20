#include "dialog.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QCheckBox>
#include <QTableWidget>
#include <QRadioButton>
#include <QApplication>
#include <QTextStream>

#include <iostream>
#include <qwindowdefs.h>

CustomDialog::CustomDialog(QWidget *parent) : QDialog(parent)
{
    QGridLayout *gridbox = new QGridLayout;
    gridbox->addWidget(setupRadioBox(),0,0);
    gridbox->addWidget(setupCheckBox(),1,0);
    gridbox->addLayout(setupTable(),0,1,-1,-1);

    setLayout(gridbox);
    setWindowTitle(tr("Sample Qt App"));
    resize(480, 320);
}

void CustomDialog::walkTree()
{
    static unsigned level_ctr[100] = {};
    static unsigned preset_name_ctr = 0;
    const QString root_prefix = "auto_";
    for (QWidget *widget : QApplication::allWidgets()) {
        QWidget* top = widget;
        for (QWidget* parent = widget->parentWidget(); parent; parent = parent->parentWidget()) {
            top = parent;
        }

        std::cout << "Take top widget with name " << top->objectName().toStdString() << std::endl;

        int level = 0;
        QWidgetList wl = { top };
        while (true) {
            QWidgetList ch_wl;
            for (QWidget* w : wl) {
                if (w->objectName().isEmpty()) {
                    QString name;
                    QTextStream strm(&name);
                    QString name_prefix = w->parentWidget() ? (w->parentWidget()->objectName() + '.') : root_prefix;
                    unsigned ctr = name_prefix.startsWith(root_prefix) ? level_ctr[level]++ : preset_name_ctr++;
                    //std::cout << "level " << level << " parent " << w->parentWidget() << std::endl;
                    strm << name_prefix << ctr;
                    w->setObjectName(name);
                    w->setToolTip(name);
                    std::cout << "Set object name to " << name.toStdString() << " for w " << w << std::endl;
                }
                else if (w->toolTip().isEmpty()) {
                    w->setToolTip(w->objectName());
                }
                else if (w->objectName().startsWith(root_prefix)) {
                    break;
                }

                for (QObject* ch_obj : w->children()) {
                    if (QWidget* ch_w = dynamic_cast<QWidget*>(ch_obj)) {
                        ch_wl.append(ch_w);
                        //assert(ch_w->parentWidget() == w);
                        
                        // std::cout << "Append child " << ch_w << " for parent " << w 
                        //     << " child name " << ch_w->objectName().toStdString() << " parent name " << w->objectName().toStdString() << std::endl;
                    }
                }
            }
            if (ch_wl.isEmpty()) {
                break;
            }
            wl = std::move(ch_wl);
            level++;
        }
    }
}

QGroupBox* CustomDialog::setupRadioBox()
{
    QGroupBox *groupBox = new QGroupBox(tr("Exclusive Radio Buttons"), this);

    QRadioButton *radio1 = new QRadioButton(tr("&Radio button 1"), this);
    QRadioButton *radio2 = new QRadioButton(tr("R&adio button 2"), this);
    QRadioButton *radio3 = new QRadioButton(tr("Ra&dio button 3"), this);

    radio1->setChecked(true);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(radio1);
    vbox->addWidget(radio2);
    vbox->addWidget(radio3);
    vbox->addStretch(1);
    groupBox->setLayout(vbox);

    return groupBox;
}

QGroupBox* CustomDialog::setupCheckBox()
{
    QGroupBox *groupBox = new QGroupBox(tr("Non-Exclusive Checkboxes"));
    groupBox->setFlat(true);

    QCheckBox *checkBox1 = new QCheckBox(tr("&Checkbox 1"));
    QCheckBox *checkBox2 = new QCheckBox(tr("C&heckbox 2"));
    checkBox2->setChecked(true);
    QCheckBox *tristateBox = new QCheckBox(tr("Tri-&state button"));
    tristateBox->setTristate(true);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(checkBox1);
    vbox->addWidget(checkBox2);
    vbox->addWidget(tristateBox);
    vbox->addStretch(1);
    groupBox->setLayout(vbox);

    return groupBox;
}

QLayout* CustomDialog::setupTable()
{
    QVBoxLayout *vbox = new QVBoxLayout;
    QLabel* label = new QLabel("Hello", this);
    vbox->addWidget(label);

    QTableWidget* table = new QTableWidget(1,5, this);
    table->setHorizontalHeaderLabels(QStringList{} << "some" << "another");
    table->setItem(0, 1, new QTableWidgetItem("Hello"));
    vbox->addWidget(table);

    QPushButton* button = new QPushButton("PUsh me", this);
    button->setObjectName("btn1");
    vbox->addWidget(button);

    return vbox;
}