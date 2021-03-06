// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_WEB_NOTIFICATION_WEB_NOTIFICATION_TRAY_H_
#define ASH_SYSTEM_WEB_NOTIFICATION_WEB_NOTIFICATION_TRAY_H_

#include "ash/ash_export.h"
#include "ash/system/tray/tray_background_view.h"
#include "ash/system/tray/tray_views.h"
#include "ash/system/user/login_status.h"
#include "base/gtest_prod_util.h"
#include "ui/message_center/message_center.h"

// Status area tray for showing browser and app notifications. This hosts
// a MessageCenter class which manages the notification list. This class
// contains the Ash specific tray implementation.
//
// Note: These are not related to system notifications (i.e NotificationView
// generated by SystemTrayItem). Visibility of one notification type or other
// is controlled by StatusAreaWidget.

namespace aura {
class LocatedEvent;
}

namespace gfx {
class ImageSkia;
}

namespace views {
class ImageButton;
}

namespace message_center {
class MessageCenterBubble;
class MessagePopupBubble;
}

namespace ash {

namespace internal {
class StatusAreaWidget;
class WebNotificationBubbleWrapper;
}

class ASH_EXPORT WebNotificationTray
    : public internal::TrayBackgroundView,
      public views::TrayBubbleView::Delegate,
      public message_center::MessageCenter::Host,
      public views::ButtonListener {
 public:
  explicit WebNotificationTray(internal::StatusAreaWidget* status_area_widget);
  virtual ~WebNotificationTray();

  // Set whether or not the popup notifications should be hidden.
  void SetHidePopupBubble(bool hide);

  // Updates tray visibility login status of the system changes.
  void UpdateAfterLoginStatusChange(user::LoginStatus login_status);

  // Returns true if the message center bubble is visible.
  bool IsMessageCenterBubbleVisible() const;

  // Returns true if the mouse is inside the notification bubble.
  bool IsMouseInNotificationBubble() const;

  message_center::MessageCenter* message_center() {
    return message_center_.get();
  }

  // Overridden from TrayBackgroundView.
  virtual void SetShelfAlignment(ShelfAlignment alignment) OVERRIDE;
  virtual void AnchorUpdated() OVERRIDE;
  virtual string16 GetAccessibleNameForTray() OVERRIDE;
  virtual void HideBubbleWithView(
      const views::TrayBubbleView* bubble_view) OVERRIDE;
  virtual bool ClickedOutsideBubble() OVERRIDE;

  // Overridden from internal::ActionableView.
  virtual bool PerformAction(const ui::Event& event) OVERRIDE;

  // Overridden from views::TrayBubbleView::Delegate.
  virtual void BubbleViewDestroyed() OVERRIDE;
  virtual void OnMouseEnteredView() OVERRIDE;
  virtual void OnMouseExitedView() OVERRIDE;
  virtual string16 GetAccessibleNameForBubble() OVERRIDE;
  virtual gfx::Rect GetAnchorRect(views::Widget* anchor_widget,
                                  AnchorType anchor_type,
                                  AnchorAlignment anchor_alignment) OVERRIDE;
  virtual void HideBubble(const views::TrayBubbleView* bubble_view) OVERRIDE;

  // Overridden from message_center::MessageCenter::Host.
  virtual void MessageCenterChanged(bool new_notification) OVERRIDE;

  // Overridden from ButtonListener.
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

 private:
  FRIEND_TEST_ALL_PREFIXES(WebNotificationTrayTest, WebNotifications);
  FRIEND_TEST_ALL_PREFIXES(WebNotificationTrayTest, WebNotificationPopupBubble);
  FRIEND_TEST_ALL_PREFIXES(WebNotificationTrayTest,
                           ManyMessageCenterNotifications);
  FRIEND_TEST_ALL_PREFIXES(WebNotificationTrayTest, ManyPopupNotifications);

  // Shows or hides the message center bubble.
  void ToggleMessageCenterBubble();

  // Shows or updates the message center bubble and hides the popup bubble.
  void ShowMessageCenterBubble();

  // Hides the message center bubble if visible.
  void HideMessageCenterBubble();

  // Shows or updates the popup notification bubble if appropriate.
  void ShowPopupBubble();

  // Hides the notification bubble if visible.
  void HidePopupBubble();

  // Updates the tray icon and visibility.
  void UpdateTray();

  internal::WebNotificationBubbleWrapper* message_center_bubble() const {
    return message_center_bubble_.get();
  }

  internal::WebNotificationBubbleWrapper* popup_bubble() const {
    return popup_bubble_.get();
  }

  // Testing accessors.
  message_center::MessageCenterBubble* GetMessageCenterBubbleForTest();
  message_center::MessagePopupBubble* GetPopupBubbleForTest();

  scoped_ptr<message_center::MessageCenter> message_center_;
  scoped_ptr<internal::WebNotificationBubbleWrapper> message_center_bubble_;
  scoped_ptr<internal::WebNotificationBubbleWrapper> popup_bubble_;
  views::ImageButton* button_;
  bool show_message_center_on_unlock_;

  DISALLOW_COPY_AND_ASSIGN(WebNotificationTray);
};

}  // namespace ash

#endif  // ASH_SYSTEM_WEB_NOTIFICATION_WEB_NOTIFICATION_TRAY_H_
