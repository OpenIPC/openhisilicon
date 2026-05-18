/*+++ *******************************************************************\
*
*  Copyright and Disclaimer:
*
*     ---------------------------------------------------------------
*     This software is provided "AS IS" without warranty of any kind,
*     either expressed or implied, including but not limited to the
*     implied warranties of noninfringement, merchantability and/or
*     fitness for a particular purpose.
*     ---------------------------------------------------------------
*
*     Copyright (c) 2012 Conexant Systems, Inc.
*     All rights reserved.
*
\******************************************************************* ---*/

#include "Comm.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */





/*******************************************************************************************************
*
* writeByte
* write a byte to a 16bit register address
*
********************************************************************************************************/
int writeByte(const CX_COMMUNICATION* p_comm,
              const unsigned long registerAddress,
              const unsigned char value)
{
    int ret_val = p_comm->write(p_comm->i2c_addr, registerAddress, value, sizeof(value));
    return ret_val;
}

/*******************************************************************************************************
*
* readByte
* read a byte from 16bit register address
*
*******************************************************************************************************/
int readByte(const CX_COMMUNICATION *p_comm,
              const unsigned long registerAddress,
              unsigned char *p_value)
{
    int ret_val = p_comm->read(p_comm->i2c_addr, registerAddress, p_value, 1);
    return ret_val;

}

/*******************************************************************************************************
*
* writeDword
* write an unsigned long value to a 16 bit register
*
*******************************************************************************************************/
int writeDword(const CX_COMMUNICATION *p_comm,
                const unsigned long registerAddress,
                const unsigned long value)
{
    int ret_val = p_comm->write(p_comm->i2c_addr, registerAddress, value, sizeof(value));
    return ret_val;
}

/*******************************************************************************************************
*
* readDword
* read an unsigned long value from a 16 bit register
*
*******************************************************************************************************/
int readDword(const CX_COMMUNICATION *p_comm,
               const unsigned long registerAddress,
               unsigned long *p_value)
{
    int ret_val = p_comm->read(p_comm->i2c_addr, registerAddress, p_value, 4);
    return ret_val;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
