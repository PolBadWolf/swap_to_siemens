/*
 * config.h
 *
 * Created: 08.02.2023 14:49:20
 *  Author: Gumerovmn
 */ 


#ifndef CONFIG_SYS_H_
#define CONFIG_SYS_H_

#define CONF_LCD
#define CONF_LCD_24

#define CONF_KEY4

#define CONF_MENU

// #define CONF_WATHDOG не работает

// #define CONF_RS232

// #define	CONF_SPI_SOFT


#if !defined(CONF_LCD) or !defined(CONF_KEY4)
#undef CONF_MENU
#endif

#endif /* CONFIG_SYS_H_ */