#ifndef __HI_CIPHER_COMPAT_H__
#define __HI_CIPHER_COMPAT_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/* enum typedef */
/* Cipher work mode */
typedef enum
{
    HI_CIPHER_WORK_MODE_ECB     = 0x0,  /* Electronic codebook (ECB) mode, ECB has been considered insecure and it is
                                           recommended not to use it.*/
    HI_CIPHER_WORK_MODE_CBC,            /* Cipher block chaining (CBC) mode*/
    HI_CIPHER_WORK_MODE_CFB,            /* Cipher feedback (CFB) mode*/
    HI_CIPHER_WORK_MODE_OFB,            /* Output feedback (OFB) mode*/
    HI_CIPHER_WORK_MODE_CTR,            /* Counter (CTR) mode*/
    HI_CIPHER_WORK_MODE_CCM,            /* Counter (CCM) mode*/
    HI_CIPHER_WORK_MODE_GCM,            /* Counter (GCM) mode*/
    HI_CIPHER_WORK_MODE_CBC_CTS,        /* Cipher block chaining CipherStealing mode*/
    HI_CIPHER_WORK_MODE_BUTT,
    HI_CIPHER_WORK_MODE_INVALID = 0xffffffff,
}hi_cipher_work_mode;

/* Cipher algorithm */
typedef enum
{
    HI_CIPHER_ALG_DES           = 0x0,  /* Data encryption standard (DES) algorithm,DES has been considered
                                           insecure and it is recommended not to use it. */
    HI_CIPHER_ALG_3DES          = 0x1,  /* 3DES algorithm */
    HI_CIPHER_ALG_AES           = 0x2,  /* Advanced encryption standard (AES) algorithm */
    HI_CIPHER_ALG_SM1           = 0x3,  /* SM1 algorithm*/
    HI_CIPHER_ALG_SM4           = 0x4,  /* SM4 algorithm*/
    HI_CIPHER_ALG_DMA           = 0x5,  /* DMA copy*/
    HI_CIPHER_ALG_BUTT          = 0x6,
    HI_CIPHER_ALG_INVALID       = 0xffffffff,
}hi_cipher_alg;

/* Key length */
typedef enum
{
    HI_CIPHER_KEY_AES_128BIT    = 0x0,  /* 128-bit key for the AES algorithm */
    HI_CIPHER_KEY_AES_192BIT    = 0x1,  /* 192-bit key for the AES algorithm */
    HI_CIPHER_KEY_AES_256BIT    = 0x2,  /* 256-bit key for the AES algorithm */
    HI_CIPHER_KEY_DES_3KEY      = 0x2,  /* Three keys for the DES algorithm */
    HI_CIPHER_KEY_DES_2KEY      = 0x3,  /* Two keys for the DES algorithm */
    HI_CIPHER_KEY_DEFAULT       = 0x0,  /* Default key length, DES-8, SM1-48, SM4-16 */
    HI_CIPHER_KEY_INVALID       = 0xffffffff,
}hi_cipher_key_length;

/* Cipher bit width */
typedef enum
{
    HI_CIPHER_BIT_WIDTH_64BIT   = 0x0,  /* 64-bit width */
    HI_CIPHER_BIT_WIDTH_8BIT    = 0x1,  /* 8-bit width */
    HI_CIPHER_BIT_WIDTH_1BIT    = 0x2,  /* 1-bit width */
    HI_CIPHER_BIT_WIDTH_128BIT  = 0x3,  /* 128-bit width */
    HI_CIPHER_BIT_WIDTH_INVALID = 0xffffffff,
}hi_cipher_bit_width;

/* Key ladder selecting parameters */
typedef enum
{
    HI_CIPHER_KEY_SRC_USER      = 0x0,  /**< User Key*/
    HI_CIPHER_KEY_SRC_KLAD_1,           /**< KLAD Key 1*/
    HI_CIPHER_KEY_SRC_KLAD_2,           /**< KLAD Key 2*/
    HI_CIPHER_KEY_SRC_KLAD_3,           /**< KLAD Key 3*/
    HI_CIPHER_KEY_SRC_BUTT,
    HI_CIPHER_KEY_SRC_INVALID   = 0xffffffff,
}hi_cipher_ca_type;

/** Klad target */
typedef enum {
    HI_CIPHER_KLAD_TARGET_AES   = 0x0,  /**< Klad for AES*/
    HI_CIPHER_KLAD_TARGET_RSA,          /**< Klad for RSA*/
    HI_CIPHER_KLAD_TARGET_BUTT,
} hi_cipher_klad_target;

/* Encryption/Decryption type selecting */
typedef enum
{
    HI_CIPHER_TYPE_NORMAL       = 0x0,
    HI_CIPHER_TYPE_COPY_AVOID,
    HI_CIPHER_TYPE_BUTT,
    HI_CIPHER_TYPE_INVALID      = 0xffffffff,
}hi_cipher_type;

/* SM1 round config */
typedef enum
{
    HI_CIPHER_SM1_ROUND_08      = 0x00, /* SM1 round 08 */
    HI_CIPHER_SM1_ROUND_10      = 0x01, /* SM1 round 10 */
    HI_CIPHER_SM1_ROUND_12      = 0x02, /* SM1 round 12 */
    HI_CIPHER_SM1_ROUND_14      = 0x03, /* SM1 round 14 */
    HI_CIPHER_SM1_ROUND_BUTT,
    HI_CIPHER_SM1_ROUND_INVALID = 0xffffffff,
}hi_cipher_sm1_round;

/* Hash algrithm type */
typedef enum
{
    HI_CIPHER_HASH_TYPE_SHA1,
    HI_CIPHER_HASH_TYPE_SHA224,
    HI_CIPHER_HASH_TYPE_SHA256,
    HI_CIPHER_HASH_TYPE_SHA384,
    HI_CIPHER_HASH_TYPE_SHA512,
    HI_CIPHER_HASH_TYPE_HMAC_SHA1,
    HI_CIPHER_HASH_TYPE_HMAC_SHA224,
    HI_CIPHER_HASH_TYPE_HMAC_SHA256,
    HI_CIPHER_HASH_TYPE_HMAC_SHA384,
    HI_CIPHER_HASH_TYPE_HMAC_SHA512,
    HI_CIPHER_HASH_TYPE_SM3,
    HI_CIPHER_HASH_TYPE_BUTT,
    HI_CIPHER_HASH_TYPE_INVALID = 0xffffffff,
}hi_cipher_hash_type;

/* Rsa encrypt and decrypt scheme */
typedef enum
{
    HI_CIPHER_RSA_ENC_SCHEME_NO_PADDING  = 0x00, /* without padding */
    HI_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_0,       /* PKCS#1 block type 0 padding*/
    HI_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_1,       /* PKCS#1 block type 1padding*/
    HI_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_2,       /* PKCS#1 block type 2 padding*/
    HI_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA1,    /* PKCS#1 RSAES-OAEP-SHA1 padding*/
    HI_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA224,  /* PKCS#1 RSAES-OAEP-SHA224 padding*/
    HI_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA256,  /* PKCS#1 RSAES-OAEP-SHA256   padding*/
    HI_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA384,  /* PKCS#1 RSAES-OAEP-SHA384   padding*/
    HI_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA512,  /* PKCS#1 RSAES-OAEP-SHA512   padding*/
    HI_CIPHER_RSA_ENC_SCHEME_RSAES_PKCS1_V1_5,   /* PKCS#1 RSAES-PKCS1_V1_5    padding*/
    HI_CIPHER_RSA_ENC_SCHEME_BUTT,
    HI_CIPHER_RSA_ENC_SCHEME_INVALID    = 0xffffffff,
}hi_cipher_rsa_enc_scheme;

/* Rsa sign and verify scheme */
typedef enum
{
    HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA1 = 0x100, /* PKCS#1 RSASSA_PKCS1_V15_SHA1 signature*/
    HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA224,       /* PKCS#1 RSASSA_PKCS1_V15_SHA224 signature*/
    HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA256,       /* PKCS#1 RSASSA_PKCS1_V15_SHA256 signature*/
    HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA384,       /* PKCS#1 RSASSA_PKCS1_V15_SHA384 signature*/
    HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA512,       /* PKCS#1 RSASSA_PKCS1_V15_SHA512 signature*/
    HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA1,         /* PKCS#1 RSASSA_PKCS1_PSS_SHA1 signature*/
    HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA224,       /* PKCS#1 RSASSA_PKCS1_PSS_SHA224 signature*/
    HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA256,       /* PKCS#1 RSASSA_PKCS1_PSS_SHA256 signature*/
    HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA384,       /* PKCS#1 RSASSA_PKCS1_PSS_SHA1 signature*/
    HI_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA512,       /* PKCS#1 RSASSA_PKCS1_PSS_SHA256 signature*/
    HI_CIPHER_RSA_SIGN_SCHEME_BUTT,
    HI_CIPHER_RSA_SIGN_SCHEME_INVALID               = 0xffffffff,
}hi_cipher_rsa_sign_scheme;

/* struct define */
/* Cipher control parameters */
typedef struct
{
    hi_u32 bit1_iv     : 2;              /* Initial Vector change flag, 0-don't set, 1-set IV for first package, 2-set
                                            IV for each package  */
    hi_u32 bits_reserve: 30;             /* Reserved */
}hi_cipher_ctrl_change_flag;

/* Structure of the cipher type */
typedef struct
{
    hi_cipher_type cipher_type;
}hi_cipher_atts;

/* Structure of the cipher control information */
typedef struct
{
    hi_u32 key[8];                   /* Key input */
    hi_u32 iv[4];                    /* Initialization vector (IV) */
    hi_bool key_by_ca;               /* Encryption using advanced conditional access (CA) or decryption using keys */
    hi_cipher_ca_type ca_type;       /* Select keyladder type when using advanced CA */
    hi_cipher_alg alg;               /* Cipher algorithm */
    hi_cipher_bit_width bit_width;   /* Bit width for encryption or decryption */
    hi_cipher_work_mode work_mode;   /* Operating mode */
    hi_cipher_key_length key_len;    /* Key length */
    hi_cipher_ctrl_change_flag change_flags; /* control information exchange choices, we default all woulde be change
                                                except they have been in the choices */
} hi_cipher_ctrl;

/* Structure of the cipher AES control information */
typedef struct
{
    hi_u32 even_key[8];                      /* Key input, default use this key*/
    hi_u32 odd_key[8];                       /* Key input, only valid for Multi encrypt/decrypt*/
    hi_u32 iv[4];                            /* Initialization vector (IV) */
    hi_cipher_bit_width bit_width;           /* Bit width for encryption or decryption */
    hi_cipher_key_length key_len;            /* Key length */
    hi_cipher_ctrl_change_flag change_flags; /* control information exchange choices, we default all woulde be change
                                                except they have been in the choices */
} hi_cipher_ctrl_aes;

/* Structure of the cipher AES CCM/GCM control information */
typedef struct
{
    hi_u32 key[8];                       /* Key input */
    hi_u32 iv[4];                        /* Initialization vector (IV) */
    hi_cipher_key_length key_len;        /* Key length */
    hi_u32 iv_len;                       /* IV lenght for CCM/GCM, which is an element of {7, 8, 9, 10, 11, 12, 13}
                                            for CCM, and is an element of [1-16] for GCM*/
    hi_u32 tag_len;                      /* Tag lenght for CCM which is an element of {4,6,8,10,12,14,16}*/
    hi_u32 a_len;                        /* Associated data for CCM and GCM*/
    hi_size_t a_phy_addr;                /* Physical address of Associated data  for CCM and GCM*/
} hi_cipher_ctrl_aes_ccm_gcm;

/* Structure of the cipher DES control information */
typedef struct
{
    hi_u32 key[2];                           /* Key input */
    hi_u32 iv[2];                            /* Initialization vector (IV) */
    hi_cipher_bit_width bit_width;           /* Bit width for encryption or decryption */
    hi_cipher_ctrl_change_flag change_flags; /* Control information exchange choices, we default all woulde be change
                                                except they have been in the choices */
} hi_cipher_ctrl_des;

/* Structure of the cipher 3DES control information */
typedef struct
{
    hi_u32 key[6];
    hi_u32 iv[2];                            /* Initialization vector (IV) */
    hi_cipher_bit_width bit_width;           /* Bit width for encryption or decryption */
    hi_cipher_key_length key_len;            /* Key length */
    hi_cipher_ctrl_change_flag change_flags; /* control information exchange choices, we default all woulde be change
                                                except they have been in the choices */
} hi_cipher_ctrl_3des;

/* Structure of the cipher SM1 control information */
typedef struct
{
    hi_u32 ek[4];                            /* Key of EK input */
    hi_u32 ak[4];                            /* Key of AK input */
    hi_u32 sk[4];                            /* Key of SK input */
    hi_u32 iv[4];                            /* Initialization vector (IV) */
    hi_cipher_bit_width bit_width;           /* Bit width for encryption or decryption */
    hi_cipher_sm1_round sm1_round;           /* SM1 round number, should be 8, 10, 12 or 14*/
    hi_cipher_ctrl_change_flag change_flags; /* control information exchange choices, we default all woulde be change
                                                except they have been in the choices */
} hi_cipher_ctrl_sm1;

/* Structure of the cipher SM4 control information */
typedef struct
{
    hi_u32 key[4];                           /* Key input */
    hi_u32 iv[4];                            /* Initialization vector (IV) */
    hi_cipher_ctrl_change_flag change_flags; /* control information exchange choices, we default all woulde be change
                                                except they have been in the choices */
} hi_cipher_ctrl_sm4;

/* Expand Structure of the cipher control information */
typedef struct
{
    hi_cipher_alg alg;              /* Cipher algorithm */
    hi_cipher_work_mode work_mode;  /* Operating mode */
    hi_bool key_by_ca;              /* Encryption using advanced conditional access (CA) or decryption using keys */
    /*  Parameter for special algorithm
        for AES, the pointer should point to hi_cipher_ctrl_aes;
        for AES_CCM or AES_GCM, the pointer should point to hi_cipher_ctrl_aes_ccm_gcm;
        for DES, the pointer should point to hi_cipher_ctrl_des;
        for 3DES, the pointer should point to hi_cipher_ctrl_3des;
        for SM1, the pointer should point to hi_cipher_ctrl_sm1;
        for SM4, the pointer should point to hi_cipher_ctrl_sm4;
    */
    hi_void *param;
} hi_cipher_ctrl_ex;

/* Cipher data */
typedef struct
{
    hi_size_t src_phy_addr;     /* phy address of the original data */
    hi_size_t dest_phy_addr;    /* phy address of the purpose data */
    hi_u32 byte_length;         /* Cigher data length*/
    hi_bool odd_key;            /* Use odd key or even key*/
} hi_cipher_data;

/* Hash init struct input */
typedef struct
{
    hi_u8 *hmac_key;
    hi_u32 hmac_key_len;
    hi_cipher_hash_type sha_type;
}hi_cipher_hash_atts;

/* RSA public key struct */
typedef struct
{
    hi_u8  *n;        /* Point to public modulus  */
    hi_u8  *e;        /* Point to public exponent */
    hi_u16 n_len;     /* Length of public modulus, max value is 512Byte*/
    hi_u16 e_len;     /* Length of public exponent, max value is 512Byte*/
}hi_cipher_rsa_pub_key;

/* RSA private key struct */
typedef struct
{
    hi_u8 *n;         /* Public modulus    */
    hi_u8 *e;         /* Public exponent   */
    hi_u8 *d;         /* Private exponent  */
    hi_u8 *p;         /* 1st prime factor  */
    hi_u8 *q;         /* 2nd prime factor  */
    hi_u8 *dp;        /* d % (p - 1) */
    hi_u8 *dq;        /* d % (q - 1) */
    hi_u8 *qp;        /* 1 / (q % p) */
    hi_u16 n_len;     /* Length of public modulus */
    hi_u16 e_len;     /* Length of public exponent */
    hi_u16 d_len;     /* Length of private exponent */
    hi_u16 p_len;     /* Length of 1st prime factor,should be half of u16NLen */
    hi_u16 q_len;     /* Length of 2nd prime factor,should be half of u16NLen */
    hi_u16 dp_len;    /* Length of D % (P - 1),should be half of u16NLen */
    hi_u16 dq_len;    /* Length of D % (Q - 1),should be half of u16NLen */
    hi_u16 qp_len;    /* Length of 1 / (Q % P),should be half of u16NLen */
}hi_cipher_rsa_pri_key;

/* RSA public key encryption struct input */
typedef struct
{
    hi_cipher_rsa_enc_scheme scheme;    /* RSA encryption scheme */
    hi_cipher_rsa_pub_key pub_key;      /* RSA private key struct */
    hi_cipher_ca_type ca_type;          /* CA type*/
}hi_cipher_rsa_pub_enc;

/* RSA private key decryption struct input */
typedef struct
{
    hi_cipher_rsa_enc_scheme scheme;    /* RSA encryption scheme */
    hi_cipher_rsa_pri_key pri_key;      /* RSA private key struct */
    hi_cipher_ca_type ca_type;          /* CA type*/
}hi_cipher_rsa_pri_enc;

/* RSA signature struct input */
typedef struct
{
    hi_cipher_rsa_sign_scheme scheme;   /* RSA signature scheme*/
    hi_cipher_rsa_pri_key pri_key;      /* RSA private key struct */
    hi_cipher_ca_type ca_type;          /* CA type*/
 }hi_cipher_rsa_sign;

/* RSA signature verify struct input */
typedef struct
{
    hi_cipher_rsa_sign_scheme scheme;   /* RSA signature scheme*/
    hi_cipher_rsa_pub_key pub_key;      /* RSA public key struct */
 }hi_cipher_rsa_verify;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_CIPHER_COMPAT_H__ */
