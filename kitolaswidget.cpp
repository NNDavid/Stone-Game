#include "kitolaswidget.h"
#include <QApplication>
#include <QKeyEvent>
#include <QMessageBox>
KitolasWidget::KitolasWidget(QWidget *parent) : QWidget(parent)
{
    setFixedSize(400,400);
    _mainLayout=new QVBoxLayout();
    _buttonTable=new QGridLayout();
    _scoreDisplay=new QLabel();
    _gameModel=new KitolasModel();
    _gridSizeDialog=new GridSizeDialog();
    _saveGameWidget=nullptr;
    _loadGameWidget=nullptr;
    _scoreDisplay->setText("Új játék kezdéséhez kattintson a Ctrl + N gombra!");
    _mainLayout->addWidget(_scoreDisplay);
    _mainLayout->addLayout(_buttonTable);
    setLayout(_mainLayout);
    connect(_gridSizeDialog,SIGNAL(accepted()),this,SLOT(newGame()));
    connect(_gameModel,SIGNAL(fieldChanged(int,int,KitolasModel::Player)),this,SLOT(modelFieldChanged(int,int,KitolasModel::Player)));
    connect(_gameModel,SIGNAL(scoreChanged(QString)),_scoreDisplay,SLOT(setText(QString)));
    connect(_gameModel,SIGNAL(gameOver(KitolasModel::Player)),this,SLOT(gameOver(KitolasModel::Player)));
    connect(_gameModel,SIGNAL(sizeChanged(int)),this,SLOT(sizeChanged(int)));
}
KitolasWidget::~KitolasWidget()
{
    foreach(QVector<QPushButton*> row, _field)
    {
           foreach(QPushButton* button,row)
           {
               _buttonTable->removeWidget(button);
               delete button;
           }
    }
    delete _gameModel;
    if(_saveGameWidget!=nullptr) delete _saveGameWidget;
    if(_loadGameWidget!=nullptr) delete _loadGameWidget;
    delete _gridSizeDialog;
}
void KitolasWidget::keyPressEvent(QKeyEvent* event)
{
    if(event->key()==Qt::Key_N && QApplication::keyboardModifiers() == Qt::ControlModifier)
    {
        //new game
        _gridSizeDialog->exec();
    }
    else if(event->key()==Qt::Key_S && QApplication::keyboardModifiers() == Qt::ControlModifier)
    {
        if (_saveGameWidget == nullptr) // ha még egyszer sem nyitották meg az ablakot
        {
            _saveGameWidget = new SaveGameWidget();
            connect(_saveGameWidget, SIGNAL(accepted()), this, SLOT(saveGame()));
        }
        _saveGameWidget->setGameList(_gameModel->saveGameList());
        _saveGameWidget->open();
    }
    else if(event->key()==Qt::Key_L && QApplication::keyboardModifiers() == Qt::ControlModifier)
    {
        //load game
        if (_loadGameWidget == nullptr) // ha még egyszer sem nyitották meg az ablakot
        {
            _loadGameWidget = new LoadGameWidget();
            connect(_loadGameWidget, SIGNAL(accepted()), this, SLOT(loadGame()));
        }


        _loadGameWidget->setGameList(_gameModel->saveGameList());
        _loadGameWidget->open();
    }
    else if(_isSelected)
    {
        bool directionButtonPressed=true;
        KitolasModel::Direction dir;
        switch(event->key())
        {
            case Qt::Key_8:dir=KitolasModel::Direction::UP;break;
            case Qt::Key_2:dir=KitolasModel::Direction::DOWN;break;
            case Qt::Key_4:dir=KitolasModel::Direction::LEFT;break;
            case Qt::Key_6:dir=KitolasModel::Direction::RIGHT;break;
            default:directionButtonPressed=false; break;
        }
        if(directionButtonPressed)
        {
            _gameModel->stepGame(_selectedIndex.first,_selectedIndex.second,dir);
            _isSelected=false;
            for(int i=0;i<_size;i++)
                for(int j=0;j<_size;j++)
                {
                    if(_gameModel->canBeSelected(i,j)) _field[i][j]->setEnabled(true);
                    else _field[i][j]->setEnabled(false);
                }
        }
    }

}
void KitolasWidget::newGame()
{
    foreach(QVector<QPushButton*> row, _field)
    {
           foreach(QPushButton* button,row)
           {
               _buttonTable->removeWidget(button);
               delete button;
           }
    }
    _size=_gridSizeDialog->gameSize();
    _field.resize(_size);
    for(int i=0;i<_size;i++)
    {
        _field[i].resize(_size);
        for(int j=0;j<_size;j++)
        {
            _field[i][j]=new QPushButton();
            _field[i][j]->setFixedSize(50,50);
            _buttonTable->addWidget(_field[i][j],i,j);
            connect(_field[i][j],SIGNAL(clicked()),this,SLOT(buttonClicked()));
        }
    }
  _gameModel->newGame(_size);
  for(int i=0;i<_size;i++)
      for(int j=0;j<_size;j++)
      {
          if(_gameModel->canBeSelected(i,j)) _field[i][j]->setEnabled(true);
          else _field[i][j]->setEnabled(false);
      }
}
void KitolasWidget::buttonClicked()
{
    int i,j;
    QPushButton* sender_button=dynamic_cast<QPushButton*>(QObject::sender());
    int location=_buttonTable->indexOf(sender_button);
    i=location/_size;
    j=location%_size;
    if(_isSelected) recolor();
    KitolasModel::Player player=_gameModel->getPlayerOnIndex(i,j);
    QString color;
    if(player==KitolasModel::Player::BlackPlayer)
       {
             color="QPushButton {background-color:rgb(255,255,0)}";
         }
         else if(player==KitolasModel::Player::WhitePlayer)
         {
             color="QPushButton {background-color:rgb(0,255,255)}";
         }
         _field[i][j]->setStyleSheet(color);
         _selectedIndex.first=i;
         _selectedIndex.second=j;
         _isSelected=true;
   }
void KitolasWidget::modelFieldChanged(int i, int j,KitolasModel::Player player)
{
    QString color;
    if(player==KitolasModel::Player::BlackPlayer)
    {
        color="QPushButton {background-color:rgb(255,0,0)}";
    }
    else if(player==KitolasModel::Player::WhitePlayer)
    {
        color="QPushButton {background-color:rgb(0,0,255)}";
    }
    else if(player==KitolasModel::Player::NoPlayer)
    {
        color="QPushButton {background-color:rgb(255,255,255)}";
    }
    _field[i][j]->setStyleSheet(color);
}
void KitolasWidget::recolor()
{
    QString color;
    KitolasModel::Player player=_gameModel->getPlayerOnIndex(_selectedIndex.first,_selectedIndex.second);
    if(player==KitolasModel::Player::BlackPlayer)
    {
        color="QPushButton {background-color:rgb(255,0,0)}";
    }
    else if(player==KitolasModel::Player::WhitePlayer)
    {
        color="QPushButton {background-color:rgb(0,0,255)}";
    }
    _field[_selectedIndex.first][_selectedIndex.second]->setStyleSheet(color);
}
void KitolasWidget::gameOver(KitolasModel::Player winner)
{
    switch(winner)
    {
        case KitolasModel::Player::NoPlayer:
            QMessageBox::information(this, trUtf8("KitolasGame"), trUtf8("Játék vége! A játék döntetlen lett!"));
            break;

        case KitolasModel::Player::BlackPlayer:
            QMessageBox::information(this, trUtf8("KitolasGame"), trUtf8("Játék vége! A piros játékos győzött"));
        break;

        case KitolasModel::Player::WhitePlayer:
            QMessageBox::information(this, trUtf8("KitolasGame"), trUtf8("Játék vége! A kék játékos győzött"));
            break;
    }
    _gameModel->newGame(_size);

}

void KitolasWidget::loadGame()
{
    // betöltjük a kiválasztott játékot
    if (_gameModel->loadGame(_loadGameWidget->selectedGame()))
    {
        QMessageBox::information(this, trUtf8("JTic-Tac-Toe"), trUtf8("Játék betöltve!"));
        for(int i=0;i<_size;i++)
            for(int j=0;j<_size;j++)
            {
                if(_gameModel->canBeSelected(i,j)) _field[i][j]->setEnabled(true);
                else _field[i][j]->setEnabled(false);
            }
    }
    else
    {
        QMessageBox::warning(this, trUtf8("Tic-Tac-Toe"), trUtf8("A játék betöltése sikertelen!"));
    }
}

void KitolasWidget::saveGame()
{
    // elmentjük a kiválasztott játékot

    if (_gameModel->saveGame(_saveGameWidget->selectedGame()))
    {
        QMessageBox::information(this, trUtf8("Tic-Tac-Toe"), trUtf8("Játék sikeresen mentve!"));
    }
    else
    {
        QMessageBox::warning(this, trUtf8("Tic-Tac-Toe"), trUtf8("A játék mentése sikertelen!"));
    }
}
void KitolasWidget::sizeChanged(int size)
{
    foreach(QVector<QPushButton*> row, _field)
    {
           foreach(QPushButton* button,row)
           {
               _buttonTable->removeWidget(button);
               delete button;
           }
    }
    _size=size;
    _field.resize(_size);
    for(int i=0;i<_size;i++)
    {
        _field[i].resize(_size);
        for(int j=0;j<_size;j++)
        {
            _field[i][j]=new QPushButton();
            _field[i][j]->setFixedSize(50,50);
            _buttonTable->addWidget(_field[i][j],i,j);
            connect(_field[i][j],SIGNAL(clicked()),this,SLOT(buttonClicked()));
        }
    }
}
