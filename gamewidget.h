#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QGLWidget>//需要在open gl中添加 + opengl
#include <QMouseEvent>
#include <QEventLoop>
#include <QTimer>
#include <QPainter>
#include <QList>
#include <QDebug>
enum GestureDirection{
    LEFT = 0,   //左
    RIGHT = 1,  //右
    UP = 2,     //上
    DOWN = 3   //下
};

enum AnimationType{
    MOVE = 0,   //移动动画
    APPRENANCE = 1  //出现动画
};

struct Animation{
    AnimationType type;
    GestureDirection direction;
    QPointF startPos;
    QPointF endPos;
    int digit;
    int digit2;
};


class GameWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GameWidget(QWidget *parent = 0);
    //~GameWidget();
private:
    //游戏面板
    int board[4][4];
    //当前面板上数字的计数
    int digitCount;
    //当前得分
    int curScore;
    //手势的起始位置
    QPoint startPos;
    //动画的链表~~
    QList<struct Animation> animationList;
    //数字格子的高度与宽度
    qreal h, w;
    //窗口缩放比例
    qreal ratioH, ratioW;
    //格子圆角的x,y
    qreal rX, rY;
    //是否在播放动画效果
    bool isAnimating;
    //动画播放的定时器
    QTimer timer;

public:
    //初始化两个方格
    void init2Block();
    //检查游戏是否结束
    bool checkGameOver()const;
    //检查是否获胜
    bool checkWin()const;
    //得到数字对应颜色在数组中的下标
    int getDigitColor(int num)const;
    //播放一阵的动画
    bool playAnimation(Animation &, QPainter &);
    //绘制出动画效果
    bool drawAnimation(QPainter &);
    //绘图事件
    void paintEvent(QPaintEvent * );
    //窗口重绘事件
    void resizeEvent(QResizeEvent * );
    //鼠标按下事件
    void mousePressEvent(QMouseEvent * );
    //鼠标释放事件
    void mouseReleaseEvent(QMouseEvent *);
signals:
    //手势方向信号
    void GestureMoved(GestureDirection );
    //分数增加信号
    void scoreInc(int );
    //游戏结束信号
    void gameOver();
    //获胜信号
    void win();

public slots:

    void onGestureMoved(GestureDirection );
    //重新开始游戏
    void restart();





};

#endif // GAMEWIDGET_H
