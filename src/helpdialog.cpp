/*! \file helpdialog.cpp
 * ICP project 2020/21
 * Author: Michal Findra
 */

#include "helpdialog.h"
#include "ui_helpdialog.h"

HelpDialog::HelpDialog(QWidget *parent) : QDialog(parent), ui(new Ui::HelpDialog) {
    ui->setupUi(this);
}

HelpDialog::~HelpDialog() {
    delete ui;
}
