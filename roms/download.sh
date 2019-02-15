#!/bin/bash

cd "${0%/*}"

get_rom() {
    local ROM="$1"
    local URL="$2"
    local ZIP="$ROM.zip"
    if [ ! -f "$ROM" ]; then
        wget "$URL" -O "$ZIP"
        local FILE=$(zipinfo -1 "$ZIP")
        unzip "$ZIP"
        mv "$FILE" "$ROM"
        rm "$ZIP"
        echo "Downloaded $ROM"
    else
        echo "Found existing $ROM"
    fi
}

get_rom "Mario_Kart_64.z64" "https://s2roms.cc/s3roms/Nintendo%2064/L-O/Mario%20Kart%2064%20%28E%29%20%28V1.1%29%20%5B%21%5D.zip"
get_rom "Paper_Mario.z64" "https://s3roms.cc/s3roms/Nintendo%2064/P-T/Paper%20Mario%20%28U%29%20%5B%21%5D.zip"
get_rom "Super_Smash_Bros.z64" "https://s3roms.cc/s3roms/Nintendo%2064/P-T/Super%20Smash%20Bros.%20%28U%29%20%5B%21%5D.zip"
#get_rom "LoZ_Ocarina_Of_Time.z64" "https://s1roms.cc/s3roms/Nintendo%2064/L-O/Legend%20of%20Zelda%2C%20The%20-%20Ocarina%20of%20Time%20%28U%29%20%28V1.2%29%20%5B%21%5D.zip"

wait
echo "All Files Downloaded"