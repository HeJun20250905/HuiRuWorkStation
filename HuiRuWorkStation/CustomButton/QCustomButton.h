#ifndef QCUSTOMSWITCHBUTTON_H
#define QCUSTOMSWITCHBUTTON_H

#include <QWidget>
#include <QTimer>
#include <QColor>
#include <QString>

class QCustomSwitchButton : public QWidget
{
    Q_OBJECT
        Q_PROPERTY(bool checked READ isChecked WRITE setChecked NOTIFY checkedChanged)
        Q_PROPERTY(QColor bgColorOn READ bgColorOn WRITE setBgColorOn)
        Q_PROPERTY(QColor bgColorOff READ bgColorOff WRITE setBgColorOff)

public:
    explicit QCustomSwitchButton(QWidget* parent = nullptr);

    bool isChecked() const { return m_checked; }
    QColor bgColorOn() const { return m_bgColorOn; }
    QColor bgColorOff() const { return m_bgColorOff; }

    void setChecked(bool checked);
    void setBgColorOn(const QColor& color);
    void setBgColorOff(const QColor& color);

signals:
    void checkedChanged(bool checked);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void updatePosition();

private:
    bool m_checked = false;
    QColor m_bgColorOff = QColor(111, 122, 126);
    QColor m_bgColorOn = QColor(111, 122, 126);
    QColor m_sliderColor = QColor(220, 220, 220);

    QString m_textOff = QString::fromUtf8("Down");
    QString m_textOn = QString::fromUtf8("UP");

    int m_sliderX = 0;
    int m_targetX = 0;
    int m_step = 2;

    QTimer* m_timer = nullptr;
};

#endif // QCUSTOMSWITCHBUTTON_H
