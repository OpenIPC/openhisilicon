驱动Build in到Kernel中的方法：

1. 在脚本中指定好要编译的kernle路径，本目录下执行脚本 ./patch.sh
   脚本说明:
   a) kernel下创建文件夹：             mkdir drivers\crypto\hisi-cipher
   b) 将SDK的drv驱动放到这个文件夹下：  board\drv\interdrv\common\cipher\src\drv -> drivers\crypto\hisi-cipher\src\drv
                                      board\drv\interdrv\common\cipher\include -> drivers\crypto\hisi-cipher\include
                                      该目录下删除用户态的头文件include\mm_ext.h
   c) drivers\crypto\Makefile文件最后增加一行：obj-y += hisi-cipher/

2. 重新编译Kernel即可

3. 重烧Kernel，不需要插hi_cipher.ko，直接运行SDK的board\drv\interdrv\common\cipher\sample\sample_cipher_via测试驱动是否Build in成功.
   如果要运行MMZ的加解密接口，需要插入hi_osal.ko

注意：cipher在Build In方式下支持的算法：
    * AES/TDES的ECB/CBC/CTR/CFB/OFB模式
    * 如果要计算CCM/GCM/MMZ的加解密，需要插入hi_osal.ko
    * sha1/sha224/sha256/sha384/sha512
    * hmac1/hmac224/hmac256/hmac384/hmac512
    * rsa1024/rsa2048/rsa4096
    * trng