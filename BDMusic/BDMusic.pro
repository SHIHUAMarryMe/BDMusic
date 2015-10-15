# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = BDMusic

CONFIG += sailfishapp
CONFIG += c++11

SOURCES += src/BDMusic.cpp \
    src/parseAPI.cpp \
    src/mediaPlayer.cpp
QT += network \
multimedia

OTHER_FILES += qml/BDMusic.qml \
    qml/cover/CoverPage.qml \
    qml/pages/FirstPage.qml \
    rpm/BDMusic.changes.in \
    rpm/BDMusic.spec \
    rpm/BDMusic.yaml \
    translations/*.ts \
    BDMusic.desktop \
    qml/pages/playSong.qml \
    qml/pages/ShowSongPic.qml \
    qml/pages/MediaPlyOperator.qml \
    qml/pages/PlayListPage.qml

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/BDMusic-de.ts

HEADERS += \
    src/parseAPI.h \
    src/mediaPlayer.h


