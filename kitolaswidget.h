#ifndef KITOLASWIDGET_H
#define KITOLASWIDGET_H

#include <QWidget>
#include <QVector>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include "kitolasmodel.h"
#include "gridsizedialog.h"
#include "savegamewidget.h"
#include "loadgamewidget.h"


class KitolasWidget : public QWidget
{
    Q_OBJECT

public:
    explicit KitolasWidget(QWidget *parent = nullptr);
    ~KitolasWidget();

protected:
    void keyPressEvent(QKeyEvent* event);

private slots:
    void buttonClicked();
    void modelFieldChanged(int i,int j,KitolasModel::Player player);
    void newGame();
    void gameOver(KitolasModel::Player winner);
    void loadGame();
    void saveGame();
    void sizeChanged(int size);

private:
    QVBoxLayout* _mainLayout;
    QGridLayout* _buttonTable;
    QVector<QVector<QPushButton*>> _field;
    bool _isSelected=false;
    QLabel* _scoreDisplay;
    int _size=6;
    KitolasModel* _gameModel;
    void recolor();
    std::pair<int,int> _selectedIndex;
    GridSizeDialog* _gridSizeDialog;
    SaveGameWidget* _saveGameWidget;
    LoadGameWidget* _loadGameWidget;


};

#endif // KITOLASWIDGET_H
