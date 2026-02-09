#pragma once
#include <QToolButton>

class QCustomToolButton  : public QToolButton
{
	Q_OBJECT

public:
	QCustomToolButton(QWidget *parent);
	~QCustomToolButton();

protected:
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
};

