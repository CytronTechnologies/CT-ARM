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

//#define TEMP_DATA_START 0x1F000
#define DATA_FLASH_START 0x1F000
#define DATA_FLASH_SIZE 4096
#define max_addr_size 1024

class EEPROMClass //emulated eeprom
{
  public:
  	EEPROMClass();
	
    uint16_t length(){ 
		return DATA_FLASH_SIZE; 
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
	//todo: find possible way to make the process faster if user needs only update a few addresses
    void write(uint16_t addr, uint32_t data){
		if(addr < max_addr_size)
		{
			bool isOtherData = false;
			uint32_t _buffer[512];
			if(!findValidPage(addr)){
				//copyToPage(addr);
				copyToPage2(addr, _buffer);
				isOtherData = true;
			}		
			erasePage(addr);
			begin();
			if(!isOtherData){
				FMC_Write(DATA_FLASH_START + addr*4, data);
			}
			else
			{
				uint16_t offset = (addr >> 7) << 9;
				uint32_t _start = DATA_FLASH_START + offset;
				for(uint16_t idx = 0; idx < 512; idx+=4)
				{
					if((addr*4 - offset) == idx)
						FMC_Write(_start + idx, data);	
					else
						//FMC_Write(_start + idx, FMC_Read(TEMP_DATA_START + idx));
						FMC_Write(_start + idx, _buffer[idx]);
				}
			}	
			end();
		}			
	}
	void update(uint16_t addr, uint32_t data){
		if(addr < max_addr_size && read(addr) != data)
			write(addr, data);
	}
	void eraseAddressBlock(uint8_t idx){
		if(idx > 6) return;
		begin();
		FMC_Erase(DATA_FLASH_START + idx*512);
		end();
	}
	void fastWrite(uint16_t addr, uint32_t data){
		begin();
		FMC_Write(DATA_FLASH_START + addr*4, data);
		end();
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
		//uint32_t idx = DATA_FLASH_START + (addr*4);
		uint32_t idx = addr;
        const uint8_t *ptr = (const uint8_t*) &t;
        //begin();
		//for( int count = sizeof(T) ; count ; --count, idx+=4 ) {
		for( int count = sizeof(T) ; count ; --count, idx+=1 ) {
			//FMC_Write(idx, *ptr++ );
			update(idx, *ptr++);
		}
		//end();
        return t;
    }
	template< typename T > const T fastPut(uint8_t addr, const T &t ){
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
	// use rom as temporarily storage
	/*
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
	*/
	// use ram as temporarily storage
	void copyToPage2(uint16_t addr, uint32_t* _buf){
		addr = (addr*4)/512;
		begin();
		for(uint16_t idx = 0; idx < 512; idx+=4)
			_buf[idx] = FMC_Read(DATA_FLASH_START + addr*512 + idx);
		end();
	}
	
	bool findValidPage(uint16_t addr){
		uint16_t idx = (addr*4)/512;
		uint8_t count = 128;
		begin();
		for(uint32_t start = DATA_FLASH_START + idx*512; count; --count, start += 4)
		{
			if(FMC_Read(start)!=0xFFFFFFFF&& (start!=(DATA_FLASH_START + addr*4)))
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

