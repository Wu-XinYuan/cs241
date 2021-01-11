#include "gridschangedialog.h"
#include "ui_gridschangedialog.h"
#include <QButtonGroup>

gridsChangeDialog::gridsChangeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::gridsChangeDialog)
{
    ui->setupUi(this);
    resize(671,800);
    radiobuttonLayout=new QGridLayout();
    buttonsGroup = new QButtonGroup(this);
    int i=0, j=0;
    int k=0;
    for(i=0; i<10; i++)
    {
        for(j=0; j<10; j++)
        {
            k=10*i+j;
            buttons[k]=new QRadioButton();
            radiobuttonLayout->addWidget(buttons[k], 10-i, j);
            buttonsGroup->addButton(buttons[k],k);
        }
    }
//    buttonsGroup->setExclusive(false);

    radiobuttonLayout->setHorizontalSpacing(0);
    radiobuttonLayout->setVerticalSpacing(0);
    radiobuttonLayout->setContentsMargins(0, 0, 0, 0);
    ui->groupBox->setLayout(radiobuttonLayout);

    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::blue);
    for(int i=0; i<100; ++i) {
        buttons[i]->setText(QString::number(i));
        buttons[i]->setBaseSize(5,10);
        buttons[i]->setPalette(pa);
    }
}

gridsChangeDialog::~gridsChangeDialog()
{
    delete ui;
}

void gridsChangeDialog::receiveGrid(int g){
    grid=g;
    buttons[g]->setChecked(true);
}

void gridsChangeDialog::on_buttonBox_accepted()
{
    grid=buttonsGroup->checkedId();
    emit sendGrid(grid);
}

//void gridsChangeDialog::on_pushButton_clicked()
//{
//    for (int i=0; i<100; ++i) {
//        buttons[i]->setChecked(false);
//    }
//}
