#ifndef RFM12B_h
#define RFM12B_h

#include <inttypes.h>
#include <avr/io.h>
#include <util/crc16.h>
#include <avr/eeprom.h>
#include <avr/sleep.h>

#include <avr/interrupt.h>



#define OPTIMIZE_SPI       1 



#define RF12_MAXDATA    128
#define RF_MAX          (RF12_MAXDATA + 6)

#define RF12_315MHZ     0
#define RF12_433MHZ     1
#define RF12_868MHZ     2
#define RF12_915MHZ     3

#define RF12_2v25       0
#define RF12_2v55       3
#define RF12_2v65       4
#define RF12_2v75       5
#define RF12_3v05       8
#define RF12_3v15       9
#define RF12_3v25       10

#define RF12_HDR_IDMASK      0x7F
#define RF12_HDR_ACKCTLMASK  0x80
#define RF12_DESTID   (rf12_hdr1 & RF12_HDR_IDMASK)
#define RF12_SOURCEID (rf12_hdr2 & RF12_HDR_IDMASK)

#define RF12_WANTS_ACK ((rf12_hdr2 & RF12_HDR_ACKCTLMASK) && !(rf12_hdr1 & RF12_HDR_ACKCTLMASK))

#define RF12_SLEEP   0
#define RF12_WAKEUP -1

#define rf12_grp        rf12_buf[0]
#define rf12_hdr1        rf12_buf[1]
#define rf12_hdr2        rf12_buf[2]

#define rf12_len        rf12_buf[3]
#define rf12_data       (rf12_buf + 4)

#define bitSet_(x,y)  (x) |= ((1<<y))

#define RFM_IRQ     2
#define SS_DDR      DDRD
#define SS_PORT     PORTD
#define SS_BIT      2
#define SPI_SS      53 
#define SPI_MOSI    51
#define SPI_MISO    50
#define SPI_SCK     52


#define RF_RECEIVER_ON  0x82DD
#define RF_XMITTER_ON   0x823D
#define RF_IDLE_MODE    0x820D
#define RF_SLEEP_MODE   0x8205
#define RF_WAKEUP_MODE  0x8207
#define RF_TXREG_WRITE  0xB800
#define RF_RX_FIFO_READ 0xB000
#define RF_WAKEUP_TIMER 0xE000

#define RF_LBD_BIT      0x0400
#define RF_RSSI_BIT     0x0100


enum {
  TXCRC1, TXCRC2, TXTAIL, TXDONE, TXIDLE,
  TXRECV,
  TXPRE1, TXPRE2, TXPRE3, TXSYN1, TXSYN2
};

uint8_t rf12_buf[RF_MAX];
uint8_t rxfill;
volatile int8_t  rxstate;
uint16_t rf12_crc;
uint32_t seqNum;
uint32_t cryptKey[4];
long rf12_seq;
uint8_t cs_pin;
uint8_t Byte(uint8_t out);
uint16_t XFERSlow(uint16_t cmd);
void XFER(uint16_t cmd);
void SPIInit(void);
uint8_t networkID;
uint8_t nodeID;
uint8_t* Data;
uint8_t* DataLen;
void InterruptHandler(void);
void Initialize(uint8_t nodeid, uint8_t groupid);
void ReceiveStart(void);
char ReceiveComplete(void);
char CanSend(void);
uint16_t Control(uint16_t cmd);
void SendStart(uint8_t toNodeId, char requestACK, char sendACK);
void SendStart2(uint8_t toNodeId, const void* sendBuf, uint8_t sendLen, char requestACK, char sendACK, uint8_t waitMode);
void SendACK2(const void* sendBuf, uint8_t sendLen, uint8_t waitMode);
void SendACK(void);
void Send(uint8_t toNodeId, const void* sendBuf, uint8_t sendLen, char requestACK);
void SendWait(uint8_t waitMode);
void OnOff(uint8_t value);
void Sleep2(char n);
void Sleep(void);
void Wakeup(void);
uint8_t * GetData(void);
uint8_t GetDataLen(void);
uint8_t GetSender(void);
char LowBattery(void);
char ACKRequested(void);
char ACKReceived(uint8_t fromNodeID);

char CRCPass(void);




#endif
