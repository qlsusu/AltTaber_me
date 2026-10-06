# AltTaber

Windows 窗口切换工具，支持 Alt+Tab 切换应用、Alt+` 切换同应用窗口，均带缩略图预览。

## 功能

- **Alt + Tab**：在应用之间切换，显示缩略图
- **Alt + `**：在同一应用的不同窗口之间切换，显示缩略图
- 毛玻璃背景、圆角、高 DPI 支持
- 鼠标滚轮切换窗口
- 任务栏图标滚轮切换窗口（Beta）

## 构建

需要 Qt 5.15+ 和 CMake：

```bash
cmake -B build -G "Visual Studio 17 2022"
cmake --build build --config Release
```

## 许可证

MIT
