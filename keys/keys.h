#ifndef KEYS_H__
#define KEYS_H__

#include "asf.h"
#include "crypto/avrnacl.h"

#define PAGE_SIZE           32
#define MAX_PAGES           64
#define MAX_BYTES           PAGE_SIZE * MAX_PAGES /* 2048 */
#define MAX_KEY_SLOTS       16

#define PAGE_STATUS         0
#define PAGE_PRIVATE_KEY    1
#define PAGE_PUBLIC_KEY     2
#define PAGE_TABLE          3 /* Page that holds the user key look-up table */
#define PAGE_KEY_START      4

#define FLAGS_UNSET         0xFF // The status register looks like this when it is unconfigured
#define FLAGS_CONFIGURED    0b10000000
#define FLAGS_PRIVATE_KEY 	0b01000000
#define FLAGS_PUBLIC_KEY 	0b00100000
#define FLAGS_NETWORK_ID 	0b00010000
#define FLAGS_PASSWORD  	0b00001000

#define KEYS_BIT_SET(val, mask)     !(val & mask) // provides inverted view of set bits
#define KEYS_SET_BIT(val, mask)     (val & ~mask)
#define KEYS_CLEAR_BIT(val, mask)   (val | mask)

typedef struct {
	uint8_t flags;
	uint8_t network_id;
	uint8_t password[16];
} key_setup_status_t;

uint8_t keys_load_status(void);

const key_setup_status_t * keys_get_status(void);

uint8_t keys_store_status(void);

void keys_set_flag(uint8_t mask);

void keys_unset_flag(uint8_t mask);

typedef struct {
								uint8_t network_id;
								uint8_t key_page;
} key_table_entry_t;

typedef struct {
								key_table_entry_t table[MAX_KEY_SLOTS];
} key_table_t;


/*  Read the key lookup table from EEPROM and store it
    Returns STATUS_OK if all went well
 */
uint8_t keys_load_table(void);

uint8_t keys_store_table(void);

uint8_t keys_reset_table(void);

const key_table_t * keys_get_table(void);

/* Returns true if there is a table entry for the network id */
bool keys_has_key(uint8_t network_id, uint8_t * slot);

uint8_t keys_store_key(uint8_t network_id, uint8_t key[PAGE_SIZE]);

uint8_t keys_read_key(uint8_t slot, uint8_t dest[PAGE_SIZE]);

#endif
