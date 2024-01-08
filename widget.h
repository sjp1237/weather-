#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QMenu>
#include<QAction>
#include <QContextMenuEvent>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    //设置退出
    QMenu* myExitMenu;
    QAction* myExitAction;

    void contextMenuEvent(QContextMenuEvent *event) override {
        myExitMenu->exec(event->globalPos());
    }

    //移动窗口
    bool isMousePressed;
    QPoint startPos;


    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;


};
#endif // WIDGET_H
