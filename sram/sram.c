#include "sram.h"
//#include <user_board.h>
#include "asf.h"
#include "cdc.h"
#include <string.h>


struct spi_device spi_device_conf = 
{
          .id = SRAM_CS_pin
};



void sram_init(void)
{
	
	ioport_set_pin_dir(SRAM_CS_pin, IOPORT_DIR_OUTPUT);  //CS
	ioport_set_pin_dir(SRAM_MOSI_pin, IOPORT_DIR_OUTPUT); //SI
	ioport_set_pin_dir(SRAM_MISO_pin, IOPORT_DIR_INPUT); //SO
	ioport_set_pin_dir(SRAM_SCK_pin, IOPORT_DIR_OUTPUT);//SCK
	ioport_set_pin_dir(SRAM_HOLD_pin, IOPORT_DIR_OUTPUT);
	
        ioport_set_pin_level(SRAM_CS_pin, IOPORT_INIT_HIGH); 
	ioport_set_pin_level(SRAM_HOLD_pin,IOPORT_INIT_HIGH);
}


void spi_init_module(void)
{
      
     

      spi_master_init(&SPIC);
      spi_master_setup_device(&SPIC, &spi_device_conf, SPI_MODE_0, 1000000, 0);
      spi_enable(&SPIC);
}



void write_data(uint8_t data, uint16_t address)
{
	
	uint8_t instruction_w =  1;/*WRSR instruction to allow setting of 
                                         operation mode*/
	
	uint8_t mode_w = 64;//Puts chip in sequential write operation mode. 
                            //E.g. all bytes written not just one


	spi_select_device(&SPIC, &spi_device_conf);/*WRSR Sequence Begin*/
	                                          /*Per page 12 of datasheet*/
                                                  /* bringing CS Low since using 
                                                  select_device function brings
                                                  device low and you must bring 
                                                  device low prior to write       
                                                  attempt*/
	
	spi_write_packet(&SPIC, &instruction_w, 1);/*Sends WRSR instruction*/
	
	spi_write_packet(&SPIC, &mode_w, 1); /*Sends command to put SRAM in 
					      sequential write 
                                              mode.Tells it to send 1 byte as last 
                                              argument and also & so that it points 
	                                      to address of value in status*/

	spi_deselect_device(&SPIC, &spi_device_conf);/*End WRSR sequence*/
						     /*Bring CS High to end writing*/
                                                     /*Ends per page 12 of datasheet*/
	


	uint8_t writecommand = 2;
        uint8_t LSB = (uint8_t) address;//stores LSB of address in variable
	uint8_t MSB = uint8_t (address>>8);//stores MSB of address in variable
	uint8_t address_var[2]; //array of 2 8 bit values to be used to send 16 bits to
                                //chip for addressing read/write purposes 

	address_var[0] = MSB;
        address_var[1] = LSB;
	
	spi_select_device(&SPIC, &spi_device_conf);/*Begin sequential Write Sequence*/
	                                          /*Per page 10 of datasheet*/
                                                  /*bringing CS Low since using 
                                                  select_device function brings
                                                  device low and you must bring 
                                                  device low prior to write       
                                                  attempt*/
	
	spi_write_packet(&SPIC, &writecommand, 1);//Sends command to write 
	
	spi_write_packet(&SPIC, address_var, 2);/*Sends the 2 byte address to 
	                                         SRAM for writing*/
        
 

	spi_write_packet(&SPIC, data, 1); //writes message to SRAM. Each char 
                                                    //is a byte
	
	spi_deselect_device(&SPIC, &spi_device_conf);/*End Byte Write Sequence*/
						     /*Per page 7 of datasheet*/
                                                     /*Bring CS High to end writing*/

}



uint8_t read_data(uint16_t address)
{
        uint8_t instruction_r =  5;/*RDSR instruction to allow setting of 
                                         operation mode*/
	
	uint8_t mode_r = 64;//Puts chip in sequential read operation mode


        spi_select_device(&SPIC, &spi_device_conf);/*RDSR Sequence Begin*/
	                                          /*Per page 11 of datasheet*/
	                                           /*bringing CS Low since using 
                                                   select_device function brings 
                                                   device low*/
	
 	

	spi_write_packet(&SPIC, &instruction_r, 1);/*Sends RDSR instruction*/	
	
	
	spi_read_packet(&SPIC, &mode_r, 1); /*Reads command to put SRAM in 
                                              sequential read 
                                              mode.Tells it to send 1 byte as last 
                                              argument and also & so that it points 
	                                      to address of value in status*/

        spi_deselect_device(&SPIC, &spi_device_conf);/*End RDSR sequence*/
						     /*Bring CS High to end writing*/
                                                     /*Ends per page 11 of datasheet*/
	

	uint8_t readcommand = 3;
	uint8_t LSB = (uint8_t) address;//stores LSB of address in variable
	uint8_t MSB = uint8_t (address>>8);//stores MSB of address in variable
	uint8_t address_var[2]; //array of 2 8 bit values to be used to send 16 bits to
                                //chip for addressing read/write purposes
        address_var[0] = MSB;
        address_var[1] = LSB;
	
	spi_select_device(&SPIC, &spi_device_conf);/*Begin Sequential Read Sequence*/
	                                          /*Per page 7 of datasheet*/

	spi_write_packet(&SPIC, &readcommand, 1);//Sends command to read 

	spi_write_packet(&SPIC, address_var, 2);/*Sends the 2 byte address to SRAM 
                                                 for reading*/                  
        uint8_t buf;//variable declared of 8 bits
			
        spi_read_packet(&SPIC, buf, 1);//write read data to allocated variable

        spi_deselect_device(&SPIC, &spi_device_conf);/*End Sequential Read Sequence*/
						     /*Per page 9 of datasheet*/
                                                     /*Bring CS High to end writing*/

        return buf;
}


//make it so data at written address can be utilized or sent t oa variable for latr use


