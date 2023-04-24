## Build libraries

```console
export PATH=/path/to/openipc_sdk/bin:$PATH
make CROSS_COMPILE=arm-openipc-linux-musleabi- CHIPARCH=gk7205v200
```

path to openipc_sdk is the toolchain tipically located in output/host directory

you may need export SDK_CODE:

for hi3516ev200
```console
export SDK_CODE=0x3516E200
```

for gk7205v200
```console
export SDK_CODE=0x7205200
```

