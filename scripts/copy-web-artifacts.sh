#/bin/sh

set -e

find ./build -depth 1 -name 'asteroids.*'  -print -exec cp {} src/web \;