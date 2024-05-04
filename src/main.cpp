/**
 ******************************************************************************
 * @file           : main.cpp
 * @author         : hui
 * @brief          : None
 * @attention      : None
 * @date           : 2024/2/3
 ******************************************************************************
 */

#include "gameApplication.h"
#include "QCefContext.h"

QCefConfig createQCefConfig() {

    // build QCefConfig
    QCefConfig config;
    // set user agent
    config.setUserAgent("QCefViewTest");
    // set log level
    config.setLogLevel(QCefConfig::LOGSEVERITY_DEFAULT);
    // set JSBridge object name (default value is QCefViewClient)
    config.setBridgeObjectName("CallBridge");
    // port for remote debugging (default is 0 and means to disable remote debugging)
    config.setRemoteDebuggingPort(9000);
    // set background color for all browsers
    // (QCefSetting.setBackgroundColor will overwrite this value for specified browser instance)
    config.setBackgroundColor(Qt::lightGray);

    // WindowlessRenderingEnabled is set to true by default,
    // set to false to disable the OSR mode
    config.setWindowlessRenderingEnabled(true);

    // add command line args, you can any cef supported switches or parameters
    config.addCommandLineSwitch("use-mock-keychain");
    // config.addCommandLineSwitch("disable-gpu");
    // config.addCommandLineSwitch("enable-media-stream");
    // config.addCommandLineSwitch("allow-file-access-from-files");
    // config.addCommandLineSwitch("disable-spell-checking");
    // config.addCommandLineSwitch("disable-site-isolation-trials");
    // config.addCommandLineSwitch("enable-aggressive-domstorage-flushing");
    config.addCommandLineSwitchWithValue("renderer-process-limit", "4");
    // config.addCommandLineSwitchWithValue("disable-features", "BlinkGenPropertyTrees,TranslateUI,site-per-process");

    return config;
}

int main(int argc, char *argv[]) {
    GameApplication a(argc, argv);

    QCefConfig config = createQCefConfig();
    // create QCefContext instance with config,
    // the lifecycle of cefContext must be the same as QApplication instance
    QCefContext cefContext(&a, argc, argv, &config);
 
    a.start();

    return a.exec();
}
