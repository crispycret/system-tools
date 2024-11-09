To filter out network interfaces that are not in use (i.e., those that show "no wireless extensions"), you can use grep along with awk or sed to list only the interfaces that have wireless extensions enabled. Here's how you can do it.
Step 1: Use iwconfig and grep

You can run iwconfig and use grep to filter out the lines that contain the phrase "no wireless extensions.".

bash

iwconfig 2>/dev/null | grep -B 1 "ESSID" | grep -v "no wireless extensions"

Explanation:

    iwconfig 2>/dev/null: This runs iwconfig but suppresses error messages (2>/dev/null).
    grep -B 1 "ESSID": This looks for lines containing "ESSID" and includes the line above it, which contains the interface name.
    grep -v "no wireless extensions": This removes lines that contain the phrase "no wireless extensions".

This will output the interfaces in use, like:

arduino

wlp2s0    IEEE 802.11  ESSID:"YourNetworkSSID"

Step 2: Extract Only Interface Names

If you want to get only the interface names, you can modify the command to further process the output with awk or cut:

bash

iwconfig 2>/dev/null | grep -B 1 "ESSID" | grep -v "no wireless extensions" | awk 'NR % 2 == 1 {print $1}'

Explanation:

    NR % 2 == 1 {print $1}: This ensures that only the interface names (which appear on every second line) are printed.

Alternatively, you can use cut to achieve the same result:

bash

iwconfig 2>/dev/null | grep -B 1 "ESSID" | grep -v "no wireless extensions" | cut -d ' ' -f 1

This will output just the interface name, like:

wlp2s0

Step 3: Use in a Script

If you want to use this in a script to get the in-use wireless interfaces dynamically, you can wrap it in a script like so:

bash

#!/bin/bash
interfaces=$(iwconfig 2>/dev/null | grep -B 1 "ESSID" | grep -v "no wireless extensions" | awk 'NR % 2 == 1 {print $1}')
echo "Active wireless interfaces: $interfaces"

This will give you a list of all active wireless interfaces on your system. You can also use this output in further network monitoring or configuration automation tasks.