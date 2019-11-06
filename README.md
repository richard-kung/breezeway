<p align="center"><img src="breezewaymod_icon.png" alt="BreezewayMod" width="100" height="100"></p>

<h1 align="center">BreezewayMod</h1>

**BreezewayMod** is a fork of the [Breezeway](https://github.com/richard-kung/breezeway) window decoration, which aims to mimic the **macOS style**, but with **power ups**.

> **Note**: It comes without weird glitches on the rounded corners.

## Preview

![](screenshot.png?raw=true)

> Looks great  with light and dark color schemes. Shown here with [Arc](https://github.com/PapirusDevelopmentTeam/arc-kde) and [Arc-Dark](https://github.com/PapirusDevelopmentTeam/arc-kde) color schemes.


| **The usual glitch on dark themes**                          | ![](sierrabreeze.png?raw=true) |
| :----------------------------------------------------------- | :----------------------------: |
| **Glitch be gone with BreezewayMod**                         | ![](breezewaymod.png?raw=true) |
| **No more overlapping windows with not enough contrast between each other** | ![](low_contrast.png?raw=true) |
| **BreezewayMod comes with a neat highlight line across the top of titlebars** |  ![](gradients.png?raw=true)   |

> **Note**: If you wish to have subtle gradients with a nicely drawn separator for titlebars, you can have just that. The gradient and separator will, like all nifty color features, use the title bar color to determine their values. *—How cool is that?*

## Experimental Features

With the power of Halloween and the dark arts combined, we can now match the titlebars color to the windows color. And I don't mean some lame shit where I make use of `QPalette::Window` in the hopes of getting a nice looking window color. *—Hmmm, no, no no!*

![](match_colors.png?raw=true)

> **Note**: This feature is still in early beta though and color glitches ***will*** occur. Please uncheck **Match titlebar color to window color (beta)** in **BreezewayMod settings** to turn this feature off.

## Building and Installation

Before proceeding with the actual installation process, make sure you have the following dependencies installed:

* cmake
* g++ / clang
* extra-cmake-modules
* libkdecorations2-dev
* ibkf5guiaddons-dev
* libkf5configwidgets-dev
* libkf5windowsystem-dev
* libkf5package-dev
* libqt5x11extras5-dev
* gettext
* pkg-config *(optional)*

Download the [latest release](https://github.com/n4n0GH/breezeway-mod/releases) or clone this repository:

```
git clone https://github.com/n4n0GH/breezeway-mod
```
> **Note**: It's recommended to clone it where you can leave the built directory, just in case you want to uninstall **Breezeway-mod**. However, if you accidentally removed the directory, please check below for manually uninstallation instructions.

Change to `breezeway-mod` directory and create a new directory called `build`:

```
cd breezeway-mod && mkdir build && cd build
```

To install, use `cmake`, `make` and `make install`:

```
cmake -DCMAKE_INSTALL_PREFIX=/usr ..
make && sudo make install
```

## Uninstalling

In the build directory, run:

```
sudo make uninstall
```

If you removed the installation directory, you'll have to delete the following files and folders:

```
/usr/lib64/cmake/BreezewayMod/BreezewayConfig.cmake
/usr/lib64/cmake/BreezewayMod/BreezewayConfigVersion.cmake
/usr/lib64/qt/plugins/org.kde.kdecoration2/breezewaydecoration.so
/usr/share/kservices5/breezewaydecorationconfig.desktop
/usr/lib64/libbreezewaycommon5.so.5.16.5
/usr/lib64/libbreezewaycommon5.so.5
```

## Contribution

If you have any questions regarding **BreezewayMod**, you are welcome to [open an issue](https://github.com/n4n0GH/breezeway-mod/issues).

## FAQ

**• What's the biggest differences between this theme and [Breezeway](https://github.com/richard-kung/breezeway)?**  
As of now there are so many changes that it's fair to say it's a completely different theme.
