#!/bin/zsh
# usage: cx_try.sh <backend>
pgrep -f -i "Riot|wineserver|winedevice|services.exe|explorer.exe" | while read p; do c=$(ps -o command= -p $p | cut -c1-100); case "$c" in *Antigravity*|*claude*|*zsh*|*pgrep*|*"CrossOver.app/Contents/MacOS"*) ;; *) kill -9 $p;; esac; done; sleep 2
B="$HOME/Library/Application Support/CrossOver/Bottles/RiotVAL"
python3 -I - "$B/cxbottle.conf" "$1" <<'PY'
import sys,re
p,v=sys.argv[1],sys.argv[2]
s=open(p).read()
s=re.sub(r'^"CX_GRAPHICS_BACKEND".*\n','',s,flags=re.M)
s=s.replace('[EnvironmentVariables]\n','[EnvironmentVariables]\n"CX_GRAPHICS_BACKEND" = "%s"\n'%v,1)
open(p,'w').write(s)
PY
CX=/Applications/CrossOver.app/Contents/SharedSupport/CrossOver/bin
cd ~/VAL-M4/experiments/vanguard
nohup "$CX/cxstart" --bottle RiotVAL "C:\\Riot Games\\Riot Client\\RiotClientServices.exe" --launch-product=valorant --launch-patchline=live > "crossover-run-$1.log" 2>&1 &
