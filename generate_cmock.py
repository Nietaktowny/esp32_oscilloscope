import platform


if platform.system() == "Linux":
    print("Checked platform: Linux")
    import subprocess
    subprocess.call([r'scripts/cmock_auto_generate.bash'])
elif platform.system() == "Windows":
    print("Checked platform: Windows")
    import subprocess
    subprocess.call([r'scripts\cmock_auto_generate.bat'])
else:
    print("Mac")





