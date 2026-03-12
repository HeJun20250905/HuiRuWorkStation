#include "HuiRuWorkStation.h"
#include "TestDialog/QTestDialog.h"
#include <QMenu>
#include <QAction>
#include "CustomStateMachine/QCustomStateMachine.h"
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QFontDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlRecord>
#include <QList>
#include "CustomButton/QCustomButton.h"
#include "CustomListView/QCustomListView.h"

HuiRuWorkStation::HuiRuWorkStation(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HuiRuWorkStationClass())
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    setAttribute(Qt::WA_TranslucentBackground);

    ui->setupUi(this);

    this->OnInitWindow();
    
	this->OnInitMachine();

    this->OnInitListView();

	this->OnInitStyle();

    this->OnInitTable();

    this->OnInitButton();

    this->OnInitLanguage(); 
}

HuiRuWorkStation::~HuiRuWorkStation()
{
    delete ui;
}

void HuiRuWorkStation::on_toolButton_clicked()
{
	QTestDialog dialog(this);
	dialog.exec();
}


void HuiRuWorkStation::on_toolButton_2_clicked()
{
	if (m_testToolButtonState == false)
	{
		m_machine->Start();
		m_testToolButtonState = !m_testToolButtonState;
		emit m_machine->startRequested();
	}
	else
	{
		m_machine->Stop();
		m_testToolButtonState = !m_testToolButtonState;
		emit m_machine->stopRequested();
	}
}

void HuiRuWorkStation::onActionTriggered()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (!action) {
        qDebug() << "No action found";
        return;
    }

    bool ok = false;
    int typeInt = action->data().toInt(&ok); // 从 QVariant 取出 int
    if (!ok) {
        qDebug() << "Failed to convert data to int";
        return;
    }

    // 转为枚举
    ToolButtonType type = static_cast<ToolButtonType>(typeInt);

    // 用 switch 处理（高效、清晰）
    switch (type)
    {
    case ToolButtonType::Data0:
        QMessageBox::information(this, "Action Triggered", "Data 0 selected");
        qDebug() << "Data 0 selected";
        break;
    case ToolButtonType::Data1:
        QMessageBox::information(this, "Action Triggered", "Data 1 selected");
        qDebug() << "Data 1 selected";
        break;
    case ToolButtonType::Data2:
        QMessageBox::information(this, "Action Triggered", "Data 2 selected");
        qDebug() << "Data 2 selected";
        break;
    case ToolButtonType::Data3:
        QMessageBox::information(this, "Action Triggered", "Data 3 selected");
        qDebug() << "Data 3 selected";
        break;
    default:
        qDebug() << "Unknown ToolButtonType:" << typeInt;
        break;
    }
}

void HuiRuWorkStation::onNavPressed(const QString& text, const QString& parentText)
{
    qDebug() << "选中:" << text << ", 父项:" << parentText;

    // 构造唯一 key
    QString key;
    if (parentText.isEmpty()) 
    {
        // 顶级项：key 就是 text（如 "帮助文档"）
        key = text;
    }
    else 
    {
        // 子项：key = "父项/子项"
        key = parentText + "/" + text;
    }

    // 如果有映射，就切换页面
    if (m_pageMap.contains(key)) 
    {
        ui->stackedWidget->setCurrentWidget(m_pageMap[key]);
        qDebug() << "切换到页面:" << key;
    }
    else 
    {
        // 可选：对无页面的项做其他处理（如仅展开）
        qDebug() << "无对应页面:" << key;
    }
}

bool HuiRuWorkStation::OnInitWindow()
{
    // 初始化标题字体
    m_titleFont = font();
    m_titleFont.setPointSize(12);
    m_titleFont.setBold(true);

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

	// 启用鼠标跟踪
    setMouseTracking(true);
    for (auto* child : findChildren<QWidget*>())
    {
        child->setMouseTracking(true);
    }

    return true;
}

bool HuiRuWorkStation::OnInitMachine()
{
    m_machine = new QCustomStateMachine(this);
    connect(m_machine, &QCustomStateMachine::stateEntered, this, [](const QString& state)
    {
        qDebug() << "Current state:" << state;
    });

    return true;
}

bool HuiRuWorkStation::OnInitListView()
{
    QStringList items;
    items << "主界面||0||"
        << "主界面|主界面|||0xf03e"
        << "主界面2|主界面|||0xf03d"
        << "主界面3|主界面|||0xf03c"

        << "系统设置||0||"
        << "系统设置|系统设置|||0xf0e8"
        << "系统设置2|系统设置|||0xf047"
        << "系统设置3|系统设置|||0xf0b1"

        << "数据库||0||"
        << "数据库|数据库|||0xf03e"
        << "数据库2|数据库|||0xf030"
        << "数据库3|数据库|||0xf03d"
        << "数据库4|数据库|||0xf06a"

        << "帮助文档||0||"

        ;

    ui->listView->setItems(items.join(","));
    connect(ui->listView, 
        static_cast<void (NavListView::*)(const QString&, const QString&)>(&NavListView::pressed),
        this,
        &HuiRuWorkStation::onNavPressed);

    // 建立页面映射
    m_pageMap["主界面/主界面"]   = ui->page;
    m_pageMap["主界面/主界面2"]   = ui->page2;
    m_pageMap["系统设置/系统设置"] = ui->page3;
    m_pageMap["系统设置/系统设置2"] = ui->page4;
    m_pageMap["系统设置/系统设置3"] = ui->page5;
    
    return true;
}

bool HuiRuWorkStation::OnInitStyle()
{
    ui->stackedWidget->setStyleSheet(R"(
        QStackedWidget {
            border: 1px solid #CCCCCC;   /* 灰色边框 */
            border-radius: 4px;          /* 可选：圆角 */
            background-color: white;     /* 可选：背景色（如果不透明） */
        }
    )");

    ui->listView->setStyleSheet(R"(
        QListView {
            border: 1px solid #CCCCCC;
            border-radius: 6px;
            background-color: white;
        }
    )");

    return true;
}

bool HuiRuWorkStation::OnInitTable()
{
    this->OnInitTable1();
    this->OnInitTable2();

    return true;
}

bool HuiRuWorkStation::OnInitTable1()
{
    // 建立连接
    QSqlDatabase db = createMysqlConnection("table_view_conn");
    if (!db.isValid() || !db.isOpen())
    {
        qDebug() << "无法建立数据库连接,初始化空表格。";
        m_tableModel = new QStandardItemModel(0, 0, this);
        ui->tableView->setModel(m_tableModel);
        return false;
    }

    // 查询数据
    QString tableName = "HeJunSQL";
    QSqlQuery query(db);
    if (!query.exec("SELECT * FROM `" + tableName + "`"))
    {
        qDebug() << "SQL查询失败:" << query.lastError().text();
        m_tableModel = new QStandardItemModel(0, 0, this);
        ui->tableView->setModel(m_tableModel);
        return false;
    }

    // 结构数据
    QSqlRecord rec = query.record();
    int cols = rec.count();
    QList<QList<QVariant>> rows;

    while (query.next())
    {
        QList<QVariant> row;
        for (int i = 0; i < cols; ++i)
        {
            row << query.value(i);
        }
        rows << row;
    }

    // 构建模型
    m_tableModel = new QStandardItemModel(rows.size(), cols, this);

    // 表头
    QStringList headers;
    for (int i = 0; i < cols; ++i)
    {
        headers << rec.fieldName(i);
    }
    m_tableModel->setHorizontalHeaderLabels(headers);

    // 数据
    for (int r = 0; r < rows.size(); ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            auto item = new QStandardItem(rows[r][c].toString());
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item->setTextAlignment(Qt::AlignCenter);
            m_tableModel->setItem(r, c, item);
        }
    }

    // 视图与样式
    ui->tableView->setModel(m_tableModel);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setStyleSheet(
        "QTableView {"
        "    alternate-background-color: #F5F5F5;"
        "    background-color: #FFFFFF;"
        "}"
    );
    ui->tableView->horizontalHeader()->setStyleSheet(
        "QHeaderView::section {"
        "    background-color: #F5F5F5;"
        "    color: black;"
        "    font-weight: bold;"
        "}"
    );
    
    //ui->tableView->setColumnWidth(1, 30);
    ui->tableView->horizontalHeader()->setMinimumSectionSize(80);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setContextMenuPolicy(Qt::NoContextMenu);
    ui->tableView->setFocusPolicy(Qt::NoFocus);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    qDebug() << "表格已加载：" << rows.size() << "行 - " << cols << "列";

    return true;
}

bool HuiRuWorkStation::OnInitTable2()
{
    // 连接到SQLite数据库
    QString sqlitePath = "./hejun_data.db";
    QSqlDatabase db2 = createSqliteConnection(sqlitePath, "sqlite_table_view_conn");
    if (!db2.isValid() || !db2.isOpen())
    {
        qDebug() << "无法建立SQLite数据库连接,初始化空表格tableView_2 !";
        ui->tableView_2->setModel(new QStandardItemModel(0, 0, this));
        return false;
    }

    // 检查表HeJunSqLite是否存在,不存在则创建并填充数据 !
    QString tableName = "HeJunSqLite";
    QSqlQuery checkTable(db2);
    bool tableExists = false;

    // 查询sqlite_master系统表判断是否存在
    if (checkTable.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='" + tableName + "';")) 
    {
        tableExists = checkTable.next();
    }

    if (!tableExists) 
    {
        qDebug() << "表" << tableName << "不存在，正在创建并填充测试数据...";

        // 创建表: 10 列(col0, col1, ..., col9),类型均为 INTEGER
        QStringList columns;
        for (int i = 0; i < 10; ++i) 
        {
            columns << QString("col%1 INTEGER").arg(i);
        }
        QString createSql = QString("CREATE TABLE %1 (%2);").arg(tableName).arg(columns.join(", "));

        if (!QSqlQuery(db2).exec(createSql)) 
        {
            qWarning() << "创建表失败:" << db2.lastError().text();
            ui->tableView_2->setModel(new QStandardItemModel(0, 0, this));
            return false;
        }

        // 插入30行,每行10个值,全部为200
        QString placeholders = QString("?,").repeated(10).chopped(1);
        QString insertSql = QString("INSERT INTO %1 VALUES (%2);").arg(tableName).arg(placeholders);

        QSqlQuery insertQuery(db2);
        if (!insertQuery.prepare(insertSql)) 
        {
            qWarning() << "Prepare 失败:" << insertQuery.lastError().text();
            ui->tableView_2->setModel(new QStandardItemModel(0, 0, this));
            return false;
        }

        for (int row = 0; row < 30; ++row) 
        {
            for (int col = 0; col < 10; ++col) 
            {
                insertQuery.bindValue(col, 200);
            }
            if (!insertQuery.exec()) 
            {
                qWarning() << "插入第" << row << "行失败:" << insertQuery.lastError().text();
                break;
            }
        }

        qDebug() << "成功创建表" << tableName << "并插入 30 行测试数据。";
    }

    // 查询数据并加载到tableView_2
    QSqlQuery query(db2);
    if (!query.exec("SELECT * FROM `" + tableName + "`")) 
    {
        qDebug() << "SQLite查询失败:" << query.lastError().text();
        ui->tableView_2->setModel(new QStandardItemModel(0, 0, this));
        return false;
    }

    // 构建模型
    QSqlRecord rec = query.record();
    int cols = rec.count();
    QList<QList<QVariant>> rows;

    while (query.next()) 
    {
        QList<QVariant> row;
        for (int i = 0; i < cols; ++i) 
        {
            row << query.value(i);
        }
        rows << row;
    }

    QStandardItemModel* model = new QStandardItemModel(rows.size(), cols, this);

    // 表头
    QStringList headers;
    for (int i = 0; i < cols; ++i) 
    {
        headers << rec.fieldName(i);
    }
    model->setHorizontalHeaderLabels(headers);

    // 数据（不可编辑、居中）
    for (int r = 0; r < rows.size(); ++r) 
    {
        for (int c = 0; c < cols; ++c) 
        {
            auto item = new QStandardItem(rows[r][c].toString());
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item->setTextAlignment(Qt::AlignCenter);
            model->setItem(r, c, item);
        }
    }

    // 设置tableView_2样式
    ui->tableView_2->setModel(model);
    ui->tableView_2->setAlternatingRowColors(true);
    ui->tableView_2->setStyleSheet(
        "QTableView {"
        "    alternate-background-color: #F5F5F5;"
        "    background-color: #FFFFFF;"
        "}"
    );
    ui->tableView_2->horizontalHeader()->setStyleSheet(
        "QHeaderView::section {"
        "    background-color: #F5F5F5;"
        "    color: black;"
        "    font-weight: bold;"
        "}"
    );
    ui->tableView_2->horizontalHeader()->setMinimumSectionSize(80);
    ui->tableView_2->verticalHeader()->setVisible(false);
    ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_2->setContextMenuPolicy(Qt::NoContextMenu);
    ui->tableView_2->setFocusPolicy(Qt::NoFocus);
    ui->tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    qDebug() << "(tableView_2) 表格已加载：" << rows.size() << "行 - " << cols << "列";

    return true;
}

bool HuiRuWorkStation::OnInitButton()
{
    QMenu* menu = new QMenu(this);

    // 辅助lambda: 创建动作并连接
    auto addAction = [&](const QString& text, ToolButtonType type) 
    {
        QAction* action = menu->addAction(text);
        action->setData(static_cast<int>(type));
        connect(action, &QAction::triggered, this, &HuiRuWorkStation::onActionTriggered);
        return action;
    };

    // 使用通用名称, 不暴露业务语义
    addAction("Data 0", ToolButtonType::Data0);
    addAction("Data 1", ToolButtonType::Data1);
    addAction("Data 2", ToolButtonType::Data2);
    addAction("Data 3", ToolButtonType::Data3);

    ui->toolButton->setMenu(menu);
    ui->toolButton->setPopupMode(QToolButton::MenuButtonPopup);

    QToolButton* btn = ui->toolButton;
    btn->setStyleSheet(R"(
        QToolButton {
            background-color: rgb(111, 122, 126);
            color: rgb(0, 0, 0);
            border: 1.5px solid rgb(80, 80, 80);
            border-radius: 5px;
            padding: 5px 10px;
        }
        QToolButton:hover {
            background-color: rgb(91, 102, 106);
            border-color: rgb(70, 70, 70);
        }
        QToolButton:pressed {
            background-color: rgb(220, 220, 220);
            color: rgb(51, 51, 51);
            border-color: rgb(204, 204, 204);
        }
        QToolButton:disabled {
            background-color: rgb(90, 99, 102);
            color: rgb(160, 160, 160);
            border-color: rgb(90, 99, 102);
        }
    )");

    QToolButton* btn_2 = ui->toolButton_2;
    btn_2->setStyleSheet(R"(
        QToolButton {
            background-color: rgb(111, 122, 126);
            color: rgb(0, 0, 0);
            border: 1.5px solid rgb(80, 80, 80);
            border-radius: 5px;
            padding: 5px 10px;
        }
        QToolButton:hover {
            background-color: rgb(91, 102, 106);
            border-color: rgb(70, 70, 70);
        }
        QToolButton:pressed {
            background-color: rgb(220, 220, 220);
            color: rgb(51, 51, 51);
            border-color: rgb(204, 204, 204);
        }
        QToolButton:disabled {
            background-color: rgb(90, 99, 102);
            color: rgb(160, 160, 160);
            border-color: rgb(90, 99, 102);
        }
    )");

    return true;
}

bool HuiRuWorkStation::OnInitLanguage()
{

    return true;
}

void HuiRuWorkStation::loadFromRegistry()
{
    //QSettings reg("HKEY_CURRENT_USER\\Software\\AutoTronik-SMT\\MqttMes\\Conifgs", QSettings::NativeFormat);

    //hostName = reg.value("Host", "broker.hivemq.com").toString();
    //port = reg.value("Port", "1883").toInt();
    //userName = reg.value("User", "tungson").toString();
    //password = reg.value("Password", "123").toString();
    //clientId = reg.value("ClientId", "781").toString();
    //topic = reg.value("Topic", "MQTT Demo").toString();
    //nHeartbeat = reg.value("Heartbeat", "5000").toInt();
    //Recv = reg.value("Recv").toString();
}

void HuiRuWorkStation::saveToRegistry() const
{
    //QSettings reg("HKEY_CURRENT_USER\\Software\\AutoTronik-SMT\\MqttMes\\Conifgs", QSettings::NativeFormat);

    //reg.setValue("Host", hostName);
    //reg.setValue("Port", port);
    //reg.setValue("User", userName);
    //reg.setValue("Password", password);
    //reg.setValue("ClientId", clientId);
    //reg.setValue("Topic", topic);
    //reg.setValue("Heartbeat", nHeartbeat);
    //reg.setValue("Recv", Recv);
}

QSqlDatabase HuiRuWorkStation::createMysqlConnection(const QString& connectionName)
{
    // 防止重复注册同名连接
    if (QSqlDatabase::contains(connectionName)) 
    {
        QSqlDatabase::removeDatabase(connectionName);
    }

    // 创建新连接
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", connectionName);
    db.setHostName("localhost");
    db.setDatabaseName("test");
    db.setUserName("root");
    db.setPassword("root");
    db.setPort(3306);

    if (!db.open()) 
    {
        qDebug() << "MySQL 连接失败 [" << connectionName << "]:" << db.lastError().text();
        return QSqlDatabase();
    }

    qDebug() << "成功连接到 MySQL 数据库（连接名：" << connectionName << "）";
    return db;
}

QSqlDatabase HuiRuWorkStation::createSqliteConnection(const QString& dbFilePath, const QString& connectionName)
{
    // 防止重复注册同名连接
    if (QSqlDatabase::contains(connectionName)) 
    {
        QSqlDatabase::removeDatabase(connectionName);
    }

    // 添加 SQLite 驱动
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    db.setDatabaseName(dbFilePath);

    // 可选：设置连接选项（如启用外键）
    // db.setConnectOptions("QSQLITE_ENABLE_FKEY=1");

    if (!db.open()) 
    {
        qWarning() << "QLite连接失败 [" << connectionName << "] 文件:" << dbFilePath
            << "错误:" << db.lastError().text();
        return QSqlDatabase(); // 返回无效连接
    }

    qDebug() << "成功连接到SQLite数据库（连接名：" << connectionName << "），文件路径：" << dbFilePath;
    return db;
}

void HuiRuWorkStation::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    const int radius = 10;
    QRect fullRect = rect();

    // 创建圆角路径
    QPainterPath windowPath;
    windowPath.addRoundedRect(fullRect, radius, radius);

    // 设置裁剪
    painter.setClipPath(windowPath);

    // 填充背景
    painter.fillPath(windowPath, QColor(245, 245, 245));

    // 标题栏
    QRect titleRect(0, 0, width(), m_titleHeight);
    painter.fillRect(titleRect, m_titleBarColor);

    // 标题文字
    painter.setPen(m_titleTextColor);
    painter.setFont(m_titleFont);
    painter.drawText(titleRect, Qt::AlignVCenter | Qt::AlignLeft, "  HuiRuWorkStation");

    if (!m_iconFont.family().isEmpty())
    {
        QFont iconFont = m_iconFont;
        iconFont.setPixelSize(20);
        painter.setFont(iconFont);

        // 最小化按钮
        painter.setPen(m_minimizeButtonHovered ? QColor("#1E88E5") : Qt::white);
        painter.drawText(m_minimizeButtonRect, Qt::AlignCenter, QChar(0xf068));
    }

    if (!m_iconFont.family().isEmpty())
    {
        QFont iconFont = m_iconFont;
        iconFont.setPixelSize(24);
        painter.setFont(iconFont);

        // 关闭按钮
        painter.setPen(m_closeButtonHovered ? QColor("#FF4D4D") : Qt::white);
        painter.drawText(m_closeButtonRect, Qt::AlignCenter, QChar(0xf00d));
    }

    // 外边框
    QPen borderPen(QColor(90, 90, 90), 2);
    painter.setPen(borderPen);
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(fullRect.adjusted(1, 1, -1, -1), radius, radius);
}

void HuiRuWorkStation::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) 
    {
        if (event->pos().y() <= m_titleHeight) 
        {
            if (m_closeButtonRect.contains(event->pos())) 
            {
                m_closeButtonDown = true;
            }
            else if (m_minimizeButtonRect.contains(event->pos()))
            {
                m_minimizeButtonDown = true;
            }
            else 
            {
                m_dragging = true;
                m_dragStartPos = event->globalPos() - frameGeometry().topLeft();
            }
        }
    }
}
 
void HuiRuWorkStation::mouseMoveEvent(QMouseEvent* event)
{
    if (m_dragging)
    {
        move(event->globalPos() - m_dragStartPos);
        return;
    }

    bool oldCloseHover = m_closeButtonHovered;
    bool oldMinHover = m_minimizeButtonHovered;
    m_closeButtonHovered = m_closeButtonRect.contains(event->pos());
    m_minimizeButtonHovered = m_minimizeButtonRect.contains(event->pos());
    if (oldCloseHover != m_closeButtonHovered || oldMinHover != m_minimizeButtonHovered)
    {
        update();
    }
}

void HuiRuWorkStation::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) 
    {
        if (m_closeButtonDown && m_closeButtonRect.contains(event->pos())) 
        {
            close();
        }
        if (m_minimizeButtonDown && m_minimizeButtonRect.contains(event->pos()))
        {
            showMinimized();
        }
        m_dragging = false;
        m_closeButtonDown = false;
        m_minimizeButtonDown = false;
    }
}

void HuiRuWorkStation::leaveEvent(QEvent*)
{
    if (m_closeButtonHovered || m_minimizeButtonHovered)
    {
        m_closeButtonHovered = false;
        m_minimizeButtonHovered = false;
        update();
    }
}

void HuiRuWorkStation::resizeEvent(QResizeEvent* event)
{
    m_closeButtonRect = QRect(width() - 24 - 12, 6, 24, 24);
    m_minimizeButtonRect = QRect(width() - 20 - 12 - 20 - 16, 8, 20, 20);
    QMainWindow::resizeEvent(event);
}