# 开发注意事项

## 编译输出截断

编译时如果出错，MSVC 错误信息可能非常长，导致 AI 会话上下文溢出。

**建议只取最后几行：**

```bash
# 方式 1：tail
powershell -Command "cd D:\tool\AltTaber_me; .\build2.bat" 2>&1 | tail -20

# 方式 2：PowerShell Select-Object
powershell -Command "cd D:\tool\AltTaber_me; .\build2.bat" 2>&1 | Select-Object -Last 20
```

## 编译前关闭程序

如果 AltTaber.exe 正在运行，链接会报 `LNK1104: 无法打开文件`。

```bash
taskkill /F /IM AltTaber.exe
```

## 构建命令

```bash
cd /d/tool/AltTaber_me
powershell -Command "cd D:\tool\AltTaber_me; .\build2.bat" 2>&1 | tail -20
```

- 构建脚本：`build2.bat`
- 输出路径：`D:\tool\AltTaber_me\build\Release\AltTaber.exe`
- CMake 生成器：Visual Studio 18 2026
- Qt 版本：6.12.0 (MSVC 2022 64-bit)
