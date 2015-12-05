#ifndef WINDOWWIDGET_H
#define WINDOWWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QFile>
#include "gamewidget.h"

#define LBLSTYLESHEET "QLabel {color: orange;background: #FFFFCC;border: %1px solid orange;border-radius: %2px;}"
#define BTNSTYLESHEET "QPushButton {color: red;background: lightgray;border: %1px solid darkgray;border-radius: %2px;} QPushButton:pressed{color: white;background: orange;border: %1px solid darkgray;border-radius: %2px;}"

class WindowWidget : public QWidget
{
    Q_OBJECT
public:
    WindowWidget(QWidget * parent = 0);
    ~WindowWidget();

private:
    QPushButton * restartButton;
    QLabel * scoreLabel;
    QLabel * highScoreLabel;
    GameWidget * gameWidget;
    qreal ratioW, ratioH;
    int highScore;

protected:
    void resizeEvent(QResizeEvent *);

public slots:
    //处理分数增加
    void onScoreInc(int);
    //游戏结束，格子被填满
    void onGameOver();
    //游戏获胜
    void onWin();

};

#endif // WINDOWWIDGET_H
