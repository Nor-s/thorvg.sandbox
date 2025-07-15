export PATH="/opt/homebrew/bin:$PATH"

meson setup build/wasm --buildtype=debug --cross-file wasm.ini --default-library=static -Dplatform=web

meson compile -C build/wasm