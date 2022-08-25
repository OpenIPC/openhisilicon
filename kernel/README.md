## Opensourced HiSilicon kernel modules

### Build manually

```console
export PATH=/path/to/openipc_sdk/bin:$PATH
make \
    ARCH=arm CROSS_COMPILE=arm-openipc-linux-musleabi- \
    CHIPARCH=hi3516ev200 \
    -C ~/git/firmware/output/build/linux-4.9.37 \
    M=$PWD
```

where `CHIPARCH` can be set to options `hi3516ev200` or `gk7205v200`

### Reformat source code to Linux kernel style

```console
find . -iname "*.h" -o -iname "*.c" | xargs clang-format -i
```
