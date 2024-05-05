#pragma once

#include <QDialog>

class QLayout;
class QGroupBox;
class QTableWidget;
class QRadioButton;
class QCheckBox;
class QLineEdit;

class CustomDialog : public QDialog
{
Q_OBJECT
public:
    CustomDialog(QWidget *parent = 0);
    void walkTree();
private:
    //void setup();
    QGroupBox* setupCheckBox();
    QGroupBox* setupRadioBox();
    QLayout* setupTable();
    QLayout* setupTextInput();
    QLayout* setupButtons();
    void setName(QWidget* w, QString name);
private slots:
    void updateTable();
    void addRow();
    void updateBoxes();
private:
    QTableWidget* m_table;
    QRadioButton* m_radio1;
    QRadioButton* m_radio2;
    QRadioButton* m_radio3;
    QCheckBox* m_checkBox1;
    QCheckBox* m_checkBox2;
    QCheckBox* m_tristateBox;
    QLineEdit* m_edit;
};