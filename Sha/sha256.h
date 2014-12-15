#ifndef Sha256_h
#define Sha256_h

#include <string.h>

#if !defined(__linux) || !defined(linux)
	#include "Arduino.h" //To bring in part.h for Energia
#endif

#if  (defined(__linux) || defined(linux)) || defined(__ARDUINO_X86__) || defined(__TM4C1294NCPDT__)
	#define memcpy_P memcpy
	#undef PROGMEM
	#ifndef __TM4C1294NCPDT__
		#define PROGMEM __attribute__((section(".progmem.data")))
	#else
		#define PROGMEM __attribute__((section(".text")))
	#endif
	#define pgm_read_dword(p) (*(p))
	#if defined(__ARDUINO_X86__) || defined(__TM4C1294NCPDT__)
		#include "Print.h"
	#endif
#else
	#include <avr/io.h>
	#include <avr/pgmspace.h>
	#include "Print.h"
#endif

#include <inttypes.h>

#define HASH_LENGTH 32
#define BLOCK_LENGTH 64

union _buffer
{
	uint8_t b[BLOCK_LENGTH];
	uint32_t w[BLOCK_LENGTH / 4];
};
union _state
{
	uint8_t b[HASH_LENGTH];
	uint32_t w[HASH_LENGTH / 4];
};

#if  (defined(__linux) || defined(linux)) && !defined(__ARDUINO_X86__) && !defined(__TM4C1294NCPDT__)
class Sha256Class
#else
class Sha256Class : public Print
#endif
{
public:
	void init(void);
	void initHmac(const uint8_t* secret, int secretLength);
	uint8_t* result(void);
	uint8_t* resultHmac(void);
#if  (defined(__linux) || defined(linux)) && !defined(__ARDUINO_X86__) && !defined(__TM4C1294NCPDT__)
	virtual size_t write(uint8_t);
	size_t write_L(const char *str);
	size_t write_L(const uint8_t *buffer, size_t size);
	size_t print(const char* str);
#else
	virtual size_t write(uint8_t);
	using Print::write;
#endif
private:
	void pad();
	void addUncounted(uint8_t data);
	void hashBlock();
	uint32_t ror32(uint32_t number, uint8_t bits);
	_buffer buffer;
	uint8_t bufferOffset;
	_state state;
	uint32_t byteCount;
	uint8_t keyBuffer[BLOCK_LENGTH];
	uint8_t innerHash[HASH_LENGTH];
};
extern Sha256Class Sha256;

#endif
