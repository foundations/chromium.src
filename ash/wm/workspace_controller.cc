// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ash/wm/workspace_controller.h"

#include "ash/ash_switches.h"
#include "ash/wm/window_util.h"
#include "ash/wm/workspace/workspace_manager.h"
#include "base/command_line.h"
#include "ui/aura/client/activation_client.h"
#include "ui/aura/client/aura_constants.h"
#include "ui/aura/root_window.h"
#include "ui/aura/window.h"

namespace ash {
namespace internal {

WorkspaceController::WorkspaceController(aura::Window* viewport)
    : viewport_(viewport) {
  aura::RootWindow* root_window = viewport->GetRootWindow();
  workspace_manager_.reset(new WorkspaceManager(viewport));
  aura::client::GetActivationClient(root_window)->AddObserver(this);
}

WorkspaceController::~WorkspaceController() {
  aura::client::GetActivationClient(viewport_->GetRootWindow())->
      RemoveObserver(this);
}

WorkspaceWindowState WorkspaceController::GetWindowState() const {
  return workspace_manager_->GetWindowState();
}

void WorkspaceController::SetShelf(ShelfLayoutManager* shelf) {
  workspace_manager_->SetShelf(shelf);
}

void WorkspaceController::SetActiveWorkspaceByWindow(aura::Window* window) {
  return workspace_manager_->SetActiveWorkspaceByWindow(window);
}

aura::Window* WorkspaceController::GetParentForNewWindow(aura::Window* window) {
  return workspace_manager_->GetParentForNewWindow(window);
}


void WorkspaceController::DoInitialAnimation() {
  workspace_manager_->DoInitialAnimation();
}

void WorkspaceController::OnWindowActivated(aura::Window* window,
                                            aura::Window* old_active) {
  if (!window || window->GetRootWindow() == viewport_->GetRootWindow())
    workspace_manager_->SetActiveWorkspaceByWindow(window);
}

}  // namespace internal
}  // namespace ash
