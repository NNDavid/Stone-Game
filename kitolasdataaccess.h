#ifndef KITOLASDATAACCESS_H
#define KITOLASDATAACCESS_H


#include <QString>
#include <QVector>

class KitolasDataAccess // az adatelérés típusa
{
public:
    KitolasDataAccess() {}

    QVector<QString> saveGameList() const; // mentett játékok lekérdezése

    bool loadGame(int gameIndex, QVector<int> &saveGameData); // játék betöltése
    bool saveGame(int gameIndex, const QVector<int> &saveGameData); // játék mentése
};

#endif // KITOLASDATAACCESS_H
