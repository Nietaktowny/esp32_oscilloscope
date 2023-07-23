import platform
import os

path = r"scripts/cmock_auto_generate.bash"
path_win = r'scripts\cmock_auto_generate.bat'

if platform.system() == "Linux":
    print("Checked platform: Linux")
    assert os.path.isfile(path)
    import subprocess
    subprocess.run([r'scripts/cmock_auto_generate.bash'])
elif platform.system() == "Windows":
    print("Checked platform: Windows")
    assert os.path.isfile(path_win)
    import subprocess
    subprocess.run([r'scripts\cmock_auto_generate.bat'])
else:
    print("Checked platform: Mac")
    assert os.path.isfile(path)
    import subprocess
    subprocess.run([r'scripts/cmock_auto_generate.bash'])





