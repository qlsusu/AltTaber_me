#include "utils/KeyboardHooker.h"
#include <QDebug>
#include <QApplication>
#include <QKeyEvent>
#include "utils/Util.h"
#include "widget.h"

// 手动跟踪 Alt 按下状态（GetAsyncKeyState 在钩子回调内不可靠，文档明确警告）
static bool g_altDown = false;
static bool g_shiftDown = false;

LRESULT keyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    using Hooker = KeyboardHooker;
    if (nCode != HC_ACTION) {
        return CallNextHookEx(nullptr, nCode, wParam, lParam);
    }
    auto* pKeyBoard = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

    // 跟踪修饰键状态
    if (wParam == WM_SYSKEYDOWN || wParam == WM_KEYDOWN) {
        if (pKeyBoard->vkCode == VK_LMENU || pKeyBoard->vkCode == VK_RMENU) {
            g_altDown = true;
        }
        if (pKeyBoard->vkCode == VK_SHIFT) {
            g_shiftDown = true;
        }
    } else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
        if (pKeyBoard->vkCode == VK_LMENU || pKeyBoard->vkCode == VK_RMENU) {
            g_altDown = false;
        }
        if (pKeyBoard->vkCode == VK_SHIFT) {
            g_shiftDown = false;
        }
    }

    if (wParam == WM_SYSKEYDOWN || wParam == WM_KEYDOWN) {
        if (g_altDown && Hooker::receiver) {
            if (pKeyBoard->vkCode == VK_TAB) {
                qDebug() << "Alt+Tab detected!";
                if ((HWND) Hooker::receiver->winId() != GetForegroundWindow()) { // not Foreground
                    // 异步，防止阻塞；超过1s会导致被系统强制绕过，传递给下一个钩子
                    QMetaObject::invokeMethod(Hooker::receiver, "requestShow", Qt::QueuedConnection);
                } else {
                    // 转发Alt+Tab给Widget
                    auto shiftModifier = g_shiftDown ? Qt::ShiftModifier : Qt::NoModifier;
                    auto tabDownEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab, Qt::AltModifier | shiftModifier);
                    QApplication::postEvent(Hooker::receiver, tabDownEvent); // async
                }
                return 1; // 阻止事件传递
            } else if (pKeyBoard->vkCode == VK_OEM_3) { // ~`
                qDebug() << "Alt+` detected!";
                auto shiftModifier = g_shiftDown ? Qt::ShiftModifier : Qt::NoModifier;
                auto event = new QKeyEvent(QEvent::KeyPress, Qt::Key_QuoteLeft, Qt::AltModifier | shiftModifier);
                QApplication::postEvent(Hooker::receiver, event); // async
                return 1; // 阻止事件传递
            }
        }
    } else if (wParam == WM_KEYUP) { // Amazing, Alt Down is `WM_SYSKEYDOWN`, but release is `WM_KEYUP`
        if (pKeyBoard->vkCode == VK_LMENU && Hooker::receiver) {
            // BUG: Alt + 方向键 长按，过一秒会触发Alt release，而Alt + 其他键则不会，可能是Windows保护机制或键盘问题？
            qDebug() << "Alt released!";
            auto event = new QKeyEvent(QEvent::KeyRelease, Qt::Key_Alt, Qt::NoModifier);
            QApplication::postEvent(Hooker::receiver, event); // async
            // not block
        }
    }
    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

KeyboardHooker::KeyboardHooker(QWidget* _receiver) {
    if (KeyboardHooker::receiver) {
        qWarning() << "Only one KeyboardHooker can be installed!";
        return;
    }
    // 回调函数的执行与消息循环密切相关，在Get/PeekMessage时，系统才会触发回调; [https://learn.microsoft.com/en-us/windows/win32/winmsg/mouseproc]
    h_keyboard = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC) keyboardProc, GetModuleHandle(nullptr), 0);
    if (!h_keyboard) {
        qWarning() << "Failed to install h_keyboard!";
        return;
    }
    if (!_receiver) {
        qWarning() << "Receiver is nullptr!";
        return;
    }
    KeyboardHooker::receiver = _receiver;
    qInfo() << "KeyboardHooker installed";
}

KeyboardHooker::~KeyboardHooker() {
    if (!h_keyboard) return;
    UnhookWindowsHookEx(h_keyboard);
    KeyboardHooker::receiver = nullptr;
    qDebug() << "KeyboardHooker uninstalled";
}
