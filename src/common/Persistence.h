/*
 * MEV board firmware
 * Copyright (C) 2021 - 2024  Silvano Seva silvano.seva@polimi.it
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

#include <cstdint>
#include <common/PidRegulator.h>

/**
 * Load data saved into the internal flash memory.
 *
 * @param data: pointer to data destination.
 * @param size: data size in bytes.
 * @return true on success, false in case of corrupted data or non-initialized
 * flash storage.
 */
bool loadDataFromFlash(void *data, const size_t size);

/**
 * Save data to the internal flash memory.
 *
 * @param data: pointer to data destination.
 * @param size: data size in bytes.
 */
void saveDataToFlash(void *data, const size_t size);
