////////////////////////////////////////////////////////////////////////////////
//	Copyright 2015 by Joseph Forgione
//	This file is part of VCC (Virtual Color Computer).
//	
//	VCC (Virtual Color Computer) is free software: you can redistribute itand/or
//	modify it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or (at your
//	option) any later version.
//	
//	VCC (Virtual Color Computer) is distributed in the hope that it will be
//	useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
//	Public License for more details.
//	
//	You should have received a copy of the GNU General Public License along with
//	VCC (Virtual Color Computer). If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////
#include "cartridge_slot.h"
#include <vcc/bus/cpak_cartridge.h>
#include <vcc/bus/null_cartridge.h>


namespace VCC::Core
{

	cartridge_slot::cartridge_slot()
		: cartridge_(std::make_unique<::VCC::Core::null_cartridge>())
	{}

	cartridge_slot::cartridge_slot(path_type path, handle_type handle, cartridge_ptr_type cartridge)
		:
		path_(move(path)),
		handle_(move(handle)),
		cartridge_(move(cartridge))
	{}

	cartridge_slot& cartridge_slot::operator=(cartridge_slot&& other) noexcept
	{
		if (this != &other)
		{
			//	Destroy the cartrige first before the handle is released
			//	in case this is the last reference to the shared library
			cartridge_.reset();
			handle_.reset();

			path_ = move(other.path_);
			handle_ = move(other.handle_);
			cartridge_ = move(other.cartridge_);
			line_state_ = other.line_state_;

			other.line_state_ = false;
		}

		return *this;
	}


	cartridge_slot::label_type cartridge_slot::label() const
	{
		std::string text;

		text += cartridge_->name();
		text += "  ";
		text += cartridge_->catalog_id();

		return text;
	}

	void cartridge_slot::set_window_handle(HWND hVccWnd) const
	{
		if (auto* cpak = dynamic_cast<::VCC::Core::cpak_cartridge*>(cartridge_.get()))
			cpak->set_window_handle(hVccWnd);
	}

	void cartridge_slot::cpu_memory_write(unsigned char value, unsigned short address) const
	{
		if (auto* cpak = dynamic_cast<::VCC::Core::cpak_cartridge*>(cartridge_.get()))
			cpak->cpu_memory_write(value, address);
	}

	bool cartridge_slot::cpu_memory_read(unsigned short address, unsigned char* value) const
	{
		if (auto* cpak = dynamic_cast<::VCC::Core::cpak_cartridge*>(cartridge_.get()))
			return cpak->cpu_memory_read(address, value);
		return false;
	}

}
