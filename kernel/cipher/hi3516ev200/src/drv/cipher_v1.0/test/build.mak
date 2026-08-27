CIPHER_CFLAGS += -I$(CIPHER_DIR)/drv/cipher_v1.0/test
CIPHER_CFLAGS += -DKAPI_TEST_SUPPORT

CIPHER_OBJS   += drv/cipher_v1.0/test/kapi_test_main.o
CIPHER_OBJS   += drv/cipher_v1.0/test/kapi_symc_test.o


