![Screenshot](screenshot.png)

# An in-depth guide to getting started with SDL3 in the C Language.

# ArchLinux instructions.
You will need to make sure SDL3, SDL3_image, SDL3_ttf and SDL3_mixer is installed.
```
sudo pacman -S --needed base-devel sdl3
```
```
cd
git clone https://aur.archlinux.org/sdl3_image-git.git
cd sdl3_image-git
makepkg -i
```
```
cd
git clone https://aur.archlinux.org/sdl3_ttf-git.git
cd sdl3_ttf-git
makepkg -i
```
```
cd
git clone https://aur.archlinux.org/sdl3_mixer-git.git
cd sdl3_mixer-git
makepkg -i
```
```
cd
git clone https://github.com/ProgrammingRainbow/Beginners-Guide-to-SDL3-in-C
cd Beginners-Guide-to-SDL3-in-C
make run
```
The Makefile supports these commands:
```
make rebuild
make clean
make release
make debug
SRC_DIR=Video8 make rebuild run
```
# Controls
Space - Changes background Color\
Arrows - Moves sprite\
M - Toggles music mute\
Escape - Quits
