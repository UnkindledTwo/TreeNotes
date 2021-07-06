![logo](/Resources/Icon.png)
# TreeNotes
A Note Taking Application Made With Qt.

## Building
You can build this application like you would a Qt app normally, 
* [Windows](https://doc.qt.io/qt-5/windows-deployment.html)
* [Linux (x11)](https://doc.qt.io/qt-5/linux-deployment.html)
* [Mac](https://doc.qt.io/qt-5/macos.html#deploying-applications-on-macos)

### Note
You need to remove windows only code in mac and linux versions. Simply remove the word `winextras` from TreeNotes.pro and remove the block:
```cpp
 54   QString styleSheetFocus = "border: 1.50px solid " + QtWin::colorizationColor().name() + ";";
 55   ui->messageEdit->setStyleSheet(ui->messageEdit->styleSheet() + styleSheetFocus);
 56   ui->treeWidget->setStyleSheet(ui->treeWidget->styleSheet() + styleSheetFocus);
 57   ui->titleEdit->setStyleSheet(ui->titleEdit->styleSheet() + styleSheetFocus);
```
from `treenotes.cpp`

To run this app on linux, you need to install `libxkbcommon-x11-0`. 

## Configuring
After you launch the app the first time, edit the settings.ini that is created in the directory of the app. 
You can change the values under [AppConfig].
