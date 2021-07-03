# TreeNotes
A Note Taking Application Made With Qt.

# Building
## Windows
1. Install Qt 5 and QtCreator
2. CLone the source code `git clone https://github.com/UnkindledTwo/TreeNotes.git`
3. Open QtCreator
4. Open the project
5. On the bottom left, click on the project name and select release.
6. Build the app
7. Open the Qt Command prompt
8. Navigate to the newly built app's directory
9. Execute the command `windeployqt TreeNotes.exe -release --no-transtalions --no-opengl-sw`
10. All Done!

## Linux
1. Install Qt 5 and gnu make
2. Remove all the windows only code from the application
3. Execute `qmake`
4. Execute `make`
5. All Done!
