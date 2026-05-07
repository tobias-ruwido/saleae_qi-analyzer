#ifndef QI_ANALYZER_H
#define QI_ANALYZER_H

#include <deque>
#include <vector>

#include <Analyzer.h>
#include "QiAnalyzerResults.h"
#include "QiSimulationDataGenerator.h"

class QiAnalyzerSettings;

class ANALYZER_EXPORT QiAnalyzer : public Analyzer2 {
  public:
    QiAnalyzer();
    virtual ~QiAnalyzer();

    virtual void SetupResults();
    virtual void WorkerThread();

    virtual U32 GenerateSimulationData(U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels);
    virtual U32 GetMinimumSampleRateHz();

    virtual const char* GetAnalyzerName() const;
    virtual bool        NeedsRerun();

  private:
    struct BitInfo {
        U64 start;
        U64 end;
        U32 value;
    };

    // Edge info for analog-to-digital conversion
    struct DigitalEdge {
        U64     sample;
        BitState state;
    };

    std::unique_ptr<QiAnalyzerSettings> mSettings;
    std::unique_ptr<QiAnalyzerResults>  mResults;
    AnalyzerChannelData*                mQi;

    QiSimulationDataGenerator mSimulationDataGenerator;
    bool                      mSimulationInitilized;

    U32 mSampleRateHz;

    U32                       mTLong;
    U32                       mTShort;
    U32                       mTLongMinError;
    U32                       mTLongMaxError;
    U32                       mTShortMinError;
    U32                       mTShortMaxError;
    std::deque<BitInfo>       mBitsForNextByte;    // value, location
    std::vector<U64>          mPreambleEdges;
    U32                       mPacketByteCount;
    bool                      mSynchronized;

    // Analog-to-digital conversion state
    bool                      mIsAnalogMode;
    std::deque<DigitalEdge>   mDigitalEdges;       // Generated digital edges from analog data
    U32                       mDigitalEdgeIndex;
    BitState                  mCurrentDigitalState;
    U64                       mCurrentAnalogSample;

  private:
    void Invalidate();
    U64 AdvanceToNextEdge(U64 edge_location, U64* p_next_edge_location, U64* p_next_edge_distance);
    void ProcessQiData();
    void SynchronizeQiData();
    void SaveBit(U64 location_start, U64 location_end, U32 value);

    // Analog processing methods
    void GenerateDigitalFromAnalog();
    void AnalogAdvanceToNextEdge();
    U64  AnalogGetSampleNumber();
    BitState AnalogGetBitState();
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer();
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer(Analyzer* analyzer);

#endif    // QI_ANALYZER_H
