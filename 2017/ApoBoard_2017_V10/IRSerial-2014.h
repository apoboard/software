/*
  IRSerial-2014 (formerly IRSerial.h (formerly NewSoftSerial.h)) -
  Multi-instance software serial library for Arduino/Wiring
  -- Adapted for simple IR send/receive by Mark Smith (mark@halibut.com)
   - Allowed the inversion of TX and/or RX pins so idle state could
     be low for driving IR LEDs.
   - TX now modulates pin at 38kHz instead of just turning it on.
  -- Interrupt-driven receive and other improvements by ladyada
   (http://ladyada.net)
  -- Tuning, circular buffer, derivation from class Print/Stream,
   multi-instance support, porting to 8MHz processors,
   various optimizations, PROGMEM delay tables, inverse logic and
   direct port writing by Mikal Hart (http://www.arduiniana.org)
  -- Pin change interrupt macros by Paul Stoffregen (http://www.pjrc.com)
  -- 20MHz processor support by Garrett Mace (http://www.macetech.com)
  -- ATmega1280/2560 support by Brett Hagman (http://www.roguerobotics.com/)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  The latest version of this library can always be found at
  http://arduiniana.org.
*/

#ifndef IRSerial_h
#define IRSerial_h

#include <inttypes.h>
#include <Stream.h>

/******************************************************************************
  Definitions
******************************************************************************/

#define _SS_MAX_RX_BUFF 64 // RX buffer size
#ifndef GCC_VERSION
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif

class IRSerial : public Stream {
  private:
    // per object data
    uint8_t _receivePin;
    uint8_t _receiveBitMask;
    volatile uint8_t *_receivePortRegister;
    uint8_t _transmitBitMask;
    volatile uint8_t *_transmitPortRegister;

    uint16_t _rx_delay_centering;
    uint16_t _rx_delay_intrabit;
    uint16_t _rx_delay_stopbit;
    uint16_t _tx_delay;
    uint16_t _modulation_frequency;

    uint16_t _RX_buffer_overflow: 1;
    uint16_t _TX_buffer_overflow: 1;
    uint16_t _inverse_logic_rx: 1;
    uint16_t _inverse_logic_tx: 1;


    // static data
    static char _receive_buffer[_SS_MAX_RX_BUFF];
    static volatile uint8_t _receive_buffer_tail;
    static volatile uint8_t _receive_buffer_head;
    static char _transmit_buffer[_SS_MAX_RX_BUFF];
    static volatile uint8_t _transmit_buffer_tail;
    static volatile uint8_t _transmit_buffer_head;
    static IRSerial *active_object;

    // private methods
    void recv_SPECTER();
    void recv();
    // uint8_t rx_pin_read();
    void tx_pin_write(uint8_t pin_state);
    void tx_pin_write_and_delay(uint8_t pin_state, uint16_t delay);
    void setTX(uint8_t transmitPin);
    void setRX(uint8_t receivePin);

    // private static method for timing
    static inline void tunedDelay(uint16_t delay);

  public:
    volatile uint8_t neo_global_delay;
    bool rxdatavalid;
    uint8_t rxdata;
    // public methods
    IRSerial(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic_rx = false, bool inverse_logic_tx = false, uint16_t modulation_frequency = 0);
    ~IRSerial();
    void begin(long speed);
    bool listen();
    void end();
    bool isListening() {
      return this == active_object;
    }
    bool overflow() {
      bool ret = _RX_buffer_overflow;
      _RX_buffer_overflow = false;
      return ret;
    }
    int peek();
    uint8_t rx_pin_read();

    //virtual size_t write(uint8_t byte);
    virtual size_t write_SPECTER(uint8_t byte);
    virtual int read();
    virtual size_t write(uint8_t byte);
    virtual int available();
    virtual void flush();

    using Print::write;

    // public only for easy access by interrupt handlers
    static inline void handle_interrupt();
};

// Arduino 0012 workaround
#undef int
#undef char
#undef long
#undef byte
#undef float
#undef abs
#undef round

#endif
