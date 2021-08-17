import os
PATH_TO_PY = os.path.dirname(os.path.abspath(__file__)) + '/'
su = ['gksudo', 'kdesudo', 'kdesu', 'gksu', 'sudo']
for com in su:
    if os.path.exists(
        os.path.join(
            '/usr/bin',
            com
        )
    ):
        os.system(f"{com} python \"{os.path.join(PATH_TO_PY, 'app/beeforgrub2.py')}\"")
        break