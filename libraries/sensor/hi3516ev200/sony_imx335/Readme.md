# Higher FPS imx335 sensor driver on Goke/Hisilicon SoCs.
Sensor modes added:
### 2592x1520 50fps , slightly cropped vertically to 16:9
for on hi356ev300  : Isp_FrameRate=50 , majestic  set to 1920x1080 50fps =>  50fps  
for gk7205v300 : Isp_FrameRate=48 , majestic  set to 1920x1080 48fps =>  47fps 

### 2592x1944 fullscale mode boosted to 40fps
for on hi356ev300 set Isp_FrameRate=45, majestic to 1920x1080 fps: 45 to get 39fps
for on gk7205v300 set Isp_FrameRate=36, majestic to 1920x1080 fps: 36 to get 33fps

### 1920x1080 cropped, max 90fps , zoom 1.5x
Isp_FrameRate=90 # can work at 90fps only when majestic is set to 1280x720 !!!  
Max Isp_FrameRate=55 for 1920x1080 

### 1296x972 binning max 65fps, 
Isp_FrameRate=68 #  # max supported value on hi356ev300 at 1280x720

### 2592x1944 stock fullscale 30fps mode 
set Isp_FrameRate=30 to enable this mode


# To configure.

The driver is shipped as `libsns_imx335.so` in the firmware package
(`hisilicon-osdrv-hi3516ev200` / `hisilicon-osdrv-gk7205v200`). A matching
preset `imx335.ini` lives alongside it under
`/etc/sensors/imx335.ini` on the device.

In `/etc/majestic.yaml`:
```
video0:
  codec: h265
  rcMode: cbr
  gopSize: 1.5
  size: 1920x1080
  fps: 45
...
isp:
  sensorConfig: /etc/sensors/imx335.ini
```

The preset `imx335.ini` contains commented-out `DevRect_w` / `DevRect_h` /
`Isp_FrameRate` blocks — uncomment the one for the desired mode.

All modes will work at fps 45; some support higher refresh rates per the
table above.