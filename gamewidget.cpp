#include "gamewidget.h"

QColor digitColor[11] = {
    QColor::fromRgb(0xFF, 0xFF, 0xCC), QColor::fromRgb(0xFF, 0xFF, 0x99),
    QColor::fromRgb(0xFF, 0xCC, 0xCC), QColor::fromRgb(0xFF, 0xCC, 0x99),
    QColor::fromRgb(0xFF, 0x99, 0x99), QColor::fromRgb(0xFF, 0x99, 0x66),
    QColor::fromRgb(0xFF, 0x66, 0x66), QColor::fromRgb(0xCC, 0x99, 0x66),
    QColor::fromRgb(0xCC, 0x33, 0x33), QColor::fromRgb(0xCC, 0x00, 0x33),
    QColor::fromRgb(0xFF, 0x00, 0x00)
};

//每个方向位置的增量
QPointF dPos[5];

GameWidget::GameWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
      timer(this)
{
    //链接手势移动的信号r
    connect(this, SIGNAL(GestureMoved(GestureDirection)), this, SLOT(onGestureMoved(GestureDirection)));
    //定时器时间到了更新函数
    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    //将记录数组清空
    memset(board, 0, sizeof(board));
    //分数初始化成0
    curScore = 0;
    //数字的个数初始化为2
    this->digitCount = 2;
    //是否开始绘图
    this->isAnimating = false;
    //初始化两个方块
    this->init2Block();

}


void GameWidget::init2Block()
{

    board[rand()%4][rand()%4] = 2;
    int i = rand()%4, j = rand()%4;
    while(board[i][j] != 0)
        i = rand()%4, j = rand()%4;
    board[i][j] = 2;
    update();
}


void GameWidget::mousePressEvent(QMouseEvent * e)
{
    startPos = e->pos();
}

void GameWidget::mouseReleaseEvent(QMouseEvent * e)
{
    if(isAnimating == true)//当前如果正在播放动画的话就先直接退出
        return;
    QPoint endPos = e->pos();
    float dX = (float)(endPos.x() - startPos.x());
    float dY = (float)(endPos.y() - startPos.y());
    if(abs(dX) > abs(dY)){
        if(dX > 0)
            emit GestureMoved(RIGHT);
        else
            emit GestureMoved(LEFT);
    }else{
        if(dY > 0)
            emit GestureMoved(DOWN);
        else
            emit GestureMoved(UP);
    }
}


void GameWidget::onGestureMoved(GestureDirection direction)
{
    Animation a;
    bool isMove = false, isCombine = false;
    bool isCombined[4][4];
    memset(isCombined, false, sizeof(isCombined));
    int i = 0, j = 0, k = 0;
    switch(direction){
    case LEFT:
        qDebug() << "LEFT!!!";
        for(i = 0; i < 4; ++i){
            j = k = 0;
            while(true){
                while(j < 4 && board[i][j] == 0)
                    j++;
                if(j > 3)
                    break;
                qSwap(board[i][k], board[i][j]);
                if(k != j)
                    isMove = true;
                //记录下动画应该执行的信息
                a.type = MOVE;
                a.startPos = QPointF(7 * ratioW + (ratioW * 5 + w) * j, 7 * ratioH + (ratioH * 5 + h) * i);
                a.endPos = QPointF(7 * ratioW + (ratioW * 5 + w) * k, 7 * ratioH + (ratioH * 5 + h) * i);//指向的实际上是每个小方格的左上角
                a.digit = a.digit2 = board[i][k];
                a.direction = LEFT;
                if(k > 0 && board[i][k] == board[i][k-1] && !isCombined[i][k-1]){
                    board[i][k-1] <<= 1;
                    board[i][k] = 0;
                    isCombined[i][k-1] = true;
                    isCombine = true;
                    a.digit2 = board[i][k-1];
                    a.endPos = QPointF(7 * ratioW + (ratioW * 5 + w) * (k - 1), 7 * ratioH + (ratioH * 5 + h) * i);
                    curScore += board[i][k-1];
                    emit scoreInc(curScore);
                    digitCount--;
                }else{
                    k++;
                }
                j++;
                animationList.append(a);
            }
        }
        break;
    case RIGHT:
        qDebug() << "RIGHT!!!";
        for(i = 0; i < 4; ++i){
            j = k = 3;
            while(true){
                while(j >= 0 && board[i][j] == 0)
                    j--;
                if(j < 0)
                    break;
                qSwap(board[i][j], board[i][k]);
                if(k != j)
                    isMove = true;
                a.type = MOVE;
                a.startPos = QPointF(7 * ratioW + (ratioW * 5 + w) * j, 7 * ratioH + (ratioH * 5 + h) * i);
                a.endPos = QPointF(7 * ratioW + (ratioW * 5 + w) * k, 7 * ratioH + (ratioH * 5 + h) * i);
                a.digit = a.digit2 = board[i][k];
                a.direction = RIGHT;
                if(k < 3 && board[i][k+1] == board[i][k] && !isCombined[i][k+1]){
                    board[i][k+1] <<= 1;
                    board[i][k] = 0;
                    isCombined[i][k+1] = true;
                    a.digit2 = board[i][k+1];
                    isCombine = true;
                    a.endPos = QPointF(7 * ratioW + (ratioW * 5 + w) * (k+1), 7 * ratioH + (ratioH * 5 + h) * i);
                    curScore += board[i][k+1];
                    emit scoreInc(curScore);
                    digitCount--;
                }else{
                    k--;
                }
                j--;
                animationList.append(a);
            }
        }
        break;
    case UP:
        qDebug() << "UP!!!";
        for(j = 0; j < 4; j++){
            i = k = 0;
            while(true){
                while(i < 4 && board[i][j] == 0)
                    i++;
                if(i > 3)
                    break;
                qSwap(board[i][j], board[k][j]);
                if(i != k)
                    isMove = true;
                a.type = MOVE;
                a.startPos = QPointF(7 * ratioW + (5 * ratioW + w) * j, 7 * ratioH + (5 * ratioH + h) * i);
                a.endPos = QPointF(7 * ratioW + (5 * ratioW + w) * j, 7 * ratioH + (5 * ratioH + h) * k);
                a.digit = a.digit2 = board[k][j];
                a.direction = UP;
                if(k > 0 && board[k-1][j] == board[k][j] && !isCombined[k-1][j]){
                    isCombine = true;
                    board[k - 1][j] <<= 1;
                    board[k][j] = 0;
                    isCombined[k - 1][j] = true;
                    a.digit2 = board[k-1][j];
                    a.endPos = QPointF(7 * ratioW + (5 * ratioW + w) * j, 7 * ratioH + (5 * ratioH + h) * (k - 1));
                    curScore += board[k-1][j];
                    emit scoreInc(curScore);
                    digitCount--;
                }else{
                    k++;
                }
                i++;
                animationList.append(a);
            }
        }
        break;
    case DOWN:
        qDebug() << "DOWN!!!";
        for(j = 0; j < 4; ++j){
            i = k = 3;
            while(true){
                while(i >=0 && board[i][j] == 0)
                    i--;
                if(i < 0)
                    break;
                qSwap(board[i][j], board[k][j]);
                if(i != k)
                    isMove = true;
                a.type = MOVE;
                a.startPos = QPointF(7 * ratioW + (5 * ratioW + w) * j, 7 * ratioH + (5 * ratioH + h) * i);
                a.endPos = QPointF(7 * ratioW + (5 * ratioW + w) * j, 7 * ratioH + (5 * ratioH + h) * k);
                a.digit = a.digit2 = board[k][j];
                a.direction = DOWN;
                if(k < 4 && board[k + 1][j] == board[k][j] && !isCombined[k+1][j]){
                    isCombine = true;
                    board[k+1][j] <<= 1;
                    board[k][j] = 0;
                    isCombined[k+1][j] = true;
                    a.digit2 = board[k+1][j];
                    a.endPos = QPointF(7 * ratioW + (5 * ratioW + w) * j, 7 * ratioH + (5 * ratioH + h) * (k + 1));
                    curScore += board[k+1][j];
                    emit scoreInc(curScore);
                    digitCount--;
                }else{
                    k--;
                }
                i--;
                animationList.append(a);
            }
        }
        break;
    }
    if((isMove || isCombine) && digitCount != 16){
        qDebug() << "Now moved!";
        i = rand()%4;
        j = rand()%4;
        while(board[i][j] != 0){
            i = rand()%4;
            j = rand()%4;
        }
        board[i][j] = (rand()%2 + 1) * 2;//产生2或者是4
        a.type = APPRENANCE;
        a.startPos = a.endPos = QPointF(7 * ratioW + (w + 5 * ratioW) * j, 7 * ratioH + (h + 5 * ratioH) * i);
        a.startPos += QPointF(w/2, h/2);
        a.digit = board[i][j];
        animationList.append(a);
        digitCount++;
    }
    //开始绘制动画
    isAnimating = true;

    timer.start(10);    //这个和update是相连的
}

bool GameWidget::drawAnimation(QPainter & painter)
{
    //设置字体
    QFont font;
    font.setFamily("Consolas");
    font.setBold(true);
    font.setPixelSize(40*ratioH);
    painter.setFont(font);

    //动画是否播放完毕
    bool ok = true;

    //底部大方格背景的颜色
    QBrush brush(QColor::fromRgb(141,121,81));
    painter.setBrush(brush);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(QRectF(2*ratioW, 2*ratioH, width() - 4*ratioW, height() - 4*ratioH), rX, rY);//画出圆角的背景大矩形

    brush.setColor(QColor::fromRgb(171,165,141));//每个数字小方格背景矩形的颜色
    painter.setBrush(brush);

    for(int i = 0; i < 4; ++i){
        for(int j = 0; j < 4; ++j){//画圆角的小矩形
            painter.drawRoundedRect(QRectF(7 * ratioW + (5 * ratioW + w) * j, 7 * ratioH + (5 * ratioH + h) * i, w, h), rX, rY);
        }
    }
    typename QList<struct Animation>::iterator it = animationList.begin();
    for(; it != animationList.end(); ++it){
        if(!playAnimation(*it, painter))
            ok = false;
    }
    return ok;
}


bool GameWidget::playAnimation(Animation & a, QPainter & painter)
{
    bool ret= false;//是否是播放的动画的最后一帧
    QBrush brush(Qt::SolidPattern);
    if(a.type == MOVE){
        switch(a.direction){
        case LEFT:
            if(a.startPos.x() > a.endPos.x())
                a.startPos += dPos[LEFT];
            else
                a.startPos = a.endPos, ret = true;
            break;
        case RIGHT:
            if(a.startPos.x() < a.endPos.x())
                a.startPos += dPos[RIGHT];
            else
                a.startPos = a.endPos, ret = true;
            break;
        case UP:
            if(a.startPos.y() > a.endPos.y())
                a.startPos += dPos[UP];
            else
                a.startPos = a.endPos, ret = true;
            break;
        case DOWN:
            if(a.startPos.y() < a.endPos.y())
                a.startPos += dPos[DOWN];
            else
                a.startPos = a.endPos, ret = true;
            break;
        }

        if(!ret){//还没移动到终点，所以数字暂时还不需要进行递增
            brush.setColor(digitColor[getDigitColor(a.digit)]);
            painter.setBrush(brush);
            painter.drawRoundedRect(QRectF(a.startPos.x(), a.startPos.y(), w, h), rX, rY);
            painter.setPen(QColor::fromRgb(0,0,0));//字体使用黑色
            painter.drawText(QRectF(a.startPos.x(), a.startPos.y(), w, h), Qt::AlignCenter, QString::number(a.digit));
        }else{//移动到终点了，所以数字需要进行递增
            brush.setColor(digitColor[getDigitColor(a.digit2)]);
            painter.setBrush(brush);
            painter.drawRoundedRect(QRectF(a.startPos.x(), a.startPos.y(), w, h), rX, rY);
            painter.setPen(QColor::fromRgb(0,0,0));
            painter.drawText(QRectF(a.startPos.x(), a.startPos.y(), w, h), Qt::AlignCenter, QString::number(a.digit2));
        }
        painter.setPen(Qt::NoPen);
    }else{//Animation type is APPREANCE
        if(a.startPos.x() > a.endPos.x()){
            a.startPos += dPos[4];//这个4是出现动画对应的下标
        }else{
            a.startPos = a.endPos, ret = true;
        }
        brush.setColor(digitColor[getDigitColor(a.digit)]);
        painter.setBrush(brush);
        painter.drawRoundedRect(QRectF(a.startPos.x(), a.startPos.y(),//注意这里两个都是x
                            w - 2*(a.startPos.x() - a.endPos.x()), h - 2 * (a.startPos.y() - a.endPos.y())), rX, rY);
        painter.setPen(QColor::fromRgb(0,0,0));
        painter.drawText(QRectF(a.endPos.x(), a.endPos.y(), w, h), Qt::AlignCenter, QString::number(a.digit));
        painter.setPen(Qt::NoPen);
    }
    return ret;
}


void GameWidget::paintEvent(QPaintEvent * )
{
    qDebug() << "Now enter paint event!";
    QPainter painter(this);
    //设置反锯齿绘图
    painter.setRenderHint(QPainter::Antialiasing);
    //如果当前正在播放动画的话那么缓存位图
    if(isAnimating){
        if(drawAnimation(painter)){//绘制的是当前动画的最后一帧
            qDebug() << "Last Animation";
            isAnimating = false;
            timer.stop();
            animationList.clear();
            if(digitCount == 16){
                //检查是否gameOver
                if(checkGameOver())
                    emit gameOver();
            }
            if(checkWin())
                emit win();
        }
        return;
    }

    //画出底部的大方格
    QBrush brush(QColor::fromRgb(121,141,81));
    painter.setBrush(brush);
    painter.setPen(Qt::NoPen);//使画出来的图形没有描边
    painter.drawRoundedRect(QRectF(2*ratioW, 2*ratioH, width() - 4 * ratioW, height() - 4 * ratioH), rX, rY);

    //设置小方格上字的字体
    QFont font;
    font.setFamily("Consolas");
    font.setBold(true);
    font.setPixelSize(40*ratioH);
    painter.setFont(font);

    //循环绘制游戏的面板
    for(int i = 0; i < 4; ++i){
        for(int j = 0; j < 4; ++j){
            if(board[i][j]){
                brush.setColor(digitColor[getDigitColor(board[i][j])]);
                painter.setBrush(brush);
                painter.drawRoundRect(QRectF(7 * ratioW + (5 * ratioW + w) * j, 7 * ratioH + (5 * ratioH + h) * i, w, h), rX, rY);
                painter.setPen(QColor::fromRgb(0,0,0));
                painter.drawText(QRectF(7*ratioW + (5*ratioW + w) * j, 7 * ratioH + (5 * ratioH + h) * i, w, h),
                                 Qt::AlignCenter, QString::number(board[i][j]));
                painter.setPen(Qt::NoPen);
            }else{//对于没有数字存在的小方格，直接画上方格的初始底色就可以了
                brush.setColor(QColor::fromRgb(171,165,141));
                painter.setBrush(brush);
                painter.drawRoundedRect(QRectF(7 * ratioW + (5 * ratioW + w) * j, 7 * ratioH + (5 * ratioH + h) * i, w, h), rX, rY);
            }
        }
    }
}

void GameWidget::resizeEvent(QResizeEvent *)
{
    ratioW = width()/400.0, ratioH = height() / 400.0;
    //计算小格子的高度以及宽度
    w = width() - 4 * ratioW, h = height() - 4 * ratioH;
    w = (w - ratioW * 25)/4, h = (h - ratioH * 25)/4;//每个小格子的宽度以及高度
    rX = 15 * ratioW, rY = 15 * ratioH;
    dPos[0] = QPointF(-25*ratioW, 0);   //25代表方格移动的速度是每秒25个单位
    dPos[1] = QPointF(25*ratioW, 0);    //这样每次定时器触发的时候就向左边绘制这么一个小矩形
    dPos[2] = QPointF(0, -25*ratioH);
    dPos[3] = QPointF(0, 25*ratioH);
    dPos[4] = QPointF(-4 * ratioW, -4 * ratioH);

}

void GameWidget::restart()
{
    curScore = 0;
    digitCount = 2;
    memset(board, 0, sizeof(board));
    init2Block();
    emit scoreInc(curScore);
    update();
}

bool GameWidget::checkGameOver()const
{
    //检测是否游戏结束，看下方以及右侧的数字与其是否相同
    for(int i = 0; i < 3; ++i){
        for(int j = 0; j < 3; ++j){
            if(board[i][j] == board[i+1][j] || board[i][j] == board[i][j+1])
                return false;
        }
    }
    for(int i = 0; i < 3; ++i){
        if(board[i][3] == board[i+1][3])
            return false;
    }
    for(int j = 0; j < 3; ++j){
        if(board[3][j] == board[3][j+1])
            return false;
    }
    return true;
}

bool GameWidget::checkWin()const
{
    for(int i = 0; i < 4; ++i){
        for(int j = 0; j < 4; ++j){
            if(board[i][j] == 2048)
                return true;
        }
    }
    return false;
}

int GameWidget::getDigitColor(int num)const
{
    int count = 0;
    while(!(num & 1)){
        num >>= 1;
        count++;
    }
    return count - 1; //返回2的幂数对应颜色的下标
}

