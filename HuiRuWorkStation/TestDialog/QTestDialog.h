#pragma once
#include <QDialog>
#include "ui_QTestDialog.h"
#include <QPaintEvent>
#include <QColor>
#include <QFont>
#include <QPoint>
#include <QPainter>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class QTestDialog; }
QT_END_NAMESPACE

class QTestDialog  : public QDialog
{
	Q_OBJECT

public:
	QTestDialog(QWidget *parent = nullptr);
	~QTestDialog();

private:
	Ui::QTestDialog *ui;

	// 自定义外观相关
	QColor m_titleBarColor = QColor(80, 80, 80);
	QColor m_titleTextColor = Qt::white;
	int m_borderRadius = 8;
	int m_titleHeight = 36;
	QFont m_titleFont;
	QFont m_iconFont;
	bool m_mousePressed = false;
	QPoint m_mousePressPos;
	QPushButton* m_closeButton = nullptr;

protected:
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;
};

