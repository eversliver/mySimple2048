#include "windowwidget.h"

WindowWidget::WindowWidget(QWidget * parent)
    : QWidget(parent)
{
    //初始化最高分
    highScore = 0;
    QFile file("score.j");
    if(file.open(QIODevice::ReadOnly)){
        file.read((char*)&highScore, sizeof(highScore));
        file.close();
    }

    //新建一个gameWidget,用于逻辑方面的操作
    gameWidget = new GameWidget(this);
    gameWidget->setGeometry(2, 200, 400, 400);
    connect(gameWidget, SIGNAL(scoreInc(int)), this, SLOT(onScoreInc(int)));
    connect(gameWidget, SIGNAL(gameOver()), this, SLOT(onGameOver()));
    connect(gameWidget, SIGNAL(win()), this, SLOT(onWin()));

    QFont font;
    font.setFamily("Consolas");
    font.setBold(true);
    font.setPixelSize(25);


    restartButton = new QPushButton("Restart", this);   //分配的时候一定要加上this指针，这样才是在当前的widget上面放置一个新的部件
    restartButton->setGeometry(100,120,200,50);
    restartButton->setFont(font);
    restartButton->setStyleSheet(QString(BTNSTYLESHEET).arg(3).arg(15));
    connect(restartButton, SIGNAL(clicked()), this->gameWidget, SLOT(restart()));

    highScoreLabel = new QLabel(QString("High Score:\n%1").arg(highScore), this);
    highScoreLabel->setGeometry(209,20,180,70);
    highScoreLabel->setFont(font);
    highScoreLabel->setAlignment(Qt::AlignCenter);
    highScoreLabel->setStyleSheet(QString(LBLSTYLESHEET).arg(5).arg(20));

    scoreLabel = new QLabel(QString("Score:\n0"),this);//初始化设置分数label上的值为0
    scoreLabel->setGeometry(15,20,180,70);
    scoreLabel->setFont(font);
    scoreLabel->setAlignment(Qt::AlignCenter);
    scoreLabel->setStyleSheet(QString(LBLSTYLESHEET).arg(5).arg(20));

    resize(404, 606);
}

WindowWidget::~WindowWidget()
{
    delete restartButton;
    delete scoreLabel;
    delete highScoreLabel;
    delete gameWidget;
}

void WindowWidget::onScoreInc(int score)
{
    scoreLabel->setText(QString("Score:\n%1").arg(score));
    if(score > highScore){
        //首先设置最高分
        highScore = score;
        highScoreLabel->setText(QString("High Score:\n%1").arg(score));
        QFile file("score.j");//然后记录最高分
        file.open(QIODevice::WriteOnly);
        file.write((char*)&highScore, sizeof(highScore));
        file.close();
    }
}

void WindowWidget::onGameOver()
{
    QMessageBox::information(this, "GameOver", "You Lose!");
}

void WindowWidget::onWin()
{
    QMessageBox::information(this, "Congratulation", "You Win!");
}

void WindowWidget::resizeEvent(QResizeEvent * )
{
    //按照比例缩放
    ratioW = width()/404.0;
    ratioH = height()/606.0;

    QFont font;
    font.setFamily("Consolas");
    font.setBold(true);
    font.setPixelSize(25*ratioH);
    restartButton->setFont(font);
    highScoreLabel->setFont(font);
    scoreLabel->setFont(font);
    //restartButton->setStyleSheet(QString(BTNSTYLESHEET).arg(3*ratioW).arg(15*ratioH));
    //highScoreLabel->setStyleSheet(QString(LBLSTYLESHEET).arg(5*ratioW).arg(20*ratioH));
    //scoreLabel->setStyleSheet(QString(LBLSTYLESHEET).arg(5*ratioW).arg(20*ratioH));
    qDebug() << "ratioW: " << ratioW << " " << ratioH ;
    gameWidget->setGeometry(2*ratioW, 200 * ratioH, 400 * ratioW, 400 * ratioH);
    restartButton->setGeometry(100 * ratioW, 120 * ratioH, 200 * ratioW, 50 * ratioH);
    scoreLabel->setGeometry(15 * ratioW, 20 * ratioH, 180 * ratioW, 70 * ratioH);
    highScoreLabel->setGeometry(209 * ratioW, 20 * ratioH, 180 * ratioW, 70 * ratioH);
}

