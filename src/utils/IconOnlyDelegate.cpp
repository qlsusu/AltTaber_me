#include "utils/IconOnlyDelegate.h"
#include "widget.h"
#include <QMetaType>
#include <QFile>
#include <QDateTime>
#include <QPainterPath>

void IconOnlyDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::NoPen); //取消边框
    // option.rect.size() == QListWidgetItem::sizeHint()
    // 背景加 3px inset，避免 hover 背景紧贴图标/文字边缘
    QRect bgRect = option.rect;
    // 调试日志
    if (option.state & (QStyle::State_Selected | QStyle::State_MouseOver)) {
        QFile f("D:/tool/AltTaber_me/debug.log");
        if (f.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream ts(&f);
            ts << "[delegate] rect=" << bgRect.x() << "," << bgRect.y() << "," << bgRect.width() << "," << bgRect.height()
               << " deco=" << option.decorationSize.width() << "x" << option.decorationSize.height()
               << " drawText=" << drawText << "\n";
        }
    }
    if (option.state & QStyle::State_Selected || option.state & QStyle::State_MouseOver) {
        // 使用 QPainterPath 绘制圆角矩形（填充 + 边框）
        QRectF roundedRect = QRectF(bgRect).adjusted(1, 1, -1, -1);
        QPainterPath path;
        path.addRoundedRect(roundedRect, 8, 8);
        
        // 填充背景色
        QColor bgColor = (option.state & QStyle::State_Selected) ? selectedColor : hoverColor;
        painter->fillPath(path, bgColor);
        
        // 绘制边框线：1px #befe00
        QPen borderPen(QColor(0xbe, 0xfe, 0x00));
        borderPen.setWidth(1);
        painter->setPen(borderPen);
        painter->drawPath(path);
    }

    // 绘制图标（兼容 QIcon / QPixmap 两种存法）
    auto icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
    if (icon.isNull()) {
        auto pix = qvariant_cast<QPixmap>(index.data(Qt::DecorationRole));
        if (!pix.isNull()) icon = QIcon(pix);
    }
    QRect iconRect{{}, option.decorationSize}; // QListWidget::iconSize()
    if (drawText) {
        // 有文字时：图标居上，文字在下方
        iconRect.moveCenter(QRect(option.rect.left(), option.rect.top(), option.rect.width(), option.rect.height() - 20).center());
    } else {
        iconRect.moveCenter(option.rect.center());
    }
    if (!icon.isNull()) {
        icon.paint(painter, iconRect);
    }



    // 绘制文字（窗口切换模式：图标下方显示窗口标题）
    if (drawText) {
        auto text = index.data(Qt::DisplayRole).toString();
        if (!text.isEmpty()) {
            QFont font{"Microsoft YaHei"};
            font.setPointSizeF(9);
            painter->setFont(font);
            painter->setPen(QColor(220, 220, 220));
            QRect textRect(option.rect.left(), option.rect.bottom() - 18, option.rect.width(), 18);
            painter->drawText(textRect, Qt::AlignHCenter | Qt::AlignTop,
                              QFontMetrics(font).elidedText(text, Qt::ElideRight, option.rect.width() - 8));
        }
    }

    // draw badge（仅 app 列表有 WindowGroup 数据）
    auto num = 0;
    if (index.data(Qt::UserRole).canConvert<WindowGroup>()) {
        num = qvariant_cast<WindowGroup>(index.data(Qt::UserRole)).windows.size();
    }
    if (num > 1) {
        auto text = QString::number(num);
        const auto extraWidth = 8 * (text.size() - 1);
        constexpr auto R = 12;
        auto badgeCenter = option.rect.topRight() + QPoint(-(R + 3), R + 3);
        // extra Width for extra number
        auto badgeRect = QRect(badgeCenter + QPoint(-R - extraWidth, -R), QSize(2 * R + extraWidth, 2 * R));
        painter->setPen(QColor(200, 200, 200, 50));
        painter->setBrush(QColor(133, 114, 97)); // learn from iOS
        painter->drawRoundedRect(badgeRect, R, R);

        QFont font{"Microsoft YaHei"};
        font.setPointSizeF(12.8);
        font.setBold(true);
        painter->setFont(font);
        painter->setPen(QColor(214, 192, 171));
        painter->drawText(badgeRect, Qt::AlignCenter, text);
    }
}
