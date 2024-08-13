# GTA Launcher
A program to launch <b>GTA1</b> using DOSBox.

![gta_menu](https://github.com/user-attachments/assets/01c12aed-24e6-432a-80b6-04e74b3a02d9)

# Reason to use
GTA1 with every expansion pack comes with 11 exe files. There's a simple launcher batch script in the <b>Max Pack</b> that explains which exe does what. You can start a game by pressing a number.<br><br>However, I like to play GTA1 on my phone using RetroArch DOSBox core. I had to remap every button on my controller to a key that the game uses, so I don't have any left for the numbers that the Max Pack batch script uses. Since I can't use the arrow keys in batch, I went the extra mile and made a C program with custom ANSI art and sound support.

# Controls
Up/Down to select a game<br>
Left/Right to switch between color modes and settings<br>
Enter/Esc to confirm or go back

# FAQ
- Where do I put the EXE and the GTAMENU folder?<br>
Inside your GTA1 folder, next to GTADATA and GTADOS folders.
- No ingame music?<br>
The program will automatically try to mount the proper discs for music. Name them GTA.CUE for the base game and GTAUK.CUE for the expansion packs.
- Why can't I select low color mode for London1961?<br>
That expansion pack never had low color mode officially, therefore I made it unselectable.
- Where did you get the ANSI art?<br>
Made them using TheDraw. The logo is from the GameBoy version of the game.

# Programs used
Turbo C++ 3<br>
TheDraw
