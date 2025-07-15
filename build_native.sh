export PATH="/opt/homebrew/bin:$PATH"

meson setup build/native --buildtype=debug --default-library=static -Dplatform=native

meson compile -C build/native