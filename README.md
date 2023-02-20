# Breezeway
## About
Breezeway is a KDE Plasma window decoration that mimics macOS style.

![](screenshot1.png)
![](screenshot2.png)

## Build
### prerequisities
    sudo apt install cmake extra-cmake-modules libkdecorations2-dev libkf5coreaddons-dev libkf5guiaddons-dev libkf5configwidgets-dev libkf5windowsystem-dev libkf5iconthemes-dev libqt5x11extras5-dev

### build
    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX=`kf5-config --prefix` ..
    make
    sudo make install

