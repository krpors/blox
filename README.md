# Blox

Pathetic attempt again to build a game using C++11.

# Build instructions

```bash
sudo apt install libsfml-dev
sudo apt install cmake
mkdir deps && cd deps
git clone https://github.com/krpors/blox
git clone https://github.com/leethomason/tinyxml2
git clone https://github.com/sainteos/tmxparser

# Build tinyxml2 version 6.0.0 (semver)
cd tinyxml2
git checkout 6.0.0
cmake CMakeLists.txt
make
sudo make install

# Build tmxparser
cd ../tmxparser
cmake CMakeLists.txt
make
sudo make install

# Configure the linker to include the /usr/local/lib path
sudo ldconfig

# Build our project
cd ../blox
make debug
```