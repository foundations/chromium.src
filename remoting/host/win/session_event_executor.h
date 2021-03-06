// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_WIN_SESSION_EVENT_EXECUTOR_H_
#define REMOTING_HOST_WIN_SESSION_EVENT_EXECUTOR_H_

#include <set>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "remoting/host/event_executor.h"
#include "remoting/host/win/scoped_thread_desktop.h"

namespace base {
class SingleThreadTaskRunner;
} // namespace base

namespace remoting {

class SasInjector;

// Monitors and passes key/mouse events to a nested event executor. Injects
// Secure Attention Sequence (SAS) when Ctrl+Alt+Del key combination has been
// detected.
class SessionEventExecutorWin : public EventExecutor {
 public:
  // |inject_sas| is invoked on |inject_sas_task_runner| to generate SAS on
  // Vista+.
  SessionEventExecutorWin(
      scoped_refptr<base::SingleThreadTaskRunner> input_task_runner,
      scoped_ptr<EventExecutor> nested_executor,
      scoped_refptr<base::SingleThreadTaskRunner> inject_sas_task_runner,
      const base::Closure& inject_sas);
  ~SessionEventExecutorWin();

  // EventExecutor implementation.
  virtual void Start(
      scoped_ptr<protocol::ClipboardStub> client_clipboard) OVERRIDE;
  virtual void StopAndDelete() OVERRIDE;

  // protocol::ClipboardStub implementation.
  virtual void InjectClipboardEvent(
      const protocol::ClipboardEvent& event) OVERRIDE;

  // protocol::InputStub implementation.
  virtual void InjectKeyEvent(const protocol::KeyEvent& event) OVERRIDE;
  virtual void InjectMouseEvent(const protocol::MouseEvent& event) OVERRIDE;

 private:
  // Switches to the desktop receiving a user input if different from
  // the current one.
  void SwitchToInputDesktop();

  scoped_refptr<base::SingleThreadTaskRunner> input_task_runner_;

  // Pointer to the next event executor.
  scoped_ptr<EventExecutor> nested_executor_;

  scoped_refptr<base::SingleThreadTaskRunner> inject_sas_task_runner_;

  ScopedThreadDesktop desktop_;

  // Used to inject Secure Attention Sequence on Vista+.
  base::Closure inject_sas_;

  // Used to inject Secure Attention Sequence on XP.
  scoped_ptr<SasInjector> sas_injector_;

  // Keys currently pressed by the client, used to detect Ctrl-Alt-Del.
  std::set<uint32> pressed_keys_;

  base::WeakPtrFactory<SessionEventExecutorWin> weak_ptr_factory_;
  base::WeakPtr<SessionEventExecutorWin> weak_ptr_;

  DISALLOW_COPY_AND_ASSIGN(SessionEventExecutorWin);
};

}  // namespace remoting

#endif  // REMOTING_HOST_WIN_SESSION_EVENT_EXECUTOR_H_
