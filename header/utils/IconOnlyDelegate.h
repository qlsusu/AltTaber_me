#ifndef WIN_SWITCHER_ICONONLYDELEGATE_H
#define WIN_SWITCHER_ICONONLYDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QIcon>
#include <QColor>

/// Icon Only Mode for QListWidget
class IconOnlyDelegate : public QStyledItemDelegate {
    QColor selectedColor;
    QColor hoverColor;
    int radius;
    bool drawText; // 是否在图标下方绘制文字（窗口切换模式需要）

public:
    explicit IconOnlyDelegate(QObject* parent = nullptr,
                              QColor selectedColor = QColor(80, 80, 80, 200),
                              QColor hoverColor = QColor(50, 50, 50, 100),
                              int radius = 8,
                              bool drawText = false)
        : QStyledItemDelegate(parent), selectedColor(selectedColor), hoverColor(hoverColor),
          radius(radius), drawText(drawText) {}

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};


#endif //WIN_SWITCHER_ICONONLYDELEGATE_H
