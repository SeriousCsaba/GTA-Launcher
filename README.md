# GTA Launcher
A program to launch <b>GTA1</b> easier inside DOSBox. It's similar to the launcher in the <b>Max Pack</b> but you can control it with the arrow keys instead of pressing numbers.

![GTA-Launcher](https://user-images.githubusercontent.com/56899074/126869958-d887e607-df1e-421f-93d3-3fb9531e4947.png)

# Reason to use
GTA1 with every expansion pack comes with 11 exe files. There's a simple launcher script in the Max Pack just to make it clear which exe does what.<br><br>I like to play GTA1 on my phone using RetroArch DOSBox core. I had to remap every button on my gamepad to a key that the game uses so it would be really inconvenient to somehow remap numbers too that the Max Pack batch script uses. Since I can't really use the arrow keys in batch, I went the extra mile and made a C program with custom ANSI art and sound support.

# Controls
- Main menu<br>
Here you can open the Start menu or the Settings menu
- Start menu<br>
Select a flag to launch a game: GTA1 (gtaX.exe), GTA1 London1969 (gtaX_uk.exe), GTA1 London1961 (gtaX_61.exe)<br>
(X stands for 8 or 24 or fx, meaning low color mode, high color mode or 3Dfx mode)<br>
You can select color mode with the up/down keys. Default is high color.
- Settings menu<br>
Select a flag to launch the settings of a game: GTA1 settings (k.exe), GTA1 London1969 settings (k_uk.exe), GTA1 London1961 settings (k_61.exe)
<br><br>
Left/Right to switch between icons<br>
Up/Down to select color mode at the Start menu (default is high color)<br>
Enter/Esc to confirm or go back

# FAQ
- Where do I put the exe and the gtamenu folder?<br>
Inside your GTA1 folder, next to gtadata and gtados folders.
- No ingame music?<br>
The program will automatically try to mount the proper discs for music. Create a gtamusic folder and put your bin/cue files in there. Name them music.cue for the base game and music_uk.cue for the expansion packs. I'm NOT providing you these files.
- Why can't I select low color mode for London1961?<br>
That expansion pack never had low color mode officially, therefore I made it unselectable.
- Where did you get the ANSI art?<br>
Made them using TheDraw. The logo is from the GameBoy version of the game.
- Can I disable sound?<br>
I included source code for no sound version separately along with a bunch of other minor launchers I worked on.

# Programs used
Turbo C++ 3<br>
TheDraw
