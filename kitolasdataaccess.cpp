#include "kitolasdataaccess.h"
#include <QDateTime>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>

QVector<QString> KitolasDataAccess::saveGameList() const
{
    QVector<QString> result(5);

    // végigmegyünk az 5 helyen
    for (int i = 0; i < 5; i++)
    {
        if (QFile::exists("game" + QString::number(i) + ".sav")) // ha az adott mentés létezik
        {
            QFileInfo info("game"+ QString::number(i) + ".sav");
            result[i] = "[" + QString::number(i + 1) + "] " + info.lastModified().toString("yyyy.MM.dd HH:mm:ss");
            // akkor betöltjük a fájl módosítás időpontját
        }
    }

    return result;
}

bool KitolasDataAccess::loadGame(int gameIndex, QVector<int> &saveGameData)
{
    QFile file("game" + QString::number(gameIndex) + ".sav");
    if (!file.open(QFile::ReadOnly))
        return false;

    QTextStream stream(&file);
    saveGameData.push_back(stream.readLine().toInt());

    // soronként beolvassuk az adatokat a fájlból
    for (int i = 1; i < 5+saveGameData[0]*saveGameData[0]; i++)
        saveGameData.push_back(stream.readLine().toInt());

    file.close();

    return true;
}

bool KitolasDataAccess::saveGame(int gameIndex, const QVector<int> &saveGameData)
{
    QFile file("game" + QString::number(gameIndex) + ".sav");
    if (!file.open(QFile::WriteOnly))
        return false;

    QTextStream stream(&file);

    // soronként egy adatot írunk ki
    for (int i = 0; i < saveGameData.size(); i++)
        stream << saveGameData[i] << endl;

    file.close();

    return true;
}


