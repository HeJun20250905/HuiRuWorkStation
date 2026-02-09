#include "QCustomListView.h"
#include <QFontDatabase>
#include <QPainter>
#include <QPainterPath>
#include <QDebug>
#include <QAbstractItemView>

// 强制触发编译错误，验证类是否可见
static_assert(std::is_base_of<QListView, NavListView>::value, "NavListView not derived from QListView!");

// NavDelegate Implementation
NavDelegate::NavDelegate(QObject* parent) : QStyledItemDelegate(parent)
{
    nav = qobject_cast<NavListView*>(parent);

    // 单例加载FontAwesome字体
    static bool fontAwesomeLoaded = false;
    if (!fontAwesomeLoaded) 
    {
        QString fontPath = ":/HuiRuWorkStation/ImageSource/fontawesome-webfont.ttf"; // 路径必须和.qrc一致
        int fontId = QFontDatabase::addApplicationFont(fontPath);
        if (fontId == -1) 
        {
            qWarning() << "[NavDelegate] Failed to load FontAwesome from:" << fontPath;
        }
        else 
        {
            QStringList families = QFontDatabase::applicationFontFamilies(fontId);
            qDebug() << "[NavDelegate] Loaded FontAwesome families:" << families;
        }
        fontAwesomeLoaded = true;
    }

    // 设置iconFont
    if (QFontDatabase().families().contains("FontAwesome")) 
    {
        iconFont = QFont("FontAwesome");
#if (QT_VERSION >= QT_VERSION_CHECK(4,8,0))
        iconFont.setHintingPreference(QFont::PreferNoHinting);
#endif
    }
    else 
    {
        qWarning() << "[NavDelegate] 'FontAwesome' not found in system fonts!";
        // 回退默认字体
        iconFont = QFont();
    }
}

NavDelegate::~NavDelegate() 
{

}

QSize NavDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    NavModel::TreeNode* node = reinterpret_cast<NavModel::TreeNode*>(index.data(Qt::UserRole).toULongLong());
    bool parent = (node->level == 1);
    QSize size(50, parent ? nav->getParentHeight() : nav->getChildHeight());
    return size;
}

void NavDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->setRenderHint(QPainter::Antialiasing);
    NavModel::TreeNode* node = reinterpret_cast<NavModel::TreeNode*>(index.data(Qt::UserRole).toULongLong());

    QRect optionRect = option.rect;
    int x = optionRect.x();
    int y = optionRect.y();
    int width = optionRect.width();
    int height = optionRect.height();
    int fontSize = nav->getParentFontSize();
    bool parent = (node->level == 1);

    QColor bgColor, textColor, tipBgColor, tipTextColor, iconColor;
    if (option.state & QStyle::State_Selected) 
    {
        bgColor = parent ? nav->getParentBgSelectedColor() : nav->getChildBgSelectedColor();
        textColor = parent ? nav->getParentTextSelectedColor() : nav->getChildTextSelectedColor();
        tipBgColor = parent ? nav->getParentTextSelectedColor() : nav->getChildTextSelectedColor();
        tipTextColor = parent ? nav->getParentBgSelectedColor() : nav->getChildBgSelectedColor();
        iconColor = parent ? nav->getParentTextSelectedColor() : nav->getChildTextSelectedColor();
    }
    else if (option.state & QStyle::State_MouseOver) 
    {
        bgColor = parent ? nav->getParentBgHoverColor() : nav->getChildBgHoverColor();
        textColor = parent ? nav->getParentTextHoverColor() : nav->getChildTextHoverColor();
        tipBgColor = parent ? nav->getParentTextSelectedColor() : nav->getChildTextSelectedColor();
        tipTextColor = parent ? nav->getParentBgSelectedColor() : nav->getChildBgSelectedColor();
        iconColor = parent ? nav->getParentTextHoverColor() : nav->getChildTextHoverColor();
    }
    else 
    {
        bgColor = parent ? nav->getParentBgNormalColor() : nav->getChildBgNormalColor();
        textColor = parent ? nav->getParentTextNormalColor() : nav->getChildTextNormalColor();
        tipBgColor = parent ? nav->getParentBgSelectedColor() : nav->getChildBgSelectedColor();
        tipTextColor = parent ? nav->getParentTextSelectedColor() : nav->getChildTextSelectedColor();
        iconColor = parent ? nav->getParentTextNormalColor() : nav->getChildTextNormalColor();
    }

    QPainterPath bgPath;
    QRect rect = option.rect;

    bool isFirstItem = (index.row() == 0);
    bool isParent = (node->level == 1);

    if (isFirstItem && isParent) 
    {
        int radius = qMin(10, rect.height() / 6);
        if (radius > 0) 
        {
            // 转为浮点矩形
            QRectF rct(rect); 
            qreal r = static_cast<qreal>(radius);

            // 左上圆角矩形
            QRectF topLeftArc(rct.left(), rct.top(), r * 2, r * 2);

            // 右上圆角矩形
            QRectF topRightArc(rct.right() - r * 2, rct.top(), r * 2, r * 2);

            // 构造路径
            bgPath.moveTo(rct.left(), rct.bottom());
            bgPath.lineTo(rct.left(), rct.top() + r);
            bgPath.arcTo(topLeftArc, 180, -90);
            bgPath.lineTo(rct.right() - r, rct.top());
            bgPath.arcTo(topRightArc, 90, -90);
            bgPath.lineTo(rct.right(), rct.bottom());
            bgPath.closeSubpath();
        }
        else 
        {
            bgPath.addRect(rect);
        }
    }
    else 
    {
        bgPath.addRect(rect);
    }

    painter->fillPath(bgPath, bgColor);
    
    // 绘制竖线
    int lineWidth = nav->getLineWidth();
    if (!parent && nav->getLineVisible() && lineWidth > 0) 
    {
        if ((option.state & QStyle::State_Selected) || (option.state & QStyle::State_MouseOver)) 
        {
            float offset = static_cast<float>(lineWidth) / 2;
            QPointF pointTop(x, y + offset);
            QPointF pointBottom(x, y + height - offset);
            if (!nav->getLineLeft()) 
            {
                pointTop.setX(width);
                pointBottom.setX(width);
            }
            painter->setPen(QPen(nav->getLineColor(), lineWidth));
            painter->drawLine(pointTop, pointBottom);
        }
    }

    // 绘制三角形
    int triangleWidth = nav->getTriangleWidth();
    if (!parent && nav->getTriangleVisible() && triangleWidth > 0) 
    {
        if ((option.state & QStyle::State_Selected) || (option.state & QStyle::State_MouseOver)) 
        {
            QFont font = iconFont;
            font.setPixelSize(fontSize + triangleWidth);
            painter->setFont(font);
            painter->setPen(nav->getTriangleColor());
            if (nav->getTriangleLeft()) 
            {
                painter->drawText(optionRect, Qt::AlignLeft | Qt::AlignVCenter, QChar(0xf0da));
            }
            else 
            {
                painter->drawText(optionRect, Qt::AlignRight | Qt::AlignVCenter, QChar(0xf0d9));
            }
        }
    }

    // 分隔线
    if (nav->getSeparateVisible()) 
    {
        if (node->level == 1 || (node->level == 2 && node->last)) 
        {
            painter->setPen(QPen(nav->getSeparateColor(), nav->getSeparateHeight()));
            painter->drawLine(QPointF(x, y + height), QPointF(x + width, y + height));
        }
    }

    // 文字
    QString text = node->text;
    if (!text.isEmpty()) 
    {
        int margin = nav->getParentMargin();
        if (node->level == 2) 
        {
            margin = nav->getChildMargin();
            fontSize = nav->getChildFontSize();
        }
        QRect textRect = optionRect;
        textRect.setWidth(width - margin);
        textRect.setX(x + margin);
        QFont font;
        font.setPixelSize(fontSize);
        painter->setFont(font);
        painter->setPen(textColor);
        painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, text);
    }

    // 提示 badge
    QString tip = node->tip;
    if (nav->getTipVisible() && !tip.isEmpty()) 
    {
        if (tip.toInt() > 0) 
        {
            tip = (tip.toInt() > 999) ? "999+" : tip;
        }
        else if (tip.length() > 2) 
        {
            tip = tip.left(2) + " .";
        }
        int radius = height / 4;
        QRect tipRect = optionRect;
        tipRect.setHeight(radius * 2);
        tipRect.moveCenter(optionRect.center());
        tipRect.setLeft(optionRect.right() - nav->getTipWidth() - 5);
        tipRect.setRight(optionRect.right() - 5);
        QFont font;
        font.setPixelSize(fontSize - 2);
        painter->setFont(font);
        painter->setPen(Qt::NoPen);
        painter->setBrush(tipBgColor);
        painter->drawRoundedRect(tipRect, radius, radius);
        painter->setPen(tipTextColor);
        painter->setBrush(Qt::NoBrush);
        painter->drawText(tipRect, Qt::AlignCenter, tip);
    }

    // 左侧图标
    QRect iconRect = optionRect;
    iconRect.setLeft(parent ? nav->getParentIconMargin() : nav->getChildIconMargin());
    QFont font = iconFont;
    font.setPixelSize(fontSize);
    painter->setFont(font);
    painter->setPen(textColor);
    if (!node->icon.isNull()) 
    {
        painter->drawText(iconRect, Qt::AlignLeft | Qt::AlignVCenter, node->icon);
    }
    else if (parent) 
    {
        painter->drawText(iconRect, Qt::AlignLeft | Qt::AlignVCenter,
            node->expand ? QChar(0xf067) : QChar(0xf068));
    }

    // 右侧展开图标
    iconRect.setRight(optionRect.width() - 10);
    if (!(nav->getTipVisible() && !node->tip.isEmpty()) &&
        nav->getRightIconVisible() && parent) 
    {
        painter->drawText(iconRect, Qt::AlignRight | Qt::AlignVCenter,
            node->expand ? QChar(0xf054) : QChar(0xf078));
    }
}

// NavModel Implementation
NavModel::NavModel(QObject* parent) : QAbstractListModel(parent) 
{

}

NavModel::~NavModel()
{
    for (auto it = treeNode.begin(); it != treeNode.end();) 
    {
        for (auto child = (*it)->children.begin(); child != (*it)->children.end();) 
        {
            delete* child;
            child = (*it)->children.erase(child);
        }
        delete* it;
        it = treeNode.erase(it);
    }
}

int NavModel::rowCount(const QModelIndex&) const 
{ 
    return listNode.size(); 
}

QVariant NavModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= listNode.size())
        return QVariant();
    if (role == Qt::DisplayRole) return listNode.at(index.row()).text;
    if (role == Qt::UserRole) return quint64(listNode.at(index.row()).treeNode);
    return QVariant();
}

void NavModel::setItems(const QStringList& items)
{
    int count = items.count();
    if (count == 0) return;

    treeNode.clear();
    listNode.clear();
    bool ok;

    for (int i = 0; i < count; ++i) 
    {
        QStringList list = items[i].split("|");
        if (list.size() < 5) continue;
        QString text = list[0];
        QString parentText = list[1];
        QString expand = list[2];
        QString tip = list[3];
        QString icon = list[4];

        if (parentText.isEmpty()) 
        {
            TreeNode* node = new TreeNode;
            node->level = 1;
            node->expand = expand.toInt();
            node->last = false;
            if (!icon.isEmpty()) node->icon = QChar(icon.toInt(&ok, 16));
            node->text = text;
            node->tip = tip;
            node->parentText = parentText;

            for (int j = 0; j < count; ++j) 
            {
                QStringList childList = items[j].split("|");
                if (childList.size() < 5) continue;
                QString childText = childList[0];
                QString childParentText = childList[1];
                QString childTip = childList[3];
                QString childIcon = childList[4].replace("0x", "");
                if (childParentText == text) 
                {
                    TreeNode* childNode = new TreeNode;
                    childNode->level = 2;
                    childNode->expand = false;
                    childNode->last = (j == count - 1);
                    if (!childIcon.isEmpty()) childNode->icon = QChar(childIcon.toInt(&ok, 16));
                    childNode->text = childText;
                    childNode->tip = childTip;
                    childNode->parentText = childParentText;
                    node->children.push_back(childNode);
                }
            }
            treeNode.push_back(node);
        }
    }

    refreshList();
    beginResetModel();
    endResetModel();
}

void NavModel::expand(const QModelIndex& index)
{
    TreeNode* node = listNode.at(index.row()).treeNode;
    if (node->children.isEmpty()) return;

    node->expand = !node->expand;
    int first = index.row() + 1;
    int last = first + static_cast<int>(node->children.size()) - 1;

    beginResetModel();
    refreshList();
    endResetModel();
}

void NavModel::refreshList()
{
    listNode.clear();
    for (auto it = treeNode.begin(); it != treeNode.end(); ++it) 
    {
        ListNode node;
        node.text = (*it)->text;
        node.treeNode = *it;
        listNode.push_back(node);
        if (!(*it)->expand) continue;
        for (auto child = (*it)->children.begin(); child != (*it)->children.end(); ++child) 
        {
            ListNode n;
            n.text = (*child)->text;
            n.treeNode = *child;
            n.treeNode->last = false;
            listNode.push_back(n);
        }
        if (!listNode.empty()) listNode.back().treeNode->last = true;
    }
}

// NavListView Implementation
NavListView::NavListView(QWidget* parent) 
    : QListView(parent)
{
    // 初始化默认值
    rightIconVisible = true;
    tipVisible = true;
    tipWidth = 40;
    separateVisible = true;
    separateHeight = 1;
    separateColor = QColor(40, 43, 51);
    lineLeft = true;
    lineVisible = true;
    lineWidth = 6;
    lineColor = QColor(0, 187, 158);
    triangleLeft = false;
    triangleVisible = true;
    triangleWidth = 6;
    triangleColor = QColor(250, 250, 250);

    parentIconMargin = 10;
    parentMargin = 30;
    parentFontSize = 12;
    parentHeight = 35;
    parentBgNormalColor = QColor(57, 61, 73);
    parentBgSelectedColor = QColor(78, 83, 102);
    parentBgHoverColor = QColor(78, 83, 102);
    parentTextNormalColor = QColor(250, 250, 250);
    parentTextSelectedColor = QColor(250, 250, 250);
    parentTextHoverColor = QColor(250, 250, 250);

    childIconMargin = 15;
    childMargin = 35;
    childFontSize = 12;
    childHeight = 30;
    childBgNormalColor = QColor(40, 43, 51);
    childBgSelectedColor = QColor(20, 20, 20);
    childBgHoverColor = QColor(20, 20, 20);
    childTextNormalColor = QColor(180, 180, 180);
    childTextSelectedColor = QColor(250, 250, 250);
    childTextHoverColor = QColor(255, 255, 255);

    expendMode = ExpendMode_SingleClick;

    setMouseTracking(true);
    model = new NavModel(this);
    delegate = new NavDelegate(this);
    setModel(model);
    setItemDelegate(delegate);

    connect(this,
        static_cast<void (QAbstractItemView::*)(const QModelIndex&)>(&QAbstractItemView::pressed),
        this,
        &NavListView::onItemPressed);

    connect(this, &NavListView::clicked, model, &NavModel::expand);

    setStyleSheet("QListView { background: #F5F5F5; }");
}

NavListView::~NavListView()
{
    delete model;
    delete delegate;
}

void NavListView::onItemPressed(const QModelIndex& data)
{
    NavModel::TreeNode* node = reinterpret_cast<NavModel::TreeNode*>(data.data(Qt::UserRole).toULongLong());
    QString text = node->text;
    QString parentText = node->parentText;
    int parentIndex = parentItem.indexOf(parentText);
    int index = parentItem.indexOf(text);
    if (parentIndex >= 0) 
    {
        index = childItem.at(parentIndex).indexOf(text);
    }
    int childIndex = -1;
    for (int i = 0; i < childItem.size(); ++i) 
    {
        const QStringList& item = childItem[i];
        for (const QString& str : item) 
        {
            childIndex++;
            if (str == text) goto found;
        }
    }
found:
    if (parentIndex == -1) childIndex = -1;
    emit pressed(text, parentText);
    emit pressed(index, parentIndex);
    emit pressed(childIndex);
}

void NavListView::setData(const QStringList& listItems)
{
    model->setItems(listItems);
}

// ========== Getters ==========
bool NavListView::getRightIconVisible() const { return rightIconVisible; }
bool NavListView::getTipVisible() const { return tipVisible; }
int NavListView::getTipWidth() const { return tipWidth; }
bool NavListView::getSeparateVisible() const { return separateVisible; }
int NavListView::getSeparateHeight() const { return separateHeight; }
QColor NavListView::getSeparateColor() const { return separateColor; }
bool NavListView::getLineLeft() const { return lineLeft; }
bool NavListView::getLineVisible() const { return lineVisible; }
int NavListView::getLineWidth() const { return lineWidth; }
QColor NavListView::getLineColor() const { return lineColor; }
bool NavListView::getTriangleLeft() const { return triangleLeft; }
bool NavListView::getTriangleVisible() const { return triangleVisible; }
int NavListView::getTriangleWidth() const { return triangleWidth; }
QColor NavListView::getTriangleColor() const { return triangleColor; }

int NavListView::getParentIconMargin() const { return parentIconMargin; }
int NavListView::getParentMargin() const { return parentMargin; }
int NavListView::getParentFontSize() const { return parentFontSize; }
int NavListView::getParentHeight() const { return parentHeight; }
QColor NavListView::getParentBgNormalColor() const { return parentBgNormalColor; }
QColor NavListView::getParentBgSelectedColor() const { return parentBgSelectedColor; }
QColor NavListView::getParentBgHoverColor() const { return parentBgHoverColor; }
QColor NavListView::getParentTextNormalColor() const { return parentTextNormalColor; }
QColor NavListView::getParentTextSelectedColor() const { return parentTextSelectedColor; }
QColor NavListView::getParentTextHoverColor() const { return parentTextHoverColor; }

int NavListView::getChildIconMargin() const { return childIconMargin; }
int NavListView::getChildMargin() const { return childMargin; }
int NavListView::getChildFontSize() const { return childFontSize; }
int NavListView::getChildHeight() const { return childHeight; }
QColor NavListView::getChildBgNormalColor() const { return childBgNormalColor; }
QColor NavListView::getChildBgSelectedColor() const { return childBgSelectedColor; }
QColor NavListView::getChildBgHoverColor() const { return childBgHoverColor; }
QColor NavListView::getChildTextNormalColor() const { return childTextNormalColor; }
QColor NavListView::getChildTextSelectedColor() const { return childTextSelectedColor; }
QColor NavListView::getChildTextHoverColor() const { return childTextHoverColor; }

NavListView::ExpendMode NavListView::getExpendMode() const { return expendMode; }

QSize NavListView::sizeHint() const { return QSize(200, 300); }
QSize NavListView::minimumSizeHint() const { return QSize(20, 20); }

// ========== Setters ==========
void NavListView::setItems(const QString& items)
{
    if (this->items != items) 
    {
        this->items = items;
        QStringList item = items.split(",");
        setData(item);

        parentItem.clear();
        childItem.clear();
        int count = item.size();
        for (int i = 0; i < count; ++i) 
        {
            QStringList list = item[i].split("|");
            if (list.size() < 5) continue;
            QString text = list[0];
            QString parentText = list[1];
            if (parentText.isEmpty()) 
            {
                QStringList childs;
                for (int j = 0; j < count; ++j) 
                {
                    QStringList childList = item[j].split("|");
                    if (childList.size() < 5) continue;
                    QString childText = childList[0];
                    QString childParentText = childList[1];
                    if (childParentText == text) 
                    {
                        childs << childText;
                    }
                }
                parentItem << text;
                childItem << childs;
            }
        }
    }
}

void NavListView::setCurrentRow(int row)
{
    setCurrentIndex(model->index(row, 0));
}

void NavListView::setRightIconVisible(bool rightIconVisible) { this->rightIconVisible = rightIconVisible; }
void NavListView::setTipVisible(bool tipVisible) { this->tipVisible = tipVisible; }
void NavListView::setTipWidth(int tipWidth) { this->tipWidth = tipWidth; }
void NavListView::setSeparateVisible(bool separateVisible) { this->separateVisible = separateVisible; }
void NavListView::setSeparateHeight(int separateHeight) { this->separateHeight = separateHeight; }
void NavListView::setSeparateColor(const QColor& separateColor) { this->separateColor = separateColor; }
void NavListView::setLineLeft(bool lineLeft) { this->lineLeft = lineLeft; }
void NavListView::setLineVisible(bool lineVisible) { this->lineVisible = lineVisible; }
void NavListView::setLineWidth(int lineWidth) { this->lineWidth = lineWidth; }
void NavListView::setLineColor(const QColor& lineColor) { this->lineColor = lineColor; }
void NavListView::setTriangleLeft(bool triangleLeft) { this->triangleLeft = triangleLeft; }
void NavListView::setTriangleVisible(bool triangleVisible) { this->triangleVisible = triangleVisible; }
void NavListView::setTriangleWidth(int triangleWidth) { this->triangleWidth = triangleWidth; }
void NavListView::setTriangleColor(const QColor& triangleColor) { this->triangleColor = triangleColor; }

void NavListView::setParentIconMargin(int parentIconMargin) { this->parentIconMargin = parentIconMargin; }
void NavListView::setParentMargin(int parentMargin) { this->parentMargin = parentMargin; }
void NavListView::setParentFontSize(int parentFontSize) { this->parentFontSize = parentFontSize; }
void NavListView::setParentHeight(int parentHeight) { this->parentHeight = parentHeight; }
void NavListView::setParentBgNormalColor(const QColor& parentBgNormalColor) { this->parentBgNormalColor = parentBgNormalColor; }
void NavListView::setParentBgSelectedColor(const QColor& parentBgSelectedColor) { this->parentBgSelectedColor = parentBgSelectedColor; }
void NavListView::setParentBgHoverColor(const QColor& parentBgHoverColor) { this->parentBgHoverColor = parentBgHoverColor; }
void NavListView::setParentTextNormalColor(const QColor& parentTextNormalColor) { this->parentTextNormalColor = parentTextNormalColor; }
void NavListView::setParentTextSelectedColor(const QColor& parentTextSelectedColor) { this->parentTextSelectedColor = parentTextSelectedColor; }
void NavListView::setParentTextHoverColor(const QColor& parentTextHoverColor) { this->parentTextHoverColor = parentTextHoverColor; }

void NavListView::setChildIconMargin(int childIconMargin) { this->childIconMargin = childIconMargin; }
void NavListView::setChildMargin(int childMargin) { this->childMargin = childMargin; }
void NavListView::setChildFontSize(int childFontSize) { this->childFontSize = childFontSize; }
void NavListView::setChildHeight(int childHeight) { this->childHeight = childHeight; }
void NavListView::setChildBgNormalColor(const QColor& childBgNormalColor) { this->childBgNormalColor = childBgNormalColor; }
void NavListView::setChildBgSelectedColor(const QColor& childBgSelectedColor) { this->childBgSelectedColor = childBgSelectedColor; }
void NavListView::setChildBgHoverColor(const QColor& childBgHoverColor) { this->childBgHoverColor = childBgHoverColor; }
void NavListView::setChildTextNormalColor(const QColor& childTextNormalColor) { this->childTextNormalColor = childTextNormalColor; }
void NavListView::setChildTextSelectedColor(const QColor& childTextSelectedColor) { this->childTextSelectedColor = childTextSelectedColor; }
void NavListView::setChildTextHoverColor(const QColor& childTextHoverColor) { this->childTextHoverColor = childTextHoverColor; }

void NavListView::setExpendMode(const NavListView::ExpendMode& mode)
{
    if (expendMode != mode) 
    {
        expendMode = mode;
        if (mode == ExpendMode_SingleClick) 
        {
            disconnect(this, &NavListView::doubleClicked, model, &NavModel::expand);
            connect(this, &NavListView::clicked, model, &NavModel::expand);
        }
        else if (mode == ExpendMode_DoubleClick) 
        {
            disconnect(this, &NavListView::clicked, model, &NavModel::expand);
            connect(this, &NavListView::doubleClicked, model, &NavModel::expand);
        }
        else if (mode == ExpendMode_NoClick) 
        {
            disconnect(this, &NavListView::clicked, model, &NavModel::expand);
            disconnect(this, &NavListView::doubleClicked, model, &NavModel::expand);
        }
    }
}
