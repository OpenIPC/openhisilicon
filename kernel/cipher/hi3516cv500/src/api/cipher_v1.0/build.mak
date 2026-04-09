CIPHER_OBJ += api/cipher_v1.0/unf_cipher.o    \
              api/cipher_v1.0/mpi_cipher.o    \
              api/cipher_v1.0/sys_cipher.o    \
              api/cipher_v1.0/user_osal_lib.o

CIPHER_INS += -I$(CIPHER_SRC_BASE)/api/cipher_v1.0
