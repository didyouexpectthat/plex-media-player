## Building

You need:

* Qt 5.6 (on OSX and windows it will be automatically downloaded when you run CMake)
* cmake 3.1 or newer
* ninja is recommended for building
* FFmpeg 3.x and mpv from github

## Building on Mac OS X

Configure

If you're happy just building from the command line then run CMake for the ninja build tool:

* Install mpv and other dependencies with homebrew:
  * ``brew install mpv --with-shared --HEAD``
* ``mkdir build ; cd build``
* ``cmake -GNinja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=output ..``

Build (ninja):

* ``ninja``

Make a distributable package:

* ``ninja install`` (be patient, it's slow)

Or if you prefer working in Xcode, run CMake for the xcode build):

* ``mkdir build ; cd build``
* ``cmake -GXcode ..``

## Building on Linux

These instructions are for Ubuntu 16.04 LTS. They were tested on a fresh install without extra options and with auto-updates applied.

First, some preparations:

* ``sudo apt install autoconf automake libtool libharfbuzz-dev libfreetype6-dev libfontconfig1-dev
    libx11-dev libxrandr-dev libvdpau-dev libva-dev mesa-common-dev libegl1-mesa-dev
    yasm libasound2-dev libpulse-dev libuchardet-dev zlib1g-dev libfribidi-dev git
    libgnutls-dev libgl1-mesa-dev cmake``
* ``mkdir pmp``
* ``cd pmp``

Systems not based on Debian/Ubuntu will have similar packages, but you'll need to figure out their names yourself.

### Downloading and installing Qt

If your distro provides Qt 5.6 or later packages, try to use them. Otherwise, proceed with these instructions.

* ``wget http://download.qt.io/official_releases/qt/5.6/5.6.1-1/qt-opensource-linux-x64-5.6.1-1.run``
* ``chmod +x qt-opensource-linux-x64-5.6.1-1.run``
* ``sudo ./qt-opensource-linux-x64-5.6.1-1.run``

A setup dialog should appear. Click through the installation. It should install itself at ``/opt/Qt5.6.1``. The instructions below assume this path, although you can change it if you know how.

### Building mpv and ffmpeg

While most distros have FFmpeg and mpv packages, they're often outdated. It's recommended to build a current version, or to get them from 3rd party sources (some are listed on https://mpv.io/installation/).

Here are instructions how to build them locally. First you need to install some build prerequisites:

* ``git clone https://github.com/mpv-player/mpv-build.git``
* ``cd mpv-build``
* ``echo --enable-libmpv-shared > mpv_options``
* you can also add ``echo --disable-cplayer >> mpv_options`` to prevent mpv CLI from being built
* ``./rebuild -j4`` (this steps checks out all sources and compiles them and takes a while)
* ``sudo ./install``
* ``sudo ldconfig``

With this, libmpv should have been installed to ``/usr/local/``. It does not conflict with the system. In particular, it does not install or use FFmpeg libraries. (The FFmpeg libraries are statically linked in libmpv when using mpv-build.)

You can also attempt to skip the installation step, and change the paths in the PMP build step to the build directory, but this is more complicated.

### Building plex-media-player

Assuming that everything else has installed correctly, building Plex Media Player should now be fairly straightforward:

* ``cd ~/pmp/``
* ``git clone git://github.com/plexinc/plex-media-player``
* ``cd plex-media-player``
* ``mkdir build``
* ``cd build``
* ``cmake -DCMAKE_BUILD_TYPE=Debug -DQTROOT=/opt/Qt5.6.1/5.6/gcc_64/ -DCMAKE_INSTALL_PREFIX=/usr/local/ ..``
* ``make -j4``
* ``sudo make install``

You should now be able to start PMP as ``plexmediaplayer`` from the terminal.

If you use your distro's Qt, omit the `-DQTROOT` argument.

Normally, the Ninja generator (via ``-GNinja``) is preferred, but cmake + ninja support appears to be broken on Ubuntu 16.04.

If you want, you can wipe the ``~/pmp/`` directory, as the PMP installation does not depend on it. Only Qt and libmpv are needed.

## License

Plex Media Player is licensed under GPL v2. See the ``LICENSE`` file.
Licenses of dependencies are summarized under ``resources/misc/licenses.txt``.
This file can also be printed at runtime when using the ``--licenses`` option.

