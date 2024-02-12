# filemonitor Daemon

filemonitor is a Linux daemon designed to monitor specified files for changes, including when they are opened, written to, moved, or closed. Upon detecting any of these events, filemonitor sends a desktop notification in real time.

## Compilation
You can sumply run `make` to compile the daemon

__Note:__, to compile manually, use the following command, ensuring you have `pkg-config` and `libnotify` installed and link the library with 
```
gcc filemonitor.c -o filemonitor `pkg-config --cflags --libs libnotify`
```
