#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_HuiRuWorkStation.h"
#include <QStandardItemModel>
#include <QColor>
#include <QRect>
#include <QFont>
#include <QPoint>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui { class HuiRuWorkStationClass; };
QT_END_NAMESPACE

class QCustomStateMachine;

class HuiRuWorkStation : public QMainWindow
{
    Q_OBJECT

public:
    HuiRuWorkStation(QWidget *parent = nullptr);
    ~HuiRuWorkStation();

    bool OnInitWindow();
	bool OnInitMachine();
    bool OnInitListView();
    bool OnInitStyle();
	bool OnInitTable();
	bool OnInitTable1();
	bool OnInitTable2();
    bool OnInitButton();
    bool OnInitLanguage();
	
    enum class ToolButtonType
    {
        Data0,
        Data1,
        Data2,
        Data3
    };

    void loadFromRegistry();
    void saveToRegistry() const;

private:
    Ui::HuiRuWorkStationClass *ui;

	QCustomStateMachine* m_machine = nullptr;

	bool m_testToolButtonState = false;

	QStandardItemModel* m_tableModel = nullptr;
	QStandardItemModel* m_tableModel2 = nullptr;

    // 标题栏配置
    QString m_windowTitle = "我的应用";
    int m_titleHeight = 35;
    QColor m_titleBarColor = QColor("#40444D");
    QColor m_titleTextColor = Qt::white;

    // 按钮状态
    QRect m_closeButtonRect;
    QRect m_minimizeButtonRect;

    // 字体
    QFont m_titleFont;
    QFont m_iconFont;

    // 拖拽
    bool m_dragging = false;
    QPoint m_dragStartPos;

    // 记录是否在按钮上按下
    bool m_closeButtonDown = false;
    bool m_closeButtonHovered = false;
    bool m_minimizeButtonHovered = false;
    bool m_minimizeButtonDown = false;

	// 页面映射
    QMap<QString, QWidget*> m_pageMap;

	// 创建MySQL连接
    QSqlDatabase createMysqlConnection(const QString& connectionName = "default_mysql");
    QSqlDatabase createSqliteConnection(const QString& dbFilePath, const QString& connectionName = "sqlite_conn");

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent*) override;
    void resizeEvent(QResizeEvent* event) override;

private slots:
	void on_toolButton_clicked();
	void on_toolButton_2_clicked();

    void onActionTriggered();

    void onNavPressed(const QString& text, const QString& parentText);
};
