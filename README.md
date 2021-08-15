## beeforgrub2
Tool to edit BLS-specified boot loader entries for Grub2

### How to use
1. Run it from terminal using "sudo python beeforgrub2.py" or "sudo ./beeforgrub2" (if you have it as a binary)
2. Choose an entry you want to edit or create a new one
3. Edit what you need and Save or Delete

### Default settings
All settings are stored in a JSON file at /root/beeforgrub2/config.json<br>
Default path to boot loader entry files is /boot/loader/entries<br>
Default language is English (en_EN) but Russian (ru_RU) is also available.

### Compile it yourself using pyinstaller
cd {directory}<br>
pyinstaller --onefile --windowed --add-data "locale:locale" --add-data "etc:etc" beeforgrub2.py
