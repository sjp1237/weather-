# 项目概括
天气预报应用是一个基于Qt框架的天气信息展示应用，支持跨平台。用户可以通过搜索找到对应的城市，获取相对应的城市的实时的天气数据，包括当前天气、未来几天的预报、空气质量等。
并将实时信息展示到qt界面上。
![image](https://github.com/sjp1237/weather-/assets/78719366/9e91d94a-4362-4fb0-b6aa-e952dceb5403)

# 功能实现
1. 实现了拖动窗口的功能，通过鼠标事件实现窗口的移动
2. 提供右键菜单，包括退出应用选项
3. 运行程序的时候，会自动显示广州的天气，包括当前天气、未来几天的预报、空气质量等
4. 用户可以点击用户查询获取相对应的城市的实时天气数据
5. 通过绘图实现高温曲线和低温曲线


## 窗口拖动功能的实现
**1.记录鼠标点击位置的偏移量：**
当按下鼠标的时候，就会触发 mousePressEvent 函数，记录了鼠标点击位置相对于窗口左上角的偏移量，并将其保存在 mOffset 变量中。

```
void Widget::mousePressEvent(QMouseEvent *event)
{
    // 获取偏移
    mOffset = event->globalPosition().toPoint() - this->pos();
}
``` 
**2.处理鼠标移动事件：**
当用户在窗口上按下鼠标按钮后，并移动鼠标时，就会触发 mouseMoveEvent 函数，根据鼠标的相对偏移量实时移动窗口
``` 
void Widget::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos() - mOffset);
}
```
这里使用了 move 函数，该函数用于移动窗口到指定的位置，这里的指定位置是鼠标的全局位置减去之前记录的偏移量


## 右键菜单实现实现退出选项
创建右键菜单： 在 Widget 类的构造函数中创建了一个右键菜单 mExitMenu 和一个菜单项 mExitAction。

``` 
mExitMenu = new QMenu(this);
mExitAction = new QAction(this);
``` 
设置菜单项： 设置菜单项的文本和图标，并将其添加到右键菜单中。
``` 
mExitAction->setText("退出");
mExitAction->setIcon(QIcon(":/res/close.png"));
mExitMenu->addAction(mExitAction);
``` 
连接槽函数： 使用 connect 函数连接菜单项的触发信号与相应的槽函数。

``` 
connect(mExitAction, &QAction::triggered, this, [=]{
    qApp->exit();
});
``` 
上述代码表示当右键菜单的退出选项被触发时，执行 qApp->exit()，即退出应用程序。

在右键菜单事件中弹出菜单： 在 contextMenuEvent 函数中，调用右键菜单的 exec 函数以弹出右键菜单。

``` 
void Widget::contextMenuEvent(QContextMenuEvent *event)
{
    // 弹出右键菜单
    mExitMenu->exec(QCursor::pos());
    // 事件已经处理，不需要向上传
    event->accept();
}
``` 
上述代码中的 QCursor::pos() 用于获取当前鼠标的全局位置，确保右键菜单在鼠标位置弹出。 event->accept() 表示事件已经被处理，不需要向上传递
