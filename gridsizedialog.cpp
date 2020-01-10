#include "gridsizedialog.h"
#include <QHBoxLayout>

SpecificValuesSpinBox::SpecificValuesSpinBox(const QVector<int>&values,QWidget* parent):QSpinBox(parent),_values(values)
{
    qSort(_values);
    setMinimum(_values[0]);
    setMaximum(_values[values.size()-1]);
    setValue(_values[0]);
}
void SpecificValuesSpinBox::stepBy(int steps)
{
   _index=(_index+steps)%_values.size();
   setValue(_values[_index]);
}

GridSizeDialog::GridSizeDialog(QWidget* parent):QDialog(parent)
{
    setFixedSize(200,200);
    setWindowTitle(trUtf8("Beállítások"));
    setModal(true); // előugró ablak, rögtön megkapja a fókuszt
    _sizeLabel = new QLabel(trUtf8("Tábla mérete: "));
     QVector<int> values;
    values.resize(3);
    values[0]=3;
    values[1]=4;
    values[2]=6;

    _gameSize = new SpecificValuesSpinBox(values);
    _okButton = new QPushButton(trUtf8("OK"));
    _okButton->setFixedSize(75, 23); // a gombok mérete rögzített
    _cancelButton = new QPushButton(trUtf8("Mégse"));
    _cancelButton->setFixedSize(75, 23);

    connect(_okButton, SIGNAL(clicked()), this, SLOT(accept())); // elfogadás állapota
    connect(_cancelButton, SIGNAL(clicked()), this, SLOT(reject())); // elvetés állapota
    QHBoxLayout *upperLayout = new QHBoxLayout();
    upperLayout->addWidget(_sizeLabel);
    upperLayout->addWidget(_gameSize);

    QHBoxLayout *lowerLayout = new QHBoxLayout();
    lowerLayout->addWidget(_okButton);
    lowerLayout->addWidget(_cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout();
   mainLayout->addLayout(upperLayout);
    mainLayout->addLayout(lowerLayout);

        setLayout(mainLayout);
}
