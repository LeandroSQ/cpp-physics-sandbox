#/bin/sh

set -e

find ./build -depth 1 -name 'sandbox.*'  -print -exec cp {} src/web \;