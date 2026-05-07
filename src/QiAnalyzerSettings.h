#ifndef QI_ANALYZER_SETTINGS
#define QI_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

enum QiInputMode {
    INPUT_MODE_DIGITAL = 0,
    INPUT_MODE_ANALOG  = 1,
};

class QiAnalyzerSettings : public AnalyzerSettings {
  public:
    QiAnalyzerSettings();
    virtual ~QiAnalyzerSettings();

    virtual bool        SetSettingsFromInterfaces();
    virtual void        LoadSettings(const char* settings);
    virtual const char* SaveSettings();

    void UpdateInterfacesFromSettings();

    Channel mInputChannel;
    double  mInputMode;        // QiInputMode as double for NumberList interface
    double  mHysteresisVolts;  // Hysteresis in Volts (default 0.050)

  protected:
    std::unique_ptr<AnalyzerSettingInterfaceChannel>    mInputChannelInterface;
    std::unique_ptr<AnalyzerSettingInterfaceNumberList> mInputModeInterface;
    std::unique_ptr<AnalyzerSettingInterfaceNumberList> mHysteresisInterface;
};

#endif    // QI_ANALYZER_SETTINGS
