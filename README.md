# 项目概括
天气预报应用是一个基于Qt框架的天气信息展示应用，支持跨平台。用户可以通过搜索找到对应的城市，获取相对应的城市的实时的天气数据，包括当前天气、未来几天的预报、空气质量等。
并将实时信息展示到qt界面上。
![image](https://github.com/sjp1237/weather-/assets/78719366/9e91d94a-4362-4fb0-b6aa-e952dceb5403)

# UI设计
## 设置背景图片


# 功能实现
1. 实现了拖动窗口的功能，通过鼠标事件实现窗口的移动
2. 提供右键菜单，包括退出应用选项
3. 运行程序的时候，会自动显示广州的天气，包括当前天气、未来几天的预报、空气质量等
4. 用户点击 城市查询 获取相对应的城市的实时天气数据
5. 更新ui界面
6. 通过绘图实现高温曲线和低温曲线


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
**创建右键菜单**： 在 Widget 类的构造函数中创建了一个右键菜单 mExitMenu 和一个菜单项 mExitAction。

``` 
mExitMenu = new QMenu(this);
mExitAction = new QAction(this);
``` 
**设置菜单项**： 设置菜单项的文本和图标，并将其添加到右键菜单中。
``` 
mExitAction->setText("退出");
mExitAction->setIcon(QIcon(":/res/close.png"));
mExitMenu->addAction(mExitAction);
``` 
**连接槽函数**： 使用 connect 函数连接菜单项的触发信号与相应的槽函数。

``` 
connect(mExitAction, &QAction::triggered, this, [=]{
    qApp->exit();
});
``` 
上述代码表示当右键菜单的退出选项被触发时，执行 qApp->exit()，即退出应用程序。

**在右键菜单事件中弹出菜单**： 在 contextMenuEvent 函数中，调用右键菜单的 exec 函数以弹出右键菜单。

``` 
void Widget::contextMenuEvent(QContextMenuEvent *event)
{
    // 弹出右键菜单
    mExitMenu->exec(QCursor::pos());
    // 事件已经处理，不需要向上传
    event->accept();
}
```
当用户右键单机部件时，会触发QContextMenuEvent 事件
 QCursor::pos() 用于获取当前鼠标的全局位置，确保右键菜单在鼠标位置弹出。 event->accept() 表示事件已经被处理，不需要向上传递


## 搜索城市天气功能的实现
### 如何获取城市的天气数据？
**当需要获取某个城市天气数据json时候，需要传入一个城市代码编码作为入参，地址： http://t.weather.itboy.net/api/weather/city/xxxxx ，其中xxxxx即为城市代码**，获取到的数据是json格式的
![image](https://github.com/sjp1237/weather-/assets/78719366/eaf8e77e-ba69-4854-b83b-09b4ac541a88) 

### 通过城市名获取城市编码
1. 在citycode.json文件中，城市名与城市编码通过json格式映射在一起
![image](https://github.com/sjp1237/weather-/assets/78719366/01a52a5e-1895-46d2-a65a-203398e3a9ea)

2. weatherTool类的主要功能是从一个JSON文件中加载城市编码和城市名的映射关系，并将 城市名与城市编码映射到QMap里面
3. 提供一个静态函数 getCityCode，通过传入城市名来获取对应的城市编号


### 搜索城市天气功能实现的流程
1. **用户输入城市名**： 用户在界面上的搜索框中输入城市名，获取搜索框里面的城市名，将城市名转换为城市编码，并构建 url.
2. **发起网络请求**： 通过 **QNetworkAccessManager** 发起网络请求，向天气数据接口发送请求。
3. **处理网络响应**： 当网络请求完成后，会触发回调函数（在项目中是 onReplied 函数）。在该函数中，解析接口返回的 JSON 数据，提取所需的天气信息。
4. **更新界面**： 将获取到的天气信息更新到界面上，显示实时天气数据。


 ## 实现高温曲线和低温曲线
 当获取到所有的天气信息后，会将数据更新到
 1.**安装事件过滤器**:在 Widget 类中安装了一个事件过滤器 (eventFilter 函数)，该过滤器通过监视 ui->lblHighCurve 和 ui->lblLowCurve 控件的绘制事件
``` 
 bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->lblHighCurve && event->type() == QEvent::Paint) {
        paintHigtCurve();
    }
    if (watched == ui->lblLowCurve && event->type() == QEvent::Paint) {
        paintLowCurve();
    }
    return QWidget::eventFilter(watched, event);
}
``` 

2. **触发重绘操作**： 触发低温 和 高温 label控件重绘的语句，通过调用 update() 函数通知触发QEvent::Paint事件，事件也会被事件过滤器给捕获到
3. paintHigtCurve的实现
* 设置抗锯齿效果，以获得更加平滑的绘图效果
```
QPainter painter(ui->lblHighCurve);
painter.setRenderHint(QPainter::Antialiasing, true);
```
* 计算坐标和绘制点及文字
   x 坐标是基于日期和标签位置计算得到的
  ``` 
  pos[i].setX(mWeekList[i]->pos().x() + mWeekList[i]->width() / 2);
  ``` 
![image](https://github.com/sjp1237/weather-/assets/78719366/d23d5aa5-3444-483a-9c55-994fd8cf3980)

  y 坐标是根据温度偏离平均值的程度计算得到的
```
    //获取label的中心点
    float center = ui->lblHighCurve->height() / 2;
    for (int i = 0; i < 6; ++i) {
        //计算高温和
        tempSum += mDay[i].maximumTemperature;
    }
    //计算平均值
    tempAverage = tempSum / 6;
    //平均值作为中心点，然后与每个点在这个中心点 的偏移量进行上下移动，INCREMENT是移动的距离
    int offset = (mDay[i].ma，ximumTemperature - tempAverage) * INCREMENT;   
    pos[i].setY(center - offset);
```   
![image](https://github.com/sjp1237/weather-/assets/78719366/6a9b3737-a9b5-47dd-abb9-a6de6c9e807d)
