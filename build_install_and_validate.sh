#!/bin/bash

# This marks the AU's directory as a bundle. I'm not sure if this is truly necessary, but
# otherwise it will appear in Finder as a regular directory.
SetFile -a B MinimalAU.component

# Build as a dynamic library, and put the binary in the package dir
clang -o MinimalAU.component/Contents/MacOS/MinimalAU -dynamiclib MinimalAU.c -framework AudioUnit -framework AudioToolbox -framework CoreFoundation

# Copy the package/bundle to the user plugin location
cp -R MinimalAU.component ~/Library/Audio/Plug-Ins/Components

# Without this, sometimes a new AU won't show up in host apps or auval
echo "Build complete. Killing the AudioComponentRegistrar to rescan plugins."
killall -9 AudioComponentRegistrar

# Attempt to validate. This will fail, but should at least recognize the plugin, and call the
# initialization functions.
auval -v aufx Mini Car7
