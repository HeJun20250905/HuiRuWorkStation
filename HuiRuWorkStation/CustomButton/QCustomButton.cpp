#include "QCustomButton.h"
#include <QPainter>
#include <QMouseEvent>
#include <QTimer>

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif

static constexpr double SLIDER_RATIO = 0.5;

QCustomSwitchButton::QCustomSwitchButton(QWidget* parent)
    : QWidget(parent)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(5);
    connect(m_timer, &QTimer::timeout, this, &QCustomSwitchButton::updatePosition);

    m_checked = true; // 初始为ON
    m_sliderX = m_checked ? width() - width() * SLIDER_RATIO : 0;
    m_targetX = m_sliderX;
}

void QCustomSwitchButton::mousePressEvent(QMouseEvent*)
{
    setChecked(!m_checked);
}

void QCustomSwitchButton::setChecked(bool checked)
{
    if (m_checked == checked) return;
    m_checked = checked;
    emit checkedChanged(checked);
    m_targetX = m_checked ? width() - width() * SLIDER_RATIO : 0;
    m_timer->start();
}

void QCustomSwitchButton::setBgColorOn(const QColor& color)
{
    if (m_bgColorOn != color)
    {
        m_bgColorOn = color;
        update();
    }
}

void QCustomSwitchButton::setBgColorOff(const QColor& color)
{
    if (m_bgColorOff != color)
    {
        m_bgColorOff = color;
        update();
    }
}

void QCustomSwitchButton::resizeEvent(QResizeEvent*)
{
    m_sliderX = m_checked ? width() - width() * SLIDER_RATIO : 0;
    m_targetX = m_sliderX;
}

void QCustomSwitchButton::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 背景
    QColor bg = m_checked ? m_bgColorOn : m_bgColorOff;
    painter.setPen(Qt::NoPen);
    painter.setBrush(bg);
    painter.drawRoundedRect(rect(), 5, 5);

    // 文字
    painter.setPen(Qt::white);
    QString text = m_checked ? m_textOn : m_textOff;
    int sliderWidth = width() * SLIDER_RATIO;
    QRect textRect(m_checked ? 0 : sliderWidth, 0, sliderWidth, height());
    painter.drawText(textRect, Qt::AlignCenter, text);

    // 滑块
    painter.setBrush(m_sliderColor);
    int sliderW = width() * SLIDER_RATIO - 4;
    int sliderH = height() - 4;
    QRect rect(m_sliderX + 2, 2, sliderW, sliderH);
    painter.drawRoundedRect(rect, 4, 4);

    // 滑块上的文字
    painter.setPen(Qt::black);         // 滑块上用黑色字体更清晰
    QFont font = painter.font();
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(rect, Qt::AlignCenter, "Data");
}

void QCustomSwitchButton::updatePosition()
{
    if (m_sliderX < m_targetX)
    {
        m_sliderX += m_step;
        if (m_sliderX >= m_targetX)
        {
            m_sliderX = m_targetX;
            m_timer->stop();
        }
    }
    else if (m_sliderX > m_targetX)
    {
        m_sliderX -= m_step;
        if (m_sliderX <= m_targetX)
        {
            m_sliderX = m_targetX;
            m_timer->stop();
        }
    }
    update();
}
