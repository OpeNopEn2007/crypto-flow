# Qt 6 - Multimedia Module

> **来源**: https://doc.qt.io/qt-6/qtmultimedia-module.html
> **获取日期**: 2026-05-12

# Qt Multimedia Module C++ Classes

The [Qt Multimedia](qtmultimedia-index.html) module provides audio, video and camera functionality. [More\...](#details)

## Namespaces(#namespaces "Direct link to this headline")

  [QtAudio](qtaudio.html)   Contains enums used by the audio classes
  [QtVideo](qtvideo.html)   Enumerations for camera and video functionality

## Classes(#classes "Direct link to this headline")

  [QAbstractVideoBuffer](qabstractvideobuffer.html)                    Abstraction for video data
  [QAbstractVideoBuffer::MapData](qabstractvideobuffer-mapdata.html)   QAbstractVideoBuffer::MapData structure describes the mapped plane layout
  [QAudioBuffer](qaudiobuffer.html)                                    Represents a collection of audio samples with a specific format and sample rate
  [QAudioBufferInput](qaudiobufferinput.html)                          Used for providing custom audio buffers to QMediaRecorder through QMediaCaptureSession
  [QAudioBufferOutput](qaudiobufferoutput.html)                        Used for capturing audio data provided by QMediaPlayer
  [QAudioDecoder](qaudiodecoder.html)                                  Implements decoding audio
  [QAudioDevice](qaudiodevice.html)                                    Information about audio devices and their functionality
  [QAudioFormat](qaudioformat.html)                                    Stores audio stream parameter information
  [QAudioInput](qaudioinput.html)                                      Represents an input channel for audio
  [QAudioOutput](qaudiooutput.html)                                    Represents an output channel for audio
  [QAudioSink](qaudiosink.html)                                        Interface for sending audio data to an audio output device
  [QAudioSource](qaudiosource.html)                                    Interface for receiving audio data from an audio input device
  [QCamera](qcamera.html)                                              Interface for system camera devices
  [QCameraDevice](qcameradevice.html)                                  General information about camera devices
  [QCameraFormat](qcameraformat.html)                                  Describes a video format supported by a camera device
  [QCapturableWindow](qcapturablewindow.html)                          Used for getting the basic information of a capturable window
  [QImageCapture](qimagecapture.html)                                  Used for the recording of media content
  [QMediaCaptureSession](qmediacapturesession.html)                    Allows capturing of audio and video content
  [QMediaDevices](qmediadevices.html)                                  Information about available multimedia input and output devices
  [QMediaFormat](qmediaformat.html)                                    Describes an encoding format for a multimedia file or stream
  [QMediaMetaData](qmediametadata.html)                                Provides meta-data for media files
  [QMediaPlayer](qmediaplayer.html)                                    Allows the playing of a media files
  [QMediaRecorder](qmediarecorder.html)                                Used for encoding and recording a capture session
  [QMediaTimeRange](qmediatimerange.html)                              Represents a set of zero or more disjoint time intervals
  [QMediaTimeRange::Interval](qmediatimerange-interval.html)           QMediaTimeRange::Interval class represents a time interval with integer precision
  [QPlaybackOptions](qplaybackoptions.html)                            Enables low-level control of media playback options
  [QScreenCapture](qscreencapture.html)                                This class is used for capturing a screen
  [QSoundEffect](qsoundeffect.html)                                    Way to play low latency sound effects
  [QVideoFrame](qvideoframe.html)                                      Represents a frame of video data
  [QVideoFrameFormat](qvideoframeformat.html)                          Specifies the stream format of a video presentation surface
  [QVideoFrameInput](qvideoframeinput.html)                            Used for providing custom video frames to QMediaRecorder or a video output through QMediaCaptureSession
  [QVideoSink](qvideosink.html)                                        Represents a generic sink for video data
  [QWindowCapture](qwindowcapture.html)                                This class is used for capturing a window

## Detailed Description(#details "Direct link to this headline")

### Using the Module(#using-the-module "Direct link to this headline")

Using a Qt module requires linking against the module library, either directly or through other dependencies. Several build tools have dedicated support for this, including [CMake](https://cmake.org/cmake/help/latest/) and [qmake](qmake-manual.html).

#### Building with CMake(#building-with-cmake "Direct link to this headline")

Use the `find_package()` command to locate the needed module components in the `Qt6` package:

``` {.cpp .prettyprint translate="no"}
find_package(Qt6 REQUIRED COMPONENTS Multimedia)
target_link_libraries(mytarget PRIVATE Qt6::Multimedia)
```
