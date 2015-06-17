#include "RFM12B.h"
#include "avr/io.h"
#include "string.h"


void SPIInit() {
  SS_DDR |= (1 << cs_pin);
  DDRB |= (1 << 0);
  DDRB |= (1 << 2);
  DDRB |= (1 << 3);
  DDRB |= (1 << 1);
  SPCR = _BV(SPE) | _BV(MSTR);
  SPSR |= _BV(SPI2X);
  DDRD &= ~(1 << 3);
  PORTD |= (1 << 3);
}

uint8_t Byte(uint8_t out) {
  SPDR = out;
  while (!(SPSR & _BV(SPIF)));
  return SPDR;
}

uint16_t XFERSlow(uint16_t cmd) {

  SPCR |= (1 << SPR0);
  SS_PORT &= ~(1 << cs_pin);
  uint16_t reply = Byte(cmd >> 8) << 8;
  reply |= Byte(cmd);
  SS_PORT |= (1 << cs_pin);
  SPCR &= ~(1 << SPR0);
  return reply;
}

void XFER(uint16_t cmd) {
  SS_PORT &= ~(1 << cs_pin);
  Byte(cmd >> 8) << 8;
  Byte(cmd);
  SS_PORT |= (1 << cs_pin);
}

void Initialize(uint8_t ID, uint8_t networkid)
{


  Data = rf12_data;
  DataLen = &rf12_buf[3];

  uint8_t txPower = 0;
  uint8_t airKbps = 0x08;
  uint8_t lowVoltageThreshold = RF12_2v75;


  cs_pin = SS_BIT;
  nodeID = ID;
  networkID = networkid;
  SPIInit();
  XFER(0x0000); // intitial SPI transfer added to avoid power-up problem
  XFER(RF_SLEEP_MODE); // DC (disable clk pin), enable lbd

  // wait until RFM12B is out of power-up reset, this takes several *seconds*
  XFER(RF_TXREG_WRITE); // in case we're still in OOK mode
  while (!(PIND & (1 << 3) )) //             digitalRead(18) == 0)
    XFER(0x0000);

  XFER(0x80C7 | (RF12_868MHZ << 4)); // EL (ena TX), EF (ena RX FIFO), 12.0pF
  XFER(0xA640); // Frequency is exactly 434/868/915MHz (whatever freqBand is)
  XFER(0xC600 + airKbps);   //Air transmission baud rate: 0x08= ~38.31Kbps
  XFER(0x94A2);             // VDI,FAST,134kHz,0dBm,-91dBm
  XFER(0xC2AC);             // AL,!ml,DIG,DQD4
  if (networkID != 0) {
    XFER(0xCA83);           // FIFO8,2-SYNC,!ff,DR
    XFER(0xCE00 | networkID); // SYNC=2DXX;
  } else {
    XFER(0xCA8B); // FIFO8,1-SYNC,!ff,DR
    XFER(0xCE2D); // SYNC=2D;
  }


  XFER(0xC483); // @PWR,NO RSTRIC,!st,!fi,OE,EN
  XFER(0x9850 | (txPower > 7 ? 7 : txPower)); // !mp,90kHz,MAX OUT               //last byte=power level: 0=highest, 7=lowest
  XFER(0xCC77); // OB1,OB0, LPX,!ddy,DDIT,BW0
  XFER(0xE000); // NOT USE
  XFER(0xC800); // NOT USE
  XFER(0xC043); // Clock output (1.66MHz), Low Voltage threshold (2.55V)

  rxstate = TXIDLE;


  EICRA |= (1 << ISC31);
  EICRA &= ~(1 << ISC30);
  EIMSK |= (1 << INT3);

  sei();

}


// access to the RFM12B internal registers with interrupts disabled
uint16_t Control(uint16_t cmd) {

  EIMSK &= ~(1 << INT0);
  uint16_t r = XFERSlow(cmd);
  EIMSK |= (1 << INT0);
  return r;
}



void InterruptHandler() {
  XFER(0x0000);

  if (rxstate == TXRECV) {
    uint8_t in = XFERSlow(RF_RX_FIFO_READ);

    if (rxfill == 0 && networkID != 0)
      rf12_buf[rxfill++] = networkID;

    rf12_buf[rxfill++] = in;
    rf12_crc = _crc16_update(rf12_crc, in);

    if (rxfill >= rf12_len + 6 || rxfill >= RF_MAX)
      XFER(RF_IDLE_MODE);
  } else {
    uint8_t out;

    if (rxstate < 0) {
      uint8_t pos = 4 + rf12_len + rxstate++;
      out = rf12_buf[pos];
      rf12_crc = _crc16_update(rf12_crc, out);
    } else
      switch (rxstate++) {
      case TXSYN1: out = 0x2D; break;
      case TXSYN2: out = networkID; rxstate = -(3 + rf12_len); break;
      case TXCRC1: out = rf12_crc; break;
      case TXCRC2: out = rf12_crc >> 8; break;
      case TXDONE: XFER(RF_IDLE_MODE);
      default:     out = 0xAA;
      }

    XFER(RF_TXREG_WRITE + out);
  }
}

ISR(INT3_vect) {

  InterruptHandler();
}


void ReceiveStart() {
  rxfill = rf12_len = 0;
  rf12_crc = ~0;
  if (networkID != 0)
    rf12_crc = _crc16_update(~0, networkID);
  rxstate = TXRECV;
  XFER(RF_RECEIVER_ON);
}

char ReceiveComplete() {
  if (rxstate == TXRECV && (rxfill >= rf12_len + 6 || rxfill >= RF_MAX)) {
    rxstate = TXIDLE;
    if (rf12_len > RF12_MAXDATA)
      rf12_crc = 1;
    if (RF12_DESTID == 0 || RF12_DESTID == nodeID) {
      if (!rf12_crc)
        rf12_seq = -1;
      return 1;
    }
  }
  if (rxstate == TXIDLE)
    ReceiveStart();
  return 0;
}

char CanSend() {

  if (rxstate == TXRECV && rxfill == 0 && (Byte(0x00) & (RF_RSSI_BIT >> 8)) == 0) {
    XFER(RF_IDLE_MODE);
    rxstate = TXIDLE;
    return 1;
  }
  return 0;
}

void SendStart(uint8_t toNodeID, char requestACK, char sendACK) {
  rf12_hdr1 = toNodeID | (sendACK ? RF12_HDR_ACKCTLMASK : 0);
  rf12_hdr2 = nodeID | (requestACK ? RF12_HDR_ACKCTLMASK : 0);
  rf12_crc = ~0;
  rf12_crc = _crc16_update(rf12_crc, networkID);
  rxstate = TXPRE1;
  XFER(RF_XMITTER_ON);
}

void SendStart2(uint8_t toNodeID, const void* sendBuf, uint8_t sendLen, char requestACK, char sendACK, uint8_t waitMode) {
  rf12_len = sendLen;
  memcpy((void*) rf12_data, sendBuf, sendLen);



  SendStart(toNodeID, requestACK, sendACK);

  SendWait(waitMode);
}


void SendACK2(const void* sendBuf, uint8_t sendLen, uint8_t waitMode) {
  while (!CanSend()) ReceiveComplete();
  SendStart2(RF12_SOURCEID, sendBuf, sendLen, 0, 1, waitMode);
}



void SendACK() {
  const void* sendBuf = "";
  uint8_t sendLen = 0;
  uint8_t waitMode = SLEEP_MODE_IDLE;
  while (!CanSend()) ReceiveComplete();
  SendStart2(RF12_SOURCEID, sendBuf, sendLen, 0, 1, waitMode);
}



void Send(uint8_t toNodeID, const void* sendBuf, uint8_t sendLen, char requestACK)
{
  uint8_t waitMode = SLEEP_MODE_STANDBY;
  while (!CanSend()) ReceiveComplete();
  SendStart2(toNodeID, sendBuf, sendLen, requestACK, 0, waitMode);
}

void SendWait(uint8_t waitMode) {
  while (rxstate != TXIDLE);
}

void OnOff(uint8_t value) {
  XFER(value ? RF_XMITTER_ON : RF_IDLE_MODE);
}

void Sleep2(char n) {
  if (n < 0)
    Control(RF_IDLE_MODE);
  else {
    Control(RF_WAKEUP_TIMER | 0x0500 | n);
    Control(RF_SLEEP_MODE);
    if (n > 0)
      Control(RF_WAKEUP_MODE);
  }
  rxstate = TXIDLE;
}
void Sleep() { Sleep2(0); }
void Wakeup() { Sleep2(-1); }

char LowBattery() {
  return (Control(0x0000) & RF_LBD_BIT) != 0;
}

uint8_t GetSender() {
  return RF12_SOURCEID;
}

uint8_t * GetData() { return rf12_data; }


uint8_t GetDataLen() { return *DataLen; }
char ACKRequested() { return RF12_WANTS_ACK; }

char ACKReceived(uint8_t fromNodeID) {
  if (ReceiveComplete())
    return CRCPass() &&
           RF12_DESTID == nodeID &&
           (RF12_SOURCEID == fromNodeID || fromNodeID == 0) &&
           (rf12_hdr1 & RF12_HDR_ACKCTLMASK) &&
           !(rf12_hdr2 & RF12_HDR_ACKCTLMASK);
  return 0;
}

char CRCPass() {
  return rf12_crc == 0;
}

