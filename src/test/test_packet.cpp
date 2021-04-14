/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */
#include <catch.hpp>

#include "../stdafx.h"

#include "../network/core/packet.h"

TEST_CASE("Packet")
{
	auto packet = Packet(42);

	CHECK(packet.pos == 0);
	CHECK(packet.size == 3);

	auto* newData = packet.buffer + 1;
	CHECK(*++newData == 42);

	SECTION("Send_bool")
	{
		packet.Send_bool(true);
		packet.Send_bool(false);

		CHECK(packet.size == 5);
		CHECK(*++newData == 1);
		CHECK(*++newData == 0);
	}

	SECTION("Send_uint8")
	{
		packet.Send_uint8(0xAB);
		packet.Send_uint8(0xFF);

		CHECK(packet.size == 5);
		CHECK(*++newData == 0xAB);
		CHECK(*++newData == 0xFF);
	}

	SECTION("Send_uint16")
	{
		packet.Send_uint16(0xABCD);
		packet.Send_uint16(0xF00F);

		CHECK(packet.size == 7);
		CHECK(*++newData == 0xCD);
		CHECK(*++newData == 0xAB);
		CHECK(*++newData == 0x0F);
		CHECK(*++newData == 0xF0);
	}

	SECTION("Send_uint32")
	{
		packet.Send_uint32(0x89ABCDEF);

		CHECK(packet.size == 7);
		CHECK(*++newData == 0xEF);
		CHECK(*++newData == 0xCD);
		CHECK(*++newData == 0xAB);
		CHECK(*++newData == 0x89);
	}

	SECTION("Send_uint64")
	{
		packet.Send_uint64(0x0123456789ABCDEF);

		CHECK(packet.size == 11);
		CHECK(*++newData == 0xEF);
		CHECK(*++newData == 0xCD);
		CHECK(*++newData == 0xAB);
		CHECK(*++newData == 0x89);
		CHECK(*++newData == 0x67);
		CHECK(*++newData == 0x45);
		CHECK(*++newData == 0x23);
		CHECK(*++newData == 0x01);
	}

	SECTION("Send_string")
	{
		packet.Send_string("");

		CHECK(packet.size == 4);
		CHECK(*++newData == '\0');

		packet.Send_string("openttd");

		CHECK(packet.size == 12);
		CHECK(*++newData == 'o');
		CHECK(*++newData == 'p');
		CHECK(*++newData == 'e');
		CHECK(*++newData == 'n');
		CHECK(*++newData == 't');
		CHECK(*++newData == 't');
		CHECK(*++newData == 'd');
		CHECK(*++newData == '\0');

		packet.Send_string("open\0ttd");

		CHECK(packet.size == 17);
		CHECK(*++newData == 'o');
		CHECK(*++newData == 'p');
		CHECK(*++newData == 'e');
		CHECK(*++newData == 'n');
		CHECK(*++newData == '\0');
	}
}
