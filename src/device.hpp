// SPDX-License-Identifier: Apache-2.0
/*
 * Copyright (C) 2019 Gwenhael Goavec-Merou <gwenhael.goavec-merou@trabucayre.com>
 */

#ifndef SRC_DEVICE_HPP_
#define SRC_DEVICE_HPP_

#include <cstdint>
#include <iostream>
#include <string>

#include "display.hpp"
#include "jtag.hpp"

/* GGM: TODO: program must have an optional
 * offset
 * and question: bitstream to load bitstream in SPI mode must
 * be hardcoded or provided by user?
 */
class Device {
	public:
		enum prog_mode {
			NONE_MODE = 0,
			SPI_MODE = 1,
			FLASH_MODE = 1,
			MEM_MODE = 2,
			READ_MODE = 3,
		};

		typedef enum {
			WR_SRAM = 0,
			WR_FLASH = 1,
			RD_FLASH = 2,
			PRG_NONE = 3
		} prog_type_t;

		Device(Jtag *jtag, std::string filename, const std::string &file_type,
				bool verify, int8_t verbose = false);
		virtual ~Device();
		virtual void program(unsigned int offset,
				bool unprotect_flash) = 0;

		virtual bool read_register(const std::string /*reg_name*/) { return false;}

		/**********************/
		/*    flash access    */
		/**********************/
		virtual bool detect_flash() {
			printError("detect flash not supported"); return false;}
		virtual bool dumpFlash(uint32_t base_addr, uint32_t len) {
			(void) base_addr; (void) len;
			printError("dump flash not supported"); return false;}
		virtual bool protect_flash(uint32_t len) = 0;
		virtual bool unprotect_flash() = 0;
		virtual bool set_quad_bit(bool set_quad) {
			(void)set_quad;
			printError("Error: SPI Flash Quad mode configuration unsupported"); return false;}
		virtual bool bulk_erase_flash() = 0;

		virtual uint32_t idCode() = 0;
		virtual void reset();

		virtual bool connectJtagToMCU() {return false;}

	protected:
		Jtag *_jtag;
		std::string _filename;
		std::string _file_extension;
		enum prog_mode _mode;
		bool _verify; /**< verify flash write */
		bool _verbose;
		int8_t _verbose_level;
		bool _quiet;
};

#endif  // SRC_DEVICE_HPP_
