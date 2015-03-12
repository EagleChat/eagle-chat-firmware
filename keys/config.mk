
TARGET = main.elf

CSRCS += \
    main.c \
    common/drivers/nvm/xmega/xmega_nvm.c \
	keys.c

INC_PATH += \
	common/services/delay \
	common/drivers/nvm/xmega \
	xmega/drivers/nvm \
	common/drivers/nvm \

# Extra header paths to include, relative to this directory
USER_INC_PATH += \

USER_LIB_PATH += \
	# add extra static libraries here
