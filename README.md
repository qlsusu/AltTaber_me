# AltTaber

Windows 窗口切换工具，支持 Alt+Tab 切换应用、Alt+` 切换同应用窗口，均带缩略图预览。

## 演示

[查看演示视频](https://github.com/qlsusu/AltTaber_me/raw/main/demo.mp4)

## 功能

- **Alt + Tab**：在应用之间切换，显示缩略图
- **Alt + `**：在同一应用的不同窗口之间切换，显示缩略图

## 构建

需要 Qt 5.15+ 和 CMake：

```bash
cmake -B build -G "Visual Studio 17 2022"
cmake --build build --config Release
```

## 许可证

MIT
