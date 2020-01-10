#ifndef KITOLASMODEL_H
#define KITOLASMODEL_H

#include <QObject>
#include "kitolasdataaccess.h"

class KitolasModel : public QObject
{
    Q_OBJECT
public:
    enum Player{NoPlayer,BlackPlayer,WhitePlayer};
    enum Direction{UP,DOWN,LEFT,RIGHT};
    KitolasModel(QObject *parent = nullptr);
    ~KitolasModel();
    void newGame(int size);
    void stepGame(int i, int j, KitolasModel::Direction dir);
    bool canBeSelected(int i,int j) const {return _gameTable[i][j] == _currentPlayer;}
    KitolasModel::Player getPlayerOnIndex(int i,int j) const {return _gameTable[i][j];}
    bool loadGame(int gameIndex);
    bool saveGame(int gameIndex);
    QVector<QString> saveGameList() const { return _dataAccess.saveGameList();}


signals:
    void fieldChanged(int i,int j,KitolasModel::Player player);
    void gameOver(KitolasModel::Player winner);
    void scoreChanged(QString score);
    void sizeChanged(int size);

private:
    int _size=0;
    Player** _gameTable;
    int _steps;
    std::pair<int,int> _scores;
    Player _currentPlayer;
    KitolasDataAccess _dataAccess;
};

#endif // KITOLASMODEL_H
