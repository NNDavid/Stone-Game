#ifndef GRIDSIZEDIALOG_H
#define GRIDSIZEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QVector>
class SpecificValuesSpinBox:public QSpinBox
{
public:
    explicit SpecificValuesSpinBox(const QVector<int>&values,QWidget* parent=nullptr);

protected:
    void stepBy(int steps);
private:
    QVector<int> _values;
    int _index=0;

};

class GridSizeDialog:public QDialog // rács méretét beállító dialógus
{
    Q_OBJECT
public:
    explicit GridSizeDialog(QWidget *parent = nullptr);
    int gameSize()const  {return _gameSize->value();}

private:
    QLabel* _sizeLabel;
    SpecificValuesSpinBox *_gameSize; // számbeállító
    QPushButton *_okButton;
    QPushButton *_cancelButton;
};

#endif // GRIDSIZEDIALOG_H
