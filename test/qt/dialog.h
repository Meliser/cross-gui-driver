#pragma once

#include <QDialog>

class QLayout;
class QGroupBox;

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
};