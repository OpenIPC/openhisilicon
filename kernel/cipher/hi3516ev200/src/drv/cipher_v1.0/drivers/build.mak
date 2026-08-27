CIPHER_CFLAGS += -I$(CIPHER_DIR)/drv/cipher_v1.0/drivers/core/include
CIPHER_CFLAGS += -I$(CIPHER_DIR)/drv/cipher_v1.0/drivers/crypto/include
CIPHER_CFLAGS += -I$(CIPHER_DIR)/drv/cipher_v1.0/drivers/extend
CIPHER_CFLAGS += -I$(CIPHER_DIR)/drv/cipher_v1.0/drivers/extend/include

CIPHER_OBJS +=  drv/cipher_v1.0/drivers/core/drv_symc_v100.o          \
                drv/cipher_v1.0/drivers/core/drv_symc_v200.o          \
                drv/cipher_v1.0/drivers/core/drv_hash_v100.o          \
                drv/cipher_v1.0/drivers/core/drv_hash_v200.o          \
                drv/cipher_v1.0/drivers/core/drv_ifep_rsa_v100.o      \
                drv/cipher_v1.0/drivers/core/drv_trng_v100.o          \
                drv/cipher_v1.0/drivers/core/drv_trng_v200.o          \
                drv/cipher_v1.0/drivers/core/drv_lib.o

CIPHER_OBJS += drv/cipher_v1.0/drivers/crypto/cryp_symc.o             \
               drv/cipher_v1.0/drivers/crypto/cryp_hash.o             \
               drv/cipher_v1.0/drivers/crypto/cryp_trng.o             \
               drv/cipher_v1.0/drivers/crypto/cryp_rsa.o

CIPHER_OBJS += drv/cipher_v1.0/drivers/kapi_symc.o                    \
               drv/cipher_v1.0/drivers/kapi_hash.o                    \
               drv/cipher_v1.0/drivers/kapi_rsa.o                     \
               drv/cipher_v1.0/drivers/kapi_trng.o                    \
               drv/cipher_v1.0/drivers/kapi_dispatch.o

CIPHER_OBJS += drv/cipher_v1.0/drivers/extend/mbedtls/bignum.o        \
               drv/cipher_v1.0/drivers/extend/mbedtls/md.o            \
               drv/cipher_v1.0/drivers/extend/mbedtls/rsa.o           \
               drv/cipher_v1.0/drivers/extend/mbedtls/asn1parse.o     \
               drv/cipher_v1.0/drivers/extend/mbedtls/oid.o           \
               drv/cipher_v1.0/drivers/extend/mbedtls/platform_util.o \
               drv/cipher_v1.0/drivers/extend/mbedtls/rsa_internal.o

CIPHER_OBJS += drv/cipher_v1.0/drivers/extend/ext_aead.o              \
               drv/cipher_v1.0/drivers/extend/ext_hash.o              \
               drv/cipher_v1.0/drivers/extend/ext_symc.o              \
               drv/cipher_v1.0/drivers/extend/ext_sm3.o               \
               drv/cipher_v1.0/drivers/extend/ext_sm4.o