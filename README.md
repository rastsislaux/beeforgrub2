## BEE (BLS Entries Editor) for Grub2
Tool to edit BLS-specified boot loader entries for Grub2
More info about specification:<br>https://systemd.io/BOOT_LOADER_SPECIFICATION/

### How to use
0. For ubuntu: sudo apt install python3-tk
1. run it from terminal using "python beeforgrub2.py" or just click it twice if you have it as a binary
2. Choose an entry you want to edit or create a new one
3. Edit what you need and Save or Delete

### Default settings
All settings are stored in a JSON file at /root/beeforgrub2/config.json<br>
Default path to boot loader entry files is /boot/loader/entries<br>
Default language is English (en_EN) but Russian (ru_RU) is also available.

### Compile it yourself using pyinstaller
sh pyinstall.sh
