// BitBang_I2C.ino
#include <Arduino.h>
#include <Wire.h>
TwoWire *pWire = &Wire;



static uint8_t iSDAState = 1;
static uint8_t iSDAState2 = 1;
static uint8_t iSDAState3 = 1;
static uint8_t iSDAState4 = 1;



inline void SCL_HIGH() {
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_8, LL_GPIO_MODE_INPUT);
}

inline void SCL_LOW() {
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_8, LL_GPIO_MODE_OUTPUT);
}

inline void SDA_HIGH() {
    if (iSDAState == 0 && iSDAState2 == 0 && iSDAState3 == 0 && iSDAState4 == 0) {
      
        LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_9, LL_GPIO_MODE_INPUT);
        LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_10, LL_GPIO_MODE_INPUT);
        LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_1, LL_GPIO_MODE_INPUT);
        LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_0, LL_GPIO_MODE_INPUT);
        
        iSDAState = 1;
        iSDAState2 = 1;
        iSDAState3 = 1;
        iSDAState4 = 1;
    }
}

inline void SDA_LOW() {
    if (iSDAState != 0 && iSDAState2 != 0 && iSDAState3 != 0 && iSDAState4 != 0) {
      
        LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_9, LL_GPIO_MODE_OUTPUT);
        LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_10, LL_GPIO_MODE_OUTPUT);
        LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_1, LL_GPIO_MODE_OUTPUT);
        LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_0, LL_GPIO_MODE_OUTPUT);
        
        iSDAState = 0; // eliminate glitches
        iSDAState2 = 0;
        iSDAState3 = 0;
        iSDAState4 = 0;
    }
}

static inline int i2cByteOut(uint8_t b) {
    uint8_t i, ack, ack2, ack3, ack4;
    
    for (i = 0; i < 8; i++) {
        if (b & 0x80) {
            SDA_HIGH();
        } else {
            SDA_LOW();
        }
        SCL_HIGH();
        SCL_LOW();
        b <<= 1;
    }

    SDA_HIGH();
    SCL_HIGH();
    
    ack = LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_9);
    ack2 = LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_10);
    
    SCL_LOW();
    SDA_LOW();

    return (ack == 0 && ack2 == 0) ? 2 : ((ack == 0 || ack2 == 0) ? 1 : 0);
}

static inline void i2cByteIn(uint8_t bLast, uint8_t &b, uint8_t &b2, uint8_t &b3, uint8_t &b4) {
    uint8_t i;

    SDA_HIGH();

    for (i = 0; i < 8; i++) {
        SCL_HIGH();
        b <<= 1;
        b2 <<= 1;
        b3 <<= 1;
        b4 <<= 1;

        b |= LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_9);
        b2 |= LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_10);
        b3 |= LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_1);
        b4 |= LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_0);

        SCL_LOW();
    }

    if (bLast) {
        SDA_HIGH();
    } else {
        SDA_LOW();
    }

    SCL_HIGH();
    SCL_LOW();
    SDA_LOW();
}



static inline void i2cRead_new(uint8_t *pData, uint8_t *pData2, uint8_t *pData3, uint8_t *pData4, int iLen) {
    uint8_t b = 0;
    uint8_t b2 = 0;
    uint8_t b3 = 0;
    uint8_t b4 = 0;

    while (iLen--) {
        i2cByteIn(iLen == 0, b, b2, b3, b4);
        *pData++ = b;
        *pData2++ = b2;
        *pData3++ = b3;
        *pData4++ = b4;
    }
}

static inline void i2cEnd() {
    SDA_LOW();
    SCL_HIGH();
    SDA_HIGH();
}

static inline int i2cBegin(uint8_t addr, uint8_t bRead) {
    int rc = 0;

    SDA_HIGH();
    SCL_HIGH();
    SDA_LOW();
    SCL_LOW();

    addr <<= 1;
    if (bRead) {
        addr++; // set read bit
    }
    rc = i2cByteOut(addr);

    return rc;
}
int I2CRead_new(uint8_t iAddr, uint8_t *pData, uint8_t *pData2, uint8_t *pData3, uint8_t *pData4, int iLen) {
    int rc;

    rc = i2cBegin(iAddr, 1);
    if (rc != 0) {
        i2cRead_new(pData, pData2, pData3, pData4, iLen);
    }
    i2cEnd();
    return rc; // returns 1 for success, 0 for error
}
