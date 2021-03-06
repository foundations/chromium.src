// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_DEMUXER_H_
#define MEDIA_BASE_DEMUXER_H_

#include "base/memory/ref_counted.h"
#include "base/time.h"
#include "media/base/data_source.h"
#include "media/base/demuxer_stream.h"
#include "media/base/media_export.h"
#include "media/base/pipeline_status.h"

namespace media {

class MEDIA_EXPORT DemuxerHost : public DataSourceHost {
 public:
  // Sets the duration of the media in microseconds.
  // Duration may be kInfiniteDuration() if the duration is not known.
  virtual void SetDuration(base::TimeDelta duration) = 0;

  // Stops execution of the pipeline due to a fatal error.  Do not call this
  // method with PIPELINE_OK.
  virtual void OnDemuxerError(PipelineStatus error) = 0;

 protected:
  virtual ~DemuxerHost();
};

class MEDIA_EXPORT Demuxer : public base::RefCountedThreadSafe<Demuxer> {
 public:
  Demuxer();

  // Completes initialization of the demuxer.
  //
  // The demuxer does not own |host| as it is guaranteed to outlive the
  // lifetime of the demuxer. Don't delete it!
  virtual void Initialize(DemuxerHost* host,
                          const PipelineStatusCB& status_cb) = 0;

  // The pipeline playback rate has been changed.  Demuxers may implement this
  // method if they need to respond to this call.
  virtual void SetPlaybackRate(float playback_rate);

  // Carry out any actions required to seek to the given time, executing the
  // callback upon completion.
  virtual void Seek(base::TimeDelta time, const PipelineStatusCB& status_cb);

  // The pipeline is being stopped either as a result of an error or because
  // the client called Stop().
  virtual void Stop(const base::Closure& callback);

  // This method is called from the pipeline when the audio renderer
  // is disabled. Demuxers can ignore the notification if they do not
  // need to react to this event.
  //
  // TODO(acolwell): Change to generic DisableStream(DemuxerStream::Type).
  virtual void OnAudioRendererDisabled();

  // Returns the given stream type, or NULL if that type is not present.
  virtual scoped_refptr<DemuxerStream> GetStream(DemuxerStream::Type type) = 0;

  // Returns the starting time for the media file.
  virtual base::TimeDelta GetStartTime() const = 0;

 protected:
  friend class base::RefCountedThreadSafe<Demuxer>;
  virtual ~Demuxer();

 private:
  DISALLOW_COPY_AND_ASSIGN(Demuxer);
};

}  // namespace media

#endif  // MEDIA_BASE_DEMUXER_H_
