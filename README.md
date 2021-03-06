![logo](/Resources/Icon.png)
# TreeNotes
A Note Taking Application Made With Qt.

## Building
You can build this application like you would a Qt app normally, 
* [Windows](https://doc.qt.io/qt-5/windows-deployment.html)
* [Linux (x11)](https://doc.qt.io/qt-5/linux-deployment.html)
* [Mac](https://doc.qt.io/qt-5/macos.html#deploying-applications-on-macos)

### Notes For Deployment
* You need to put `libcrypto-1_1-x64.dll` and `libssl-1_1-x64.dll` in the .exe's directory when deploying for windows.

* You need openssl to be installed for linux

* To run this app on linux, you need to install `libxkbcommon-x11-0`. 

## Multiple Save Files
To have another save file. Run `TreeNotes {Save File Name}`. Make sure the save file name is reachable by the app and is a legal file name.

## Configuring
After you launch the app the first time, edit the settings.ini that is created in the directory of the app. 
You can change the values under [AppConfig].

Or you can use the settings menu from `Other -> Settings`

![screenshot(Dark)](/Resources/screenshot_light.PNG)
![screenshot(Light)](/Resources/screenshot_dark.PNG)
![screenshot(Windows Native)](/Resources/screenshot_native_windows.PNG)

