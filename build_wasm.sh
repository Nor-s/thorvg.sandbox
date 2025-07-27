export PATH="/opt/homebrew/bin:$PATH"

meson setup build/wasm --buildtype=release --cross-file wasm.ini --default-library=static -Dplatform=web

meson compile -C build/wasm