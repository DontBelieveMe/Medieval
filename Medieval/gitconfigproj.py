import os
def find(name, path):
    for root, dirs, files in os.walk(path):
        if name in files:
            return True
    return False

projFile = open("Medieval.vcxproj", "r")
lines = projFile.readlines()
projFile.()
print(lines)
for line in lines:
    print(line)
    if 'ClInclude' in line or 'ClCompile' in line:
        parts = line.split('"')
        print(parts)
