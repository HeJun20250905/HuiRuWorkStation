#pragma once
#pragma execution_character_set("utf-8")

#include <QObject>
#include <QStyledItemDelegate>
#include <QAbstractListModel>
#include <QListView>
#include <QColor>
#include <QList>
#include <QVariant>

class NavListView;

// NavDelegate
class NavDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit NavDelegate(QObject* parent = nullptr);
    ~NavDelegate() override;

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    NavListView* nav;
    QFont iconFont;
};

// NavModel
class NavModel : public QAbstractListModel
{
    Q_OBJECT

public:
    struct TreeNode 
    {
        int level = 0;
        bool expand = false;
        bool last = false;
        QString text;
        QString tip;
        QString parentText;
        QChar icon;
        QList<TreeNode*> children;
    };

    struct ListNode 
    {
        QString text;
        TreeNode* treeNode = nullptr;
    };

    explicit NavModel(QObject* parent = nullptr);
    ~NavModel() override;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void setItems(const QStringList& items);
    void expand(const QModelIndex& index);

private:
    void refreshList();

    QList<TreeNode*> treeNode;
    QList<ListNode> listNode;
};

// NavListView
class NavListView : public QListView
{
    Q_OBJECT
        Q_ENUMS(ExpendMode)

public:
    enum ExpendMode 
    {
        ExpendMode_SingleClick,
        ExpendMode_DoubleClick,
        ExpendMode_NoClick
    };

    explicit NavListView(QWidget* parent = nullptr);
    ~NavListView();

    // 获取配置
    bool getRightIconVisible() const;
    bool getTipVisible() const;
    int getTipWidth() const;
    bool getSeparateVisible() const;
    int getSeparateHeight() const;
    QColor getSeparateColor() const;
    bool getLineLeft() const;
    bool getLineVisible() const;
    int getLineWidth() const;
    QColor getLineColor() const;
    bool getTriangleLeft() const;
    bool getTriangleVisible() const;
    int getTriangleWidth() const;
    QColor getTriangleColor() const;
    int getParentIconMargin() const;
    int getParentMargin() const;
    int getParentFontSize() const;
    int getParentHeight() const;
    QColor getParentBgNormalColor() const;
    QColor getParentBgSelectedColor() const;
    QColor getParentBgHoverColor() const;
    QColor getParentTextNormalColor() const;
    QColor getParentTextSelectedColor() const;
    QColor getParentTextHoverColor() const;
    int getChildIconMargin() const;
    int getChildMargin() const;
    int getChildFontSize() const;
    int getChildHeight() const;
    QColor getChildBgNormalColor() const;
    QColor getChildBgSelectedColor() const;
    QColor getChildBgHoverColor() const;
    QColor getChildTextNormalColor() const;
    QColor getChildTextSelectedColor() const;
    QColor getChildTextHoverColor() const;
    ExpendMode getExpendMode() const;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

public slots:
    void setItems(const QString& items);
    void setCurrentRow(int row);
    void setRightIconVisible(bool rightIconVisible);
    void setTipVisible(bool tipVisible);
    void setTipWidth(int tipWidth);
    void setSeparateVisible(bool separateVisible);
    void setSeparateHeight(int separateHeight);
    void setSeparateColor(const QColor& separateColor);
    void setLineLeft(bool lineLeft);
    void setLineVisible(bool lineVisible);
    void setLineWidth(int lineWidth);
    void setLineColor(const QColor& lineColor);
    void setTriangleLeft(bool triangleLeft);
    void setTriangleVisible(bool triangleVisible);
    void setTriangleWidth(int triangleWidth);
    void setTriangleColor(const QColor& triangleColor);
    void setParentIconMargin(int parentIconMargin);
    void setParentMargin(int parentMargin);
    void setParentFontSize(int parentFontSize);
    void setParentHeight(int parentHeight);
    void setParentBgNormalColor(const QColor& parentBgNormalColor);
    void setParentBgSelectedColor(const QColor& parentBgSelectedColor);
    void setParentBgHoverColor(const QColor& parentBgHoverColor);
    void setParentTextNormalColor(const QColor& parentTextNormalColor);
    void setParentTextSelectedColor(const QColor& parentTextSelectedColor);
    void setParentTextHoverColor(const QColor& parentTextHoverColor);
    void setChildIconMargin(int childIconMargin);
    void setChildMargin(int childMargin);
    void setChildFontSize(int childFontSize);
    void setChildHeight(int childHeight);
    void setChildBgNormalColor(const QColor& childBgNormalColor);
    void setChildBgSelectedColor(const QColor& childBgSelectedColor);
    void setChildBgHoverColor(const QColor& childBgHoverColor);
    void setChildTextNormalColor(const QColor& childTextNormalColor);
    void setChildTextSelectedColor(const QColor& childTextSelectedColor);
    void setChildTextHoverColor(const QColor& childTextHoverColor);
    void setExpendMode(const ExpendMode& expendMode);

signals:
    void pressed(const QString& text, const QString& parentText);
    void pressed(int index, int parentIndex);
    void pressed(int childIndex);

private slots:
    void onItemPressed(const QModelIndex& data);

private:
    void setData(const QStringList& listItems);

    NavModel* model;
    NavDelegate* delegate;

    QString items;
    QStringList parentItem;
    QList<QStringList> childItem;

    // 外观配置
    bool rightIconVisible;
    bool tipVisible;
    int tipWidth;
    bool separateVisible;
    int separateHeight;
    QColor separateColor;
    bool lineLeft;
    bool lineVisible;
    int lineWidth;
    QColor lineColor;
    bool triangleLeft;
    bool triangleVisible;
    int triangleWidth;
    QColor triangleColor;

    int parentIconMargin;
    int parentMargin;
    int parentFontSize;
    int parentHeight;
    QColor parentBgNormalColor;
    QColor parentBgSelectedColor;
    QColor parentBgHoverColor;
    QColor parentTextNormalColor;
    QColor parentTextSelectedColor;
    QColor parentTextHoverColor;

    int childIconMargin;
    int childMargin;
    int childFontSize;
    int childHeight;
    QColor childBgNormalColor;
    QColor childBgSelectedColor;
    QColor childBgHoverColor;
    QColor childTextNormalColor;
    QColor childTextSelectedColor;
    QColor childTextHoverColor;

    ExpendMode expendMode;
};
