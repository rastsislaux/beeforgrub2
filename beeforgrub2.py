# -*- coding: utf-8 -*-
"""BLS Entry Editor for Grub2"""
import tkinter
import tkinter.filedialog
import tkinter.messagebox
from tkinter.constants import DISABLED
import json
import os
from functools import partial
from shutil import copyfile
from math import ceil
APP_TITLE = "BEE for Grub2"
APP_VERSION = "v0.2.1-beta"
APP_AUTHOR = "ungaf"
APP_LICENSE = "GPL"
PATH_TO_PY = os.path.dirname(os.path.abspath(__file__)) + '/'
HOME_DIR = os.path.expanduser("~")
DATA_DIR = os.path.join(HOME_DIR, "beeforgrub2")

try:
    with open(os.path.join(DATA_DIR, "config.json"), "r") as config_file:
        config = json.loads(config_file.read())
except FileNotFoundError:
    if not os.path.exists(DATA_DIR):
        print(os.path.exists(DATA_DIR))
        os.makedirs(DATA_DIR)
    copyfile(PATH_TO_PY+"etc/config.json", os.path.join(
        DATA_DIR,
        "config.json"
    ))
    with open(os.path.join(DATA_DIR, "config.json"), "r") as config_file:
        config = json.loads(config_file.read())

with open(PATH_TO_PY+f"locales/{config['locale']}.json", 'r') as locale_file:
    locale = json.loads(locale_file.read())
# Menuentry class
class Menuentry():
    """Menuentry class"""
    def __init__(self, file) -> None:
        """init func"""
        self.filename = file.name
        entry_text = file.read().split('\n')
        self.params = {}
        for line in entry_text:
            temp = line.strip().split()
            try:
                if temp[0] in self.params:
                    self.params.update({
                        temp[0]: f"{self.params[temp[0]]} {' '.join(temp[1:])}".strip()
                    })
                else:
                    self.params.update({
                        temp[0]: ' '.join(temp[1:])
                    })
            except IndexError:
                pass
    def __repr__(self) -> str:
        """repr func"""
        return self.filename
    def export_to_file(self, path):
        """exporting menuentry to file"""
        with open(path, 'w') as target_file:
            for key in self.params:
                if not (key in ['grub_arg', 'grub_class']):
                    target_file.write(f"{key} {self.params[key]}\n")
                else:
                    for arg in self.params[key].split():
                        target_file.write(f"{key} {arg}\n")
# App windows
class App(tkinter.Tk):
    """main window"""
    def __init__(self):
        """init func"""
        super().__init__()
        self.title(APP_TITLE)
        self.resizable(0, 1)
        self.icon = tkinter.PhotoImage(file=f"{PATH_TO_PY}etc/bee.png")
        self.tk.call('wm', 'iconphoto', self._w, self.icon)
        menu = tkinter.Menu()
        menu_look = tkinter.Menu(menu, tearoff=0)
        menu_look.add_command(label=locale['renew'], command=self.renew_entries)
        menu_about = tkinter.Menu(menu, tearoff=0)
        menu_about.add_command(label=locale['about'], command=self.open_about)
        menu.add_cascade(label=locale['look'], menu=menu_look)
        menu.add_cascade(label=locale['about'], menu=menu_about)
        self.config(menu=menu)
        with open(f'{PATH_TO_PY}etc/default.conf', 'r') as default_config:
            tkinter.Button(
                text=locale['new_entry'],
                command=partial(self.open_editor, Menuentry(default_config), True)).pack(pady=10)
        self.entry_buttons = []
        self.renew_entries()
    def renew_entries(self):
        """renew entries in main window"""
        for button in self.entry_buttons:
            button.pack_forget()
        self.entry_buttons.clear()
        found_entries = [file for file in os.listdir(
            config['entries_path']
        ) if (os.path.isfile(os.path.join(
            config['entries_path'], file
        )) and file.endswith(".conf"))]
        self.geometry(f"800x{ceil(len(found_entries)*30+53)}")
        for entry in found_entries:
            with open(os.path.join(
                config['entries_path'],
                entry
            ), 'r') as entry_file:
                new_entry = Menuentry(entry_file)
                self.entry_buttons.append(tkinter.Button(
                    self,
                    text=new_entry.filename,
                    width=98,
                    command=partial(self.open_editor, new_entry)
                    ))
                self.entry_buttons[-1].pack()
    def open_editor(self, entry, is_new = False):
        """open entry editor"""
        editor = Editor(self, entry, is_new)
        editor.mainloop()
    def open_about(self):
        """open about window"""
        about = About(self)
        about.mainloop()
class About(tkinter.Toplevel):
    """about window"""
    def __init__(self, parent):
        """init func"""
        super().__init__(parent)
        self.tk.call('wm', 'iconphoto', self._w, parent.icon)
        self.title(F"{APP_TITLE} > about")
        self.resizable(0,0)
        about_text = f"{APP_TITLE}\nVersion: {APP_VERSION}\nLicense: {APP_LICENSE}\n\
Author: {APP_AUTHOR}\n\n\
This tool allows you to edit boot loader entries that are created using Boot Loader Specification by systemd team.\n\
For more information visit: \nhttps://systemd.io/BOOT_LOADER_SPECIFICATION/"
        tkinter.Label(
            self,
            text=about_text,
            padx=10,
            pady=10
        ).pack()
class Editor(tkinter.Toplevel):
    """editor window"""
    def __init__(self, parent, entry, is_new):
        """init func"""
        super().__init__(parent)
        self.tk.call('wm', 'iconphoto', self._w, parent.icon)
        self.title(f"{APP_TITLE} > edit")
        self.geometry("500x670")
        self.resizable(0,0)
        tkinter.Label(
            self,
            text=entry.filename.split('/')[-1]).pack(
            pady=10,
            padx=10
        )
        new = {
            "title":tkinter.StringVar(),
            "version":tkinter.StringVar(),
            "machine-id":tkinter.StringVar(),
            "linux":tkinter.StringVar(),
            "initrd":tkinter.StringVar(),
            "efi":tkinter.StringVar(),
            "options":tkinter.StringVar(),
            "devicetree":tkinter.StringVar(),
            "devicetree-overlay":tkinter.StringVar(),
            "architecture":tkinter.StringVar(),
            "grub_users":tkinter.StringVar(),
            "grub_arg":tkinter.StringVar(),
            "grub_class":tkinter.StringVar()
        }
        for key in new:
            tkinter.Label(self, text=locale[key]).pack()
            if key in entry.params:
                new[key].set(entry.params[key])
            tkinter.Entry(self, textvariable=new[key], width=100).pack()
        control_buttons = tkinter.Frame(self)
        save_button = tkinter.Button(control_buttons, text=locale['save'],
            command=partial(self.save, parent, new, entry))
        save_button.grid(
                row=0, column=0
            )
        tkinter.Button(control_buttons, text=locale['saveas'],
            command=partial(self.saveas, parent, new, entry)).grid(
                row=0, column=1
            )
        delete_button = tkinter.Button(control_buttons, text=locale['delete'],
            command=partial(self.delete, parent, entry))
        delete_button.grid(
            row=0, column=2
            )
        if is_new:
            delete_button["state"] = DISABLED
            save_button["state"] = DISABLED
        control_buttons.pack(pady=10)
    def save(self, parent, new, entry):
        """save entry"""
        temp = entry
        for key in new:
            if new[key].get() != "":
                temp.params[key] = new[key].get()
        temp.export_to_file(entry.filename)
        parent.renew_entries()
        self.destroy()
    def saveas(self, parent, new, entry):
        """save entry as"""
        temp = entry
        for key in new:
            if new[key].get() != "":
                temp.params[key] = new[key].get()
        new_path = tkinter.filedialog.asksaveasfilename(
            initialdir=config['entries_path'],
            defaultextension=".conf"
        )
        temp.export_to_file(new_path)
        parent.renew_entries()
        self.destroy()
    def delete(self, parent, entry):
        """delete entry"""
        if tkinter.messagebox.askokcancel(
            f"{APP_TITLE} > ?",
            locale['delete_question']
        ):
            os.remove(entry.filename)
            parent.renew_entries()
            self.destroy()
def main():
    """Main function"""
    if os.getuid() != 0:
        root = tkinter.Tk()
        root.withdraw()
        tkinter.messagebox.showerror(
            title=f"{APP_TITLE} > Oops",
            message=locale['needs_root'])
    else:
        window = App()
        window.mainloop()
if __name__ == "__main__":
    main()
