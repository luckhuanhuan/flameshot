#ifndef BASESOURCE_H
#define BASESOURCE_H

#include <vector>

#include "src/core/Global.h"
#include "src/AV/AVWrapper.h"
#include "src/common/MutexDataPair.h"

#define SINK_TIMESTAMP_NONE ((int64_t) 0x8000000000000000ull)
#define SINK_TIMESTAMP_ASAP ((int64_t) 0x8000000000000001ull)

class BaseSource;
class BaseSink;

class BaseSource
{
public:
    struct SinkData {
        BaseSink *sink;
        int priority;
        inline SinkData() {}
        inline SinkData(BaseSink *sink, int priority): sink(sink), priority(priority) {}
        inline bool operator<(const SinkData& other) const {
            return (priority > other.priority);
        }
    };
    struct SharedData {
        std::vector<SinkData> m_sinks;
    };
    typedef MutexDataPair<SharedData>::Lock SharedLock;

public:
    MutexDataPair<SharedData> m_shared_data;
public:
    BaseSource();
    virtual ~BaseSource();
};


class BaseSink
{
public:
    BaseSource *m_source;
    int m_priority;
public:
    BaseSink();
    virtual ~BaseSink();
    void ConnectBaseSource(BaseSource* source, int priority);
};

class VideoSource: private BaseSource
{
    friend class VideoSink;
protected:
    VideoSource() {}
    int64_t CalculateNextVideoTimestamp();
    void PushVideoFrame(unsigned int width, unsigned int height, const uint8_t* data, int stride, AVPixelFormat format, int64_t timestamp);
    void PushVideoPing(int64_t timestamp);
};

class VideoSink: private BaseSink
{
    friend class VideoSource;
protected:
    VideoSink() {}
public:
    inline void ConnectVideoSource(VideoSource* source, int priority = 0) {ConnectBaseSource(source, priority); }
public:
    virtual int64_t GetNextVideoTimestamp() {return SINK_TIMESTAMP_NONE; }
    virtual void ReadVideoFrame(unsigned int width, unsigned int height, const uint8_t* data, int stride, AVPixelFormat format, int64_t timestamp) = 0;
    virtual void ReadVideoPing(int64_t timestamp) {}
};

class AudioSource: private BaseSource
{
    friend class AudioSink;
protected:
    AudioSource() {}
    void PushAudioSamples(unsigned int channels, unsigned int sample_rate, AVSampleFormat format, unsigned int sample_count, const uint8_t* data, int64_t timestamp);
    void PushAudioHole();
};


class AudioSink: private BaseSink
{
    friend class AudioSource;
protected:
    AudioSink() {}
public:
    inline void ConnectAudioSource(AudioSource* source, int priority = 0) {ConnectBaseSource(source, priority); }
public:
    virtual void ReadAudioSamples(unsigned int channels, unsigned int sample_rate, AVSampleFormat format, unsigned int sample_count, const uint8_t* data, int64_t timestamp) = 0;
    virtual void ReadAudioHole() {};
};











































#endif // BASESOURCE_H
