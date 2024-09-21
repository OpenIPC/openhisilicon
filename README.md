# OpenHisilicon

Opensource HiSilicon/Goke SoCs SDK

Used by OpenIPC firmware and can be used by your software as well (libraries are MIT and kernel modules are GPL)

## Changes for FPV sensor modes 

# Higher FPS imx335 sensor driver on Goke/Hisilicon SoCs.
Sensor modes added:
### 2592x1520 50fps , slightly cropped vertically to 16:9
for on hi3516ev300  : Isp_FrameRate=50 , majestic  set to 1920x1080 50fps =>  50fps  
for gk7205v300 : Isp_FrameRate=48 , majestic  set to 1920x1080 48fps =>  47fps 

### 2592x1944 fullscale mode boosted to 40fps
for on hi3516ev300 set Isp_FrameRate=45, majestic to 1920x1080 fps: 45 to get 39fps
for on gk7205v300 set Isp_FrameRate=36, majestic to 1920x1080 fps: 36 to get 33fps

### 1920x1080 cropped, max 90fps , zoom 1.5x
Isp_FrameRate=90 # can work at 90fps only when majestic is set to 1280x720 !!!  
Max Isp_FrameRate=55 for 1920x1080 

### 1296x972 binning max 65fps, 
Isp_FrameRate=68 #  # max supported value on hi356ev300 at 1280x720

### 2592x1944 stock fullscale 30fps mode 
set Isp_FrameRate=30 to enable this mode
