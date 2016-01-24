/*
  EEPROM.h - EEPROM library
  Copyright (c) 2006 David A. Mellis.  All right reserved.

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

 */

#ifndef EEPROM_h
#define EEPROM_h

#include <inttypes.h>
#include <fmc.h>

#define TEMP_DATA_START 0xF000
#define DATA_FLASH_START 0xF200
#define DATA_FLASH_TEST_END 0x11000
#define DATA_FLASH_SIZE 8192
#define max_addr_size 1920

class EEPROMClass //emulated eeprom
{
  public:
  	EEPROMClass();
	
    uint16_t length(){ 
		return max_addr_size*4; 
	}
    uint32_t read(uint16_t addr){
		
		if(addr < max_addr_size)
		{
			begin();
			uint32_t c = FMC_Read(DATA_FLASH_START + (addr*4));
			end();
			return c;
		}
			
		else return 0;
	}
    void write(uint16_t addr, uint32_t data){
		if(addr < max_addr_size)
		{
			if(!findValidPage(addr))
				copyToPage(addr);
			erasePage(addr);
			uint16_t offset = (addr >> 7) << 9;
			uint32_t _start = DATA_FLASH_START + offset;
			begin();
			for(uint16_t idx = 0; idx < 512; idx+=4)
			{
				if((addr*4 - offset) == idx)
					FMC_Write(_start + idx, data);	
				else
					FMC_Write(_start + idx, FMC_Read(TEMP_DATA_START + idx));
			}
			end();
		}			
	}
	void update(uint16_t addr, uint32_t data){
		if(addr < max_addr_size && read(addr) != data)
			write(addr, data);
	}
	
	template< typename T > T get(uint8_t addr, T &t ){    
		uint32_t idx = DATA_FLASH_START + (addr*4);
		uint8_t *ptr = (uint8_t*) &t;
        begin();
		for( int count = sizeof(T) ; count ; --count, idx+=4 )  {
			*ptr++ = FMC_Read(idx);
		}
		end();
        return t;
    }
    template< typename T > const T put(uint8_t addr, const T &t ){
		uint32_t idx = DATA_FLASH_START + (addr*4);
        const uint8_t *ptr = (const uint8_t*) &t;
        begin();
		for( int count = sizeof(T) ; count ; --count, idx+=4 ) {
			FMC_Write(idx, *ptr++ );
		}
		end();
        return t;
    }
	
private:
		
	void begin(){
		SYS_UnlockReg();
		FMC_Open();
	}
	void end(){ 	
		SYS_LockReg();
		FMC_Close();
	} 
	void copyToPage(uint16_t addr){
		addr = (addr*4)/512;
		begin();
		FMC_Erase(TEMP_DATA_START);
		for(uint16_t idx = 0; idx < 512; idx+=4)
		{
			uint32_t c = FMC_Read(DATA_FLASH_START + addr*512 + idx);
			if(c!=0xFFFFFFFF)
				FMC_Write(TEMP_DATA_START + idx, c);
		}
		end();
	}
	bool findValidPage(uint16_t addr){
		uint16_t idx = (addr*4)/512;
		uint8_t count = 128;
		begin();
		for(uint32_t start = DATA_FLASH_START + idx*512; count; --count, start += 4)
		{
			if(FMC_Read(start)!=0xFFFFFFFF)
			{
				end();
				return 0;
			}			
		}
		end();
		return 1;
	}
	void erasePage(uint16_t addr){
		uint16_t idx = (addr*4)/512;
		begin();
		FMC_Erase(DATA_FLASH_START + idx*512);
		end();
	}
	
};

extern EEPROMClass EEPROM;

#endif

