// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/aura/desktop/desktop_screen_win.h"

#include "base/logging.h"
#include "ui/aura/desktop/desktop_screen.h"
#include "ui/aura/root_window.h"
#include "ui/aura/root_window_host.h"
#include "ui/gfx/display.h"

namespace {

MONITORINFO GetMonitorInfoForMonitor(HMONITOR monitor) {
  MONITORINFO monitor_info = { 0 };
  monitor_info.cbSize = sizeof(monitor_info);
  GetMonitorInfo(monitor, &monitor_info);
  return monitor_info;
}

gfx::Display GetDisplay(MONITORINFO& monitor_info) {
  // TODO(oshima): Implement ID and Observer.
  gfx::Display display(0, gfx::Rect(monitor_info.rcMonitor));
  display.set_work_area(gfx::Rect(monitor_info.rcWork));
  return display;
}

}  // namespace

namespace aura {

////////////////////////////////////////////////////////////////////////////////
// DesktopScreenWin, public:

DesktopScreenWin::DesktopScreenWin() {
}

DesktopScreenWin::~DesktopScreenWin() {
}

////////////////////////////////////////////////////////////////////////////////
// DesktopScreenWin, gfx::ScreenWin implementation:

bool DesktopScreenWin::IsDIPEnabled() {
  return true;
}

gfx::Display DesktopScreenWin::GetDisplayMatching(
    const gfx::Rect& match_rect) const {
  return GetDisplayNearestPoint(match_rect.CenterPoint());
}

HWND DesktopScreenWin::GetHWNDFromNativeView(gfx::NativeView window) const {
  return window->GetRootWindow()->GetAcceleratedWidget();
}

gfx::NativeWindow DesktopScreenWin::GetNativeWindowFromHWND(HWND hwnd) const {
  return (::IsWindow(hwnd)) ?
      RootWindow::GetForAcceleratedWidget(hwnd) : NULL;
}

////////////////////////////////////////////////////////////////////////////////

gfx::Screen* CreateDesktopScreen() {
  return new DesktopScreenWin;
}

}  // namespace aura
