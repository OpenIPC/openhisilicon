## Opensourced HiSilicon kernel modules

### Build manually

```console
export PATH=/path/to/openipc_sdk/bin:$PATH
make ARCH=arm CROSS_COMPILE=arm-openipc-linux-musleabi- -C ~/git/firmware/output/build/linux-4.9.37 M=$PWD
```

### Reformat source code to Linux kernel style

```console
find . -iname "*.h" -o -iname "*.c" | xargs clang-format -i
```
