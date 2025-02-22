#############################################
# OpenRGB Artifacts Extract/Prepare Script  #
#                                           #
# Downloads artifacts from GitLab CI,       #
# extracts the zip archives, and renames    #
# them to the format used for release       #
# downloads on the website.                 #
#############################################

import os

#############################################
# OpenRGB version                           #
#############################################
version = "1.0rc1"

#############################################
# Git version (tag, branch, or commit)      #
#############################################
git_id = "release_candidate_1.0rc1"

command = "git rev-list -n 1 " + git_id
print(command)
commit = os.popen(command).read().strip()

command = "git rev-parse --short=7 " + commit
print(command)
commit = os.popen(command).read().strip()

git_url = "https://gitlab.com/CalcProgrammer1/OpenRGB/-/jobs/artifacts/" + git_id + "/download?job="

#############################################
# List of artifacts to download             #
#############################################
artifacts = [
    [ "Windows%2064%20MSI",                         "OpenRGB_" + version + "_Windows_64_"     + commit, ".msi"      ],
    [ "Windows%2064",                               "OpenRGB_" + version + "_Windows_64_"     + commit, ".zip"      ],
    [ "Windows%2032",                               "OpenRGB_" + version + "_Windows_32_"     + commit, ".zip"      ],
    [ "Linux+amd64+.deb+%28Debian+Bookworm%29",     "openrgb_" + version + "_amd64_bookworm_" + commit, ".deb"      ],
    [ "Linux+i386+.deb+%28Debian+Bookworm%29",      "openrgb_" + version + "_i386_bookworm_"  + commit, ".deb"      ],
    [ "Linux%20amd64%20AppImage",                   "OpenRGB_" + version + "_x86_64_"         + commit, ".AppImage" ],
    [ "Linux%20i386%20AppImage",                    "OpenRGB_" + version + "_i386_"           + commit, ".AppImage" ],
    [ "Linux+64+F40+rpm",                           "openrgb_" + version + "_x86_64_f40_"     + commit, ".rpm"      ],
    [ "Linux+arm64+.deb+%28Debian%20Bookworm%29",   "openrgb_" + version + "_arm64_bookworm_" + commit, ".deb"      ],
    [ "Linux+armhf+.deb+%28Debian%20Bookworm%29",   "openrgb_" + version + "_armhf_bookworm_" + commit, ".deb"      ],
    [ "Linux%20arm64%20AppImage",                   "OpenRGB_" + version + "_arm64_"          + commit, ".AppImage" ],
    [ "Linux%20armhf%20AppImage",                   "OpenRGB_" + version + "_armhf_"          + commit, ".AppImage" ],
    [ "MacOS%20Intel",                              "OpenRGB_" + version + "_MacOS_Intel_"    + commit, ".zip"      ],
    [ "MacOS%20ARM64",                              "OpenRGB_" + version + "_MacOS_ARM64_"    + commit, ".zip"      ]
]

command = "mkdir artifacts"
print(command)
os.system(command)

command = "mkdir download"
print(command)
os.system(command)

#############################################
# Loop through all artifacts                #
#############################################
for artifact in artifacts:
    #########################################
    # Download the artifact                 #
    #########################################
    command = "wget " + git_url + artifact[0] + " -O download.zip"
    print(command)
    os.system(command)

    if(artifact[2] != ".zip"):
        #########################################
        # Unzip the downloaded artifact         #
        #########################################
        command = "unzip download.zip -d download"
        print(command)
        os.system(command)

        #########################################
        # Delete downloaded zip                 #
        #########################################
        command = "rm download.zip"
        print(command)
        os.system(command)

        #########################################
        # Rename downloaded artifact file and   #
        # move it to artifacts folder           #
        #########################################
        command = "mv download/*" + artifact[2] + " artifacts/" + artifact[1] + artifact[2]
        print(command)
        os.system(command)

        #########################################
        # If AppImage, also copy udev rules     #
        #########################################
        if(artifact[2] == ".AppImage"):
            command = "mv download/60-openrgb.rules artifacts/60-openrgb.rules"
            print(command)
            os.system(command)

        #########################################
        # Delete any remaining download files   #
        #########################################
        command = "rm -r download/*"
        print(command)
        os.system(command)

    else:
        #########################################
        # Rename downloaded zip and move it to  #
        # artifacts folder                      #
        #########################################
        command = "mv download.zip artifacts/" + artifact[1] + artifact[2]
        print(command)
        os.system(command)

#############################################
# The openrgb-udev-install.sh script is     #
# not a CI artifact.  Download it           #
# separately.                               #
#############################################
udev_url = "https://gitlab.com/CalcProgrammer1/OpenRGB/-/raw/master/scripts/openrgb-udev-install.sh"

command = "wget " + udev_url + " -O artifacts/openrgb-udev-install.sh"
print(command)
os.system(command)
