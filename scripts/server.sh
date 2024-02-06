#/bin/sh

set -e

npx nodemon --watch build/asteroids.* --exec "sh scripts/copy-web-artifacts.sh" &
npx live-server --port=3000 --no-browser ./src/web &

wait