void Widget::showWindowSwitcherOverlay() {
    auto foreWin = GetForegroundWindow();
    auto targetExe = Util::getWindowProcessPath(foreWin);
    
    if (targetExe.isEmpty()) {
        return;
    }
    
    // 获取同组窗口列表
    windowList.clear();
    const auto list = Util::listValidWindows();
    
    for (auto hwnd : list) {
        if (hwnd == this->hWnd()) continue;
        auto path = Util::getWindowProcessPath(hwnd);
        if (path == targetExe) {
            windowList.append({Util::getWindowTitle(hwnd), Util::getClassName(hwnd), hwnd});
        }
    }
    
    if (windowList.size() <= 1) {
        return;
    }
    
    // 直接复用现有的 lw 控件，改变显示内容
    lw->clear();
    for (int i = 0; i < windowList.size(); i++) {
        auto& info = windowList[i];
        auto* item = new QListWidgetItem(lw);
        item->setText(info.title);
        item->setData(Qt::UserRole, QVariant::fromValue(info));
        
        // 获取缩略图
        QPixmap thumbnail = Util::getWindowThumbnail(info.hwnd, QSize(180, 130));
        if (!thumbnail.isNull()) {
            item->setIcon(QIcon(thumbnail));
        } else {
            item->setIcon(Util::getCachedIcon(targetExe, info.hwnd));
        }
    }
    
    // 选择当前前台窗口
    windowSelectedIndex = 0;
    for (int i = 0; i < windowList.size(); i++) {
        if (windowList[i].hwnd == foreWin) {
            windowSelectedIndex = i;
            break;
        }
    }
    lw->setCurrentRow(windowSelectedIndex);
    
    // 更新标题标签
    if (!windowTitleLabel) {
        windowTitleLabel = new QLabel(this);
        windowTitleLabel->setStyleSheet("QLabel { background-color: transparent; color: white; font-size: 12px; padding: 4px 8px; }");
    }
    updateWindowTitleLabel();
    windowTitleLabel->show();
    windowTitleLabel->raise();
    
    windowModeActive = true;
}
