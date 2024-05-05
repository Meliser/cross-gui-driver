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
#include <QDebug>
#include <QLineEdit>

#include <iostream>
#include <qwindowdefs.h>

CustomDialog::CustomDialog(QWidget *parent) : QDialog(parent)
{
    QGridLayout *gridbox = new QGridLayout;
    gridbox->addWidget(setupRadioBox(),0,0);
    gridbox->addWidget(setupCheckBox(),1,0);
    gridbox->addLayout(setupTextInput(),2,0);
    gridbox->addLayout(setupTable(),0,1, -1, -1);

    setLayout(gridbox);
    setWindowTitle(tr("Sample Qt App"));
    resize(960, 640);

    updateTable();
}

void CustomDialog::updateTable()
{
    qInfo() << "Update table";
    int row = m_table->currentRow();
    //assert(row == 0);

    QTableWidgetItem* i = m_table->item(row, 0);
    assert(i);
    if (m_radio1->isChecked()) {
        i->setText("radio1");
    }
    else if (m_radio2->isChecked()) {
        i->setText("radio2");
    }
    else if (m_radio3->isChecked()) {
        i->setText("radio3");
    }
    else {
        i->setText("N/A");
    }

    i = m_table->item(row, 1);
    assert(i);
    if (m_checkBox1->isChecked()) {
        i->setText("OK");
    }
    else {
        i->setText("FAIL");
    }

    i = m_table->item(row, 2);
    assert(i);
    if (m_checkBox2->isChecked()) {
        i->setText("OK");
    }
    else {
        i->setText("FAIL");
    }

    i = m_table->item(row, 3);
    assert(i);
    if (m_tristateBox->checkState() == Qt::Unchecked) {
        i->setText("FAIL");
    }
    else if (m_tristateBox->checkState() == Qt::PartiallyChecked) {
        i->setText("partial");
    }
    else {
        i->setText("OK");
    }

    i = m_table->item(row, 4);
    assert(i);
    i->setText(m_edit->text());
}

QGroupBox* CustomDialog::setupRadioBox()
{
    QGroupBox *groupBox = new QGroupBox(tr("Exclusive Radio Buttons"), this);

    m_radio1 = new QRadioButton(tr("&Radio button 1"), this);
    m_radio2 = new QRadioButton(tr("R&adio button 2"), this);
    m_radio3 = new QRadioButton(tr("Ra&dio button 3"), this);

    connect(m_radio1, &QRadioButton::clicked, this, &CustomDialog::updateTable);
    connect(m_radio2, &QRadioButton::clicked, this, &CustomDialog::updateTable);
    connect(m_radio3, &QRadioButton::clicked, this, &CustomDialog::updateTable);

    m_radio1->setChecked(true);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(m_radio1);
    vbox->addWidget(m_radio2);
    vbox->addWidget(m_radio3);
    vbox->addStretch(1);
    groupBox->setLayout(vbox);

    return groupBox;
}

QGroupBox* CustomDialog::setupCheckBox()
{
    QGroupBox *groupBox = new QGroupBox(tr("Non-Exclusive Checkboxes"));
    groupBox->setFlat(true);

    m_checkBox1 = new QCheckBox(tr("&Checkbox 1"));
    m_checkBox2 = new QCheckBox(tr("C&heckbox 2"));
    m_checkBox2->setChecked(true);
    m_tristateBox = new QCheckBox(tr("Tri-&state button"));
    m_tristateBox->setTristate(true);

    connect(m_checkBox1, &QRadioButton::clicked, this, &CustomDialog::updateTable);
    connect(m_checkBox2, &QRadioButton::clicked, this, &CustomDialog::updateTable);
    connect(m_tristateBox, &QRadioButton::clicked, this, &CustomDialog::updateTable);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(m_checkBox1);
    vbox->addWidget(m_checkBox2);
    vbox->addWidget(m_tristateBox);
    vbox->addStretch(1);
    groupBox->setLayout(vbox);

    return groupBox;
}

QLayout* CustomDialog::setupTable()
{
    QVBoxLayout *vbox = new QVBoxLayout;
    QLabel* label = new QLabel("Какая-то таблица", this);
    vbox->addWidget(label);

    m_table = new QTableWidget(0,5, this);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setEditTriggers(QTableWidget::NoEditTriggers);
    m_table->setHorizontalHeaderLabels(QStringList{} << "Radio button" << "Checkbox1" << "Checkbox2" << "Tri-state" << "Text");
    addRow();
    m_table->setCurrentCell(0,0);
    connect(m_table, &QTableWidget::itemSelectionChanged, this, &CustomDialog::updateBoxes);

    vbox->addWidget(m_table);

    vbox->addLayout(setupButtons());

    return vbox;
}

void CustomDialog::updateBoxes()
{
    qInfo() << "Update boxes";
    int row = m_table->currentRow();
    //assert(row == 0);

    QTableWidgetItem* i = m_table->item(row, 0);
    assert(i);
    if (i->text() == "radio1") {
        m_radio1->setChecked(true);
    }
    else if (i->text() == "radio2") {
        m_radio2->setChecked(true);
    }
    else if (i->text() == "radio3") {
        m_radio3->setChecked(true);
    }

    i = m_table->item(row, 1);
    assert(i);
    if (i->text() == "OK") {
        m_checkBox1->setChecked(true);
    }
    else {
        m_checkBox1->setChecked(false);
    }

    i = m_table->item(row, 2);
    assert(i);
    if (i->text() == "OK") {
        m_checkBox2->setChecked(true);
    }
    else {
        m_checkBox2->setChecked(false);
    }

    i = m_table->item(row, 3);
    assert(i);
    if (i->text() == "OK") {
        m_tristateBox->setCheckState(Qt::Checked);
    }
    else if (i->text() == "FAIL") {
        m_tristateBox->setCheckState(Qt::Unchecked);
    }
    else {
        m_tristateBox->setCheckState(Qt::PartiallyChecked);
    }

    i = m_table->item(row, 4);
    assert(i);
    m_edit->setText(i->text());
}

void CustomDialog::addRow()
{
    int rc = m_table->rowCount();
    m_table->setRowCount(rc+1);
    //rc++;
    //assert(rc == 0);
    //rc = 0;
    m_table->setItem(rc, 0, new QTableWidgetItem);
    m_table->setItem(rc, 1, new QTableWidgetItem);
    m_table->setItem(rc, 2, new QTableWidgetItem);
    m_table->setItem(rc, 3, new QTableWidgetItem);
    m_table->setItem(rc, 4, new QTableWidgetItem);
}

QLayout* CustomDialog::setupButtons()
{
    QGridLayout *gbox = new QGridLayout();

    QPushButton* button = new QPushButton("Добавить", this);
    setName(button,"AddBtn");
    connect(button, &QPushButton::clicked, [this](){
        CustomDialog::addRow();
        disconnect(m_table, &QTableWidget::itemSelectionChanged, this, &CustomDialog::updateBoxes);
        m_table->setCurrentCell(m_table->rowCount()-1, 0);
        connect(m_table, &QTableWidget::itemSelectionChanged, this, &CustomDialog::updateBoxes);
        updateTable();
    });
    gbox->addWidget(button, 0, 0);

    button = new QPushButton("Удалить", this);
    connect(button, &QPushButton::clicked, [this](){
        int row = m_table->currentRow();
        m_table->removeRow(row);
        if (row == m_table->rowCount())
            m_table->setCurrentCell(row - 1, 0);
        else
            m_table->setCurrentCell(row, 0);
    });
    setName(button,"DeleteBtn");
    gbox->addWidget(button, 1, 0);

    // button = new QPushButton("Отменить", this);
    // setName(button,"RevertBtn");
    // gbox->addWidget(button, 0, 1);

    // button = new QPushButton("Сохранить", this);
    // setName(button,"SaveBtn");
    // gbox->addWidget(button, 1, 1);

    return gbox;
}

QLayout* CustomDialog::setupTextInput()
{
    QVBoxLayout *vbox = new QVBoxLayout;
    m_edit = new QLineEdit("Какое-то сообщение", this);
    connect(m_edit, &QLineEdit::textEdited, this, &CustomDialog::updateTable);
    vbox->addWidget(m_edit);
    return vbox;
}

void CustomDialog::setName(QWidget* w, QString name)
{
    w->setObjectName(name);
    w->setToolTip(name);
}