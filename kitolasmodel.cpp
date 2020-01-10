#include "kitolasmodel.h"
#include<ctime>
#include<cstdlib>
#include <QDebug>

KitolasModel::KitolasModel(QObject *parent) : QObject(parent)
{


}
KitolasModel::~KitolasModel()
{
    for(int i=0;i<_size;i++)
        delete[] _gameTable[i];

    delete[] _gameTable;
}
void KitolasModel::newGame(int size)
{
    srand(time(0));
    if(_size>0)
    {
        for(int i=0;i<_size;i++)
            delete[] _gameTable[i];
        delete[] _gameTable;
    }

    _size=size;
    _gameTable=new KitolasModel::Player* [_size];
    for(int i=0;i<_size;i++)
    {
        _gameTable[i]=new KitolasModel::Player[_size];
        for(int j=0;j<_size;j++)
        {
            _gameTable[i][j]=KitolasModel::Player::NoPlayer;
            emit fieldChanged(i,j,KitolasModel::Player::NoPlayer);
        }
    }

    for(int i=0;i<_size;i++)
    {
        int x,y;
        do
        {
            x=rand()%_size;
            y=rand()%_size;
        }while(_gameTable[x][y]!=KitolasModel::Player::NoPlayer);
        _gameTable[x][y]=KitolasModel::Player::BlackPlayer;
        emit fieldChanged(x,y,KitolasModel::Player::BlackPlayer);
    }

    for(int i=0;i<_size;i++)
    {
        int x,y;
        do
        {
            x=rand()%_size;
            y=rand()%_size;
        }while(_gameTable[x][y]!=KitolasModel::Player::NoPlayer);
        _gameTable[x][y]=KitolasModel::Player::WhitePlayer;
        emit fieldChanged(x,y,KitolasModel::Player::WhitePlayer);
    }

    _steps=_size*5;
    _scores.first=_size;
    _scores.second=_size;

    _currentPlayer=KitolasModel::Player::BlackPlayer;
    emit scoreChanged("Piros játékos kövei: " + QString::number(_scores.first) + "\nKék játékos kövei: " + QString::number(_scores.second)+"\nMegmaradt lépések száma: " + QString::number(_steps));

}
void KitolasModel::stepGame(int i, int j,KitolasModel::Direction dir)
{
    bool isEliminated=false;
    KitolasModel::Player current,before;
    if(dir == KitolasModel::Direction::UP)
    {
       // bool not_found=true;
        int k=i;
        current=KitolasModel::Player::NoPlayer;
        while(k>=0&&_gameTable[k][j]!=KitolasModel::Player::NoPlayer)
        {
            before=current;
            current=_gameTable[k][j];
            _gameTable[k][j]=before;
            emit fieldChanged(k,j,_gameTable[k][j]);
            k--;
        }
        if(k<0) isEliminated=true;
        else
        {
            _gameTable[k][j]=current;
            emit fieldChanged(k,j,_gameTable[k][j]);
        }

    }
    else if(dir == KitolasModel::Direction::DOWN)
    {
        int k=i;
        current=KitolasModel::Player::NoPlayer;
        while(k<_size&&_gameTable[k][j]!=KitolasModel::Player::NoPlayer)
        {
            before=current;
            current=_gameTable[k][j];
            _gameTable[k][j]=before;
            emit fieldChanged(k,j,_gameTable[k][j]);
            k++;
        }
        if(k==_size) isEliminated=true;
        else
        {
            _gameTable[k][j]=current;
            emit fieldChanged(k,j,_gameTable[k][j]);
        }
    }
    else if (dir == KitolasModel::Direction::LEFT)
    {
        int k=j;
        current=KitolasModel::Player::NoPlayer;
        while(k>=0&&_gameTable[i][k]!=KitolasModel::Player::NoPlayer)
        {
            before=current;
            current=_gameTable[i][k];
            _gameTable[i][k]=before;
            emit fieldChanged(i,k,_gameTable[i][k]);
            k--;
        }
        if(k<0) isEliminated=true;
        else
        {
            _gameTable[i][k]=current;
            emit fieldChanged(i,k,_gameTable[i][k]);
        }
    }
    else if(dir == KitolasModel::Direction::RIGHT)
    {
        int k=j;
        current=KitolasModel::Player::NoPlayer;
        while(k<_size&&_gameTable[i][k]!=KitolasModel::Player::NoPlayer)
        {
            before=current;
            current=_gameTable[i][k];
            _gameTable[i][k]=before;
            emit fieldChanged(i,k,_gameTable[i][k]);
            k++;
        }
        if(k==_size) isEliminated=true;
        else
        {
            _gameTable[i][k]=current;
            emit fieldChanged(i,k,_gameTable[i][k]);
        }
    }

    if(isEliminated&&current==KitolasModel::Player::BlackPlayer) _scores.first--;
    else if(isEliminated&&current==KitolasModel::Player::WhitePlayer) _scores.second--;
    _steps--;
     emit scoreChanged("Piros játékos kövei: " + QString::number(_scores.first) + "\nKék játékos kövei: " + QString::number(_scores.second)+"\nMegmaradt lépések száma: " + QString::number(_steps));
    if(_steps==0)
    {
        if(_scores.first==_scores.second) emit gameOver(KitolasModel::Player::NoPlayer);
        else if(_scores.first<_scores.second) emit gameOver(KitolasModel::Player::WhitePlayer);
        else if(_scores.first>_scores.second) emit gameOver(KitolasModel::Player::BlackPlayer);
    }
    else if(_scores.first==0) emit gameOver(KitolasModel::Player::WhitePlayer);
    else if(_scores.second==0) emit gameOver(KitolasModel::Player::BlackPlayer);
    else
    {
        if(_currentPlayer==KitolasModel::Player::BlackPlayer) _currentPlayer=KitolasModel::Player::WhitePlayer;
        else if(_currentPlayer==KitolasModel::Player::WhitePlayer) _currentPlayer=KitolasModel::Player::BlackPlayer;

    }
}

bool KitolasModel::loadGame(int gameIndex)
{
    QVector<int> saveGameData;

    if (!_dataAccess.loadGame(gameIndex, saveGameData)) // az adatelérés végzi a tevékenységeket
        return false;

    // feldolgozttuk a kapott vektort
    if(saveGameData[0]!=_size)
    {
        emit sizeChanged(saveGameData[0]);
        for(int i=0;i<_size;i++)
            delete[] _gameTable[i];
        delete[] _gameTable;
        _size=_size = saveGameData[0];

        _gameTable=new Player* [_size];
        for(int i=0;i<_size;i++)
            _gameTable[i]=new Player[_size];

    }
    else _size = saveGameData[0];
    _steps= saveGameData[1];
    _currentPlayer = (Player)saveGameData[2];
    _scores.first=saveGameData[3];
    _scores.second=saveGameData[4];
    for (int i = 0; i < _size; ++i)
        for (int j = 0; j < _size; ++j)
        {
            _gameTable[i][j] = (Player)saveGameData[5 + i * _size + j];
            emit fieldChanged(i,j,_gameTable[i][j]);
        }
    emit scoreChanged("Piros játékos kövei: " + QString::number(_scores.first) + "\nKék játékos kövei: " + QString::number(_scores.second)+"\nMegmaradt lépések száma: " + QString::number(_steps));


    return true;
}

bool KitolasModel::saveGame(int gameIndex)
{
    QVector<int> saveGameData;

    // összerakjuk a megfelelő tartalmat
    saveGameData.push_back(_size);
    saveGameData.push_back(_steps);
    saveGameData.push_back((int)_currentPlayer);
    saveGameData.push_back(_scores.first);
    saveGameData.push_back(_scores.second);
    for (int i = 0; i < _size; ++i)
        for (int j = 0; j < _size; ++j)
        {
            saveGameData.push_back((int)_gameTable[i][j]);
        }

    return _dataAccess.saveGame(gameIndex, saveGameData); // az adatelérés végzi a tevékenységeket
}

