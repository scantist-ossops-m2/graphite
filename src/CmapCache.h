/*  GRAPHITE2 LICENSING

    Copyright 2010, SIL International
    All rights reserved.

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation; either version 2.1 of License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should also have received a copy of the GNU Lesser General Public
    License along with this library in the file named "LICENSE".
    If not, write to the Free Software Foundation, Inc., 51 Franklin St.,
    Fifth Floor, Boston, MA 02110-1301, USA or visit their web page on the
    internet at http://www.fsf.org/licenses/lgpl.html.

Alternatively, the contents of this file may be used under the terms of the
Mozilla Public License (http://mozilla.org/MPL) or the GNU General Public
License, as published by the Free Software Foundation, either version 2
of the License or (at your option) any later version.
*/
#pragma once

#include <graphite2/Types.h>

namespace graphite2 {

class CmapCache
{
public:
    CmapCache(const void * cmapTable, size_t length);
    ~CmapCache();
    uint16 lookup(unsigned int unicode) const {
        if ((m_isBmpOnly && unicode > 0xFFFF) || (unicode > 0x10FFFF))
            return 0;
        unsigned int block = (0xFFFFFF & unicode) >> 8;
        if (m_blocks && m_blocks[block])
            return m_blocks[block][unicode & 0xFF];
        return 0;
    };
    CLASS_NEW_DELETE
private:
    bool m_isBmpOnly;
    uint16 ** m_blocks;
};

} // namespace graphite2
