# TreeNotes
A Note Taking Application Made With Qt.

#Building
##Windows
*Install Qt 5 and QtCreator
*CLone the source code `git clone https://github.com/UnkindledTwo/TreeNotes.git`
*Open QtCreator
*Open the project
*On the bottom left, click on the project name and select release.
*Build the app
*Open the Qt Command prompt
*Navigate to the newly built app's directory
*Execute the command `windeployqt TreeNotes.exe -release --no-transtalions --no-opengl-sw`
*All Done!

#Linux
*Install Qt 5 and gnu make
*Remove all the windows only code from the application
*Execute `qmake`
*Execute `make`
*All Done!
