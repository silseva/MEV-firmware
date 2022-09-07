/*
 * MEV board firmware
 * Copyright (C) 2022  Silvano Seva silvano.seva@polimi.it
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

/**
 * Driver for MCU's internal flash management, allowing for sector erase and data
 * writing.
 */

/**
 * Erase one sector of the MCU flash memory.
 *
 * @param secNum: sector number.
 * @return true for successful erase, false otherwise.
 */
bool flash_eraseSector(const uint8_t secNum);

/**
 * Write data to the MCU flash memory.
 *
 * @param address: starting address for the write operation.
 * @param data: data to be written.
 * @param len: data length.
 */
void flash_write(const uint32_t address, const void *data, const size_t len);
