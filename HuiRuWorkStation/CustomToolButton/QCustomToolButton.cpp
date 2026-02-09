#include "QCustomToolButton.h"
#include <QStylePainter>
#include <QStyleOptionToolButton>
#include <QMouseEvent>

QCustomToolButton::QCustomToolButton(QWidget *parent)
	: QToolButton(parent)
{

}

QCustomToolButton::~QCustomToolButton()
{

}

void QCustomToolButton::paintEvent(QPaintEvent* event)
{
    QStylePainter p(this);
    QStyleOptionButton opt;
    opt.initFrom(this);
    opt.text = text();
    opt.icon = icon();

    // 设置按下状态
    if (isDown() || isChecked())
    {
        opt.state |= QStyle::State_Sunken;
    }
    
    // 绘制整体按钮
    p.drawControl(QStyle::CE_PushButton, opt);

    // 绘制下拉箭头
    QStyleOptionToolButton arrowOpt;
    arrowOpt.initFrom(this);
    arrowOpt.rect = QRect(width() - 20, 0, 20, height());
    arrowOpt.state = QStyle::State_Enabled;

    if (isDown() || isChecked())
    {
        arrowOpt.state |= QStyle::State_Sunken;
    }
    p.drawPrimitive(QStyle::PE_IndicatorArrowDown, arrowOpt);

    // 箭头左侧竖线
    int lineX = width() - 20;
    p.drawLine(lineX, 6, lineX, height() - 6);
}

void QCustomToolButton::mousePressEvent(QMouseEvent* event)
{
    int arrowAreaStart = width() * 5 / 6;
    if (event->button() == Qt::LeftButton && event->x() >= arrowAreaStart)
    {
        if (menu())
        {
            showMenu();
            return;
        }
    }
    QToolButton::mousePressEvent(event);
}
