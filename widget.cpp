#include "widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    setWindowFlag(Qt::FramelessWindowHint);
    //设置窗口固定大小
    setFixedSize(QSize(800, 450));

    //构建菜单
    myExitMenu = new QMenu(this);
    myExitAction = new QAction(this);
    //设置菜单项的名字和图标
    myExitAction->setText("退出");
    myExitAction->setIcon(QIcon(":/res/close.png"));

    connect(myExitAction, &QAction::triggered, qApp, &QApplication::quit);
    myExitMenu->addAction(myExitAction);

}

Widget::~Widget()
{
    delete ui;
}



void Widget::mousePressEvent(QMouseEvent *event)  {
    if (event->button() == Qt::LeftButton) {
        isMousePressed = true;
        startPos = event->globalPos() - this->frameGeometry().topLeft();
    }
    QWidget::mousePressEvent(event);
}

void  Widget::mouseMoveEvent(QMouseEvent *event)  {
    if (isMousePressed && (event->buttons() & Qt::LeftButton)) {
        QPoint newPos = event->globalPos() - startPos;
        this->move(newPos);
    }
    QWidget::mouseMoveEvent(event);
}

void  Widget::mouseReleaseEvent(QMouseEvent *event)  {
    if (event->button() == Qt::LeftButton) {
        isMousePressed = false;
    }
    QWidget::mouseReleaseEvent(event);
}
