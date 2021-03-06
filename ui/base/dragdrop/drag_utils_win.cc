// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/dragdrop/drag_utils.h"

#include <objidl.h>
#include <shlobj.h>
#include <shobjidl.h>

#include "base/win/scoped_comptr.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/base/dragdrop/os_exchange_data.h"
#include "ui/base/dragdrop/os_exchange_data_provider_win.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/gdi_util.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/skbitmap_operations.h"

namespace drag_utils {

static void SetDragImageOnDataObject(HBITMAP hbitmap,
                                     const gfx::Size& size,
                                     const gfx::Vector2d& cursor_offset,
                                     IDataObject* data_object) {
  base::win::ScopedComPtr<IDragSourceHelper> helper;
  HRESULT rv = CoCreateInstance(CLSID_DragDropHelper, 0, CLSCTX_INPROC_SERVER,
                                IID_IDragSourceHelper, helper.ReceiveVoid());
  if (SUCCEEDED(rv)) {
    SHDRAGIMAGE sdi;
    sdi.sizeDragImage = size.ToSIZE();
    sdi.crColorKey = 0xFFFFFFFF;
    sdi.hbmpDragImage = hbitmap;
    sdi.ptOffset = gfx::PointAtOffsetFromOrigin(cursor_offset).ToPOINT();
    helper->InitializeFromBitmap(&sdi, data_object);
  }
}

// Blit the contents of the canvas to a new HBITMAP. It is the caller's
// responsibility to release the |bits| buffer.
static HBITMAP CreateHBITMAPFromSkBitmap(const SkBitmap& sk_bitmap) {
  HDC screen_dc = GetDC(NULL);
  BITMAPINFOHEADER header;
  gfx::CreateBitmapHeader(sk_bitmap.width(), sk_bitmap.height(), &header);
  void* bits;
  HBITMAP bitmap =
      CreateDIBSection(screen_dc, reinterpret_cast<BITMAPINFO*>(&header),
                       DIB_RGB_COLORS, &bits, NULL, 0);
  DCHECK(sk_bitmap.rowBytes() == sk_bitmap.width() * 4);
  SkAutoLockPixels lock(sk_bitmap);
  memcpy(
      bits, sk_bitmap.getPixels(), sk_bitmap.height() * sk_bitmap.rowBytes());
  ReleaseDC(NULL, screen_dc);
  return bitmap;
}

void SetDragImageOnDataObject(const gfx::ImageSkia& image_skia,
                              const gfx::Size& size,
                              const gfx::Vector2d& cursor_offset,
                              ui::OSExchangeData* data_object) {
  DCHECK(data_object && !size.IsEmpty());
  // InitializeFromBitmap() doesn't expect an alpha channel and is confused
  // by premultiplied colors, so unpremultiply the bitmap.
  // SetDragImageOnDataObject(HBITMAP) takes ownership of the bitmap.
  HBITMAP bitmap = CreateHBITMAPFromSkBitmap(
      SkBitmapOperations::UnPreMultiply(*image_skia.bitmap()));

  // Attach 'bitmap' to the data_object.
  SetDragImageOnDataObject(bitmap, size, cursor_offset,
      ui::OSExchangeDataProviderWin::GetIDataObject(*data_object));
}

}  // namespace drag_utils
