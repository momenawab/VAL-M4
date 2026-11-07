#!/bin/zsh
# usage: run_riot.sh <label> ; launches Riot Client in val-prefix2 with Wine 11.21 and logs to run-<label>.log
W=~/wine-11.21/"Wine Staging.app/Contents/Resources/wine/bin"
export WINEPREFIX=~/val-prefix2
"$W/wineserver" -k 2>/dev/null; sleep 3
cd ~/VAL-M4/experiments/vanguard
WINEDEBUG=err+all nohup "$W/wine" "C:\\Riot Games\\Riot Client\\RiotClientServices.exe" --launch-product=valorant --launch-patchline=live > "run-$1.log" 2>&1 &
