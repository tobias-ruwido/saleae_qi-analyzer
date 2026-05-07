#include "QiAnalyzerSettings.h"
#include <AnalyzerHelpers.h>

QiAnalyzerSettings::QiAnalyzerSettings()
    : mInputChannel(UNDEFINED_CHANNEL)
    , mInputMode(double(INPUT_MODE_DIGITAL))
    , mHysteresisVolts(0.050) {
    mInputChannelInterface.reset(new AnalyzerSettingInterfaceChannel());
    mInputChannelInterface->SetTitleAndTooltip("Qi", "WPC Qi");
    mInputChannelInterface->SetChannel(mInputChannel);

    mInputModeInterface.reset(new AnalyzerSettingInterfaceNumberList());
    mInputModeInterface->SetTitleAndTooltip("Input Mode", "Select Digital or Analog input channel mode");
    mInputModeInterface->AddNumber(double(INPUT_MODE_DIGITAL), "Digital", "Use a digital input channel");
    mInputModeInterface->AddNumber(double(INPUT_MODE_ANALOG), "Analog", "Use an analog input channel with threshold detection");
    mInputModeInterface->SetNumber(mInputMode);

    mHysteresisInterface.reset(new AnalyzerSettingInterfaceNumberList());
    mHysteresisInterface->SetTitleAndTooltip("Hysteresis (mV)", "Hysteresis around threshold for analog-to-digital conversion");
    mHysteresisInterface->AddNumber(10.0, "10 mV", "10 mV hysteresis");
    mHysteresisInterface->AddNumber(20.0, "20 mV", "20 mV hysteresis");
    mHysteresisInterface->AddNumber(30.0, "30 mV", "30 mV hysteresis");
    mHysteresisInterface->AddNumber(40.0, "40 mV", "40 mV hysteresis");
    mHysteresisInterface->AddNumber(50.0, "50 mV", "50 mV hysteresis (default)");
    mHysteresisInterface->AddNumber(75.0, "75 mV", "75 mV hysteresis");
    mHysteresisInterface->AddNumber(100.0, "100 mV", "100 mV hysteresis");
    mHysteresisInterface->AddNumber(150.0, "150 mV", "150 mV hysteresis");
    mHysteresisInterface->AddNumber(200.0, "200 mV", "200 mV hysteresis");
    mHysteresisInterface->SetNumber(50.0);

    AddInterface(mInputChannelInterface.get());
    AddInterface(mInputModeInterface.get());
    AddInterface(mHysteresisInterface.get());

    AddExportOption(0, "Export as text/csv file");
    AddExportExtension(0, "text", "txt");
    AddExportExtension(0, "csv", "csv");

    ClearChannels();
    AddChannel(mInputChannel, "Qi", false);
}

QiAnalyzerSettings::~QiAnalyzerSettings() {}

bool QiAnalyzerSettings::SetSettingsFromInterfaces() {
    mInputChannel    = mInputChannelInterface->GetChannel();
    mInputMode       = mInputModeInterface->GetNumber();
    mHysteresisVolts = mHysteresisInterface->GetNumber() / 1000.0;    // Convert mV to V

    ClearChannels();
    AddChannel(mInputChannel, "Qi", true);

    return true;
}

void QiAnalyzerSettings::LoadSettings(const char* settings) {
    SimpleArchive text_archive;
    text_archive.SetString(settings);

    const char* name_string;
    text_archive >> &name_string;
    if (strcmp(name_string, "QiAnalyzer") != 0)
        AnalyzerHelpers::Assert("QiAnalyzer: LoadSettings() called with a settings string from a different analyzer.");

    text_archive >> mInputChannel;
    text_archive >> mInputMode;
    text_archive >> mHysteresisVolts;

    ClearChannels();
    AddChannel(mInputChannel, "Qi", true);

    UpdateInterfacesFromSettings();
}

const char* QiAnalyzerSettings::SaveSettings() {
    SimpleArchive text_archive;

    text_archive << "QiAnalyzer";
    text_archive << mInputChannel;
    text_archive << mInputMode;
    text_archive << mHysteresisVolts;

    return SetReturnString(text_archive.GetString());
}

void QiAnalyzerSettings::UpdateInterfacesFromSettings() {
    mInputChannelInterface->SetChannel(mInputChannel);
    mInputModeInterface->SetNumber(mInputMode);
    mHysteresisInterface->SetNumber(mHysteresisVolts * 1000.0);    // Convert V back to mV for display
}
