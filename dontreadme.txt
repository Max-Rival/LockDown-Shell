[---------------------------------------------------------------------]
[       __   __        __   __                __        ___           ]
[ |    /  \ /  ` |__/ |  \ /  \ |  | |\ |    /__` |__| |__  |    |    ]
[ |___ \__/ \__, |  \ |__/ \__/ |/\| | \|    .__/ |  | |___ |___ |___ ]
[---------------------------------------------------------------------]

[Release 1 - Beta Build]

[Build]
./build.sh
useradd shelluser
#change the /etc/passwd file for the user from /bin/bash to /bin/LDShell

[Bug Fixes]
-Fixed Segmentation fault when attempting to read or write to .. or .
-checkTraversal() has been changed to checkSecurity()


[ToDo]
-Add configuration file options for administrators. (Custom messages, permissions)
-Write the RSCR command.

Credits:
Max - Rival - siteofmax.com - rival@riseup.net