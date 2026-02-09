#include "QTestDialog.h"
#include <QFontDatabase>

QTestDialog::QTestDialog(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::QTestDialog)
{
	ui->setupUi(this);

	this->setFixedSize(this->size());

	this->setWindowTitle("  TestDialog");

	// 初始化标题字体
	m_titleFont = font();
	m_titleFont.setPointSize(14);
	m_titleFont.setBold(true);

	// 设置无边框
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

	// 关键：设置背景透明
	setAttribute(Qt::WA_TranslucentBackground, true);

	// 加载FontAwesome字体
	QString fontPath = ":/HuiRuWorkStation/ImageSource/fontawesome-webfont.ttf";
	int fontId = QFontDatabase::addApplicationFont(fontPath);
	if (fontId != -1)
	{
		QStringList families = QFontDatabase::applicationFontFamilies(fontId);
		if (!families.isEmpty())
		{
			m_iconFont = QFont(families.first());
#if (QT_VERSION >= QT_VERSION_CHECK(4,8,0))
			m_iconFont.setHintingPreference(QFont::PreferNoHinting);
#endif
		}
	}

	// 创建关闭按钮
	m_closeButton = new QPushButton(this);
	m_closeButton->setFixedSize(24, 24);

	//m_closeButton->setToolTip(tr("关闭"));

	// 设置FontAwesome字体
	m_closeButton->setFont(m_iconFont);

	// 设置关闭图标（FontAwesome的Unicode码）
	m_closeButton->setText(QChar(0xF00D));

	// 设置样式表
	m_closeButton->setStyleSheet(
		"QPushButton {"
		"  border: none;"
		"  background: transparent;"
		"  color: #FFFFFF;"
		"  font-size: 18px;"
		"}"
		"QPushButton:hover {"
		"  background: transparent;"
		"  color: #FF4D4D;"
		"}"
		"QPushButton:focus {"
		"  outline: none;"
		"  border: none;"
		"}"
	);

	// 关闭按钮点击事件
	connect(m_closeButton, &QPushButton::clicked, this, &QDialog::close);

}

QTestDialog::~QTestDialog()
{
	delete ui;
}

void QTestDialog::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	// 圆角裁剪
	QPainterPath path;
	path.addRoundedRect(rect(), m_borderRadius, m_borderRadius);
	painter.setClipPath(path);

	// 绘制背景
	painter.fillRect(rect(), QColor(245, 245, 245));

	// 绘制标题栏
	QRect titleRect(0, 0, width(), m_titleHeight);
	painter.fillRect(titleRect, m_titleBarColor);

	// 绘制标题文字
	painter.setPen(m_titleTextColor);
	painter.setFont(m_titleFont);
	painter.drawText(titleRect, Qt::AlignVCenter | Qt::AlignLeft, windowTitle());

	// 绘制边框
	QPen pen(m_titleBarColor, 2);
	painter.setPen(pen);
	painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), m_borderRadius, m_borderRadius);

}

void QTestDialog::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton && event->pos().y() <= m_titleHeight)
	{
		m_mousePressed = true;
		m_mousePressPos = event->globalPos() - frameGeometry().topLeft();
	}
}

void QTestDialog::mouseMoveEvent(QMouseEvent* event)
{
	if (m_mousePressed)
	{
		move(event->globalPos() - m_mousePressPos);
	}
}

void QTestDialog::mouseReleaseEvent(QMouseEvent* event)
{
	m_mousePressed = false;
}

void QTestDialog::resizeEvent(QResizeEvent* event)
{
	if (m_closeButton)
	{
		m_closeButton->move(width() - m_closeButton->width() - 8, (m_titleHeight - m_closeButton->height()) / 2);
	}
	QDialog::resizeEvent(event);
}
