// This file is part of the ustl library, an STL implementation.
// Copyright (C) 2003 by Mike Sharov <msharov@talentg.com>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the 
// Free Software Foundation, Inc., 59 Temple Place - Suite 330, 
// Boston, MA  02111-1307  USA.
//
// sostream.h
//

#ifndef SOSTREAM_H
#define SOSTREAM_H

#include "mostream.h"
#include "uios.h"

namespace ustl {

class string;
/// This stream writes textual data into a memory block.
class ostringstream : public ostream {
public:
    				ostringstream (void);
				ostringstream (void* p, size_t n);
    explicit			ostringstream (const memlink& source);
    inline ostringstream&	operator<< (char v);
    inline ostringstream&	operator<< (signed char v);
    inline ostringstream&	operator<< (short v);
    inline ostringstream&	operator<< (int v);
    ostringstream&		operator<< (long v);
    inline ostringstream&	operator<< (u_char v);
    inline ostringstream&	operator<< (u_short v);
    inline ostringstream&	operator<< (u_int v);
    ostringstream&		operator<< (u_long v);
    inline ostringstream&	operator<< (float v);
    ostringstream&		operator<< (double v);
    ostringstream&		operator<< (bool v);
    inline ostringstream&	operator<< (wchar_t v);
    ostringstream&		operator<< (const char* s);
    inline ostringstream&	operator<< (char* s);
    inline ostringstream&	operator<< (const u_char* s);
    inline ostringstream&	operator<< (u_char* s);
    inline ostringstream&	operator<< (const void* s);
    inline ostringstream&	operator<< (void* s);
    ostringstream&		operator<< (const string& v);
    ostringstream&		operator<< (ios::fmtflags f);
    int				format (const char* fmt, ...) __attribute__((__format__(__printf__, 2, 3)));
    inline void			set_base (uint16_t b);
    inline void			set_width (uint16_t w);
    inline void			set_decimal_separator (char s);
    inline void			set_thousand_separator (char s);
    inline void			set_precision (u_short precision);
protected:
    inline virtual size_t	overflow (void)	{ return (0); }
    void			write_buffer (const char* buf, size_t bufSize);
private:
    char			m_DecimalSeparator;	///< Period by default.
    char			m_ThousandSeparator;	///< Comma by default.
    uint16_t			m_Base;			///< Numeric base for writing numbers.
    uint16_t			m_Precision;		///< Number of digits after the decimal separator.
    uint16_t			m_Width;		///< Field width.
    uint32_t			m_Flags;		///< See ios::fmtflags.
};

/// Sets the numeric base for writing numbers.
inline void ostringstream::set_base (uint16_t b)
{
    m_Base = b;
}

/// Sets the numeric base for writing numbers.
inline void ostringstream::set_width (uint16_t w)
{
    m_Width = w;
}

/// Sets the decimal separator in numbers. Period by default.
inline void ostringstream::set_decimal_separator (char s)
{
    m_DecimalSeparator = s;
}

/// Sets the thousand separator in numbers. Comma by default.
inline void ostringstream::set_thousand_separator (char s)
{
    m_ThousandSeparator = s;
}

/// Sets the number of digits after the decimal separator.
inline void ostringstream::set_precision (u_short precision)
{
    m_Precision = precision;
}

/// Writes a single character into the stream.
inline ostringstream& ostringstream::operator<< (char v)
{
    ostream::iwrite (v);
    return (*this);
}

/// Writes a single character into the stream.
inline ostringstream& ostringstream::operator<< (signed char v)
{
    ostream::iwrite (v);
    return (*this);
}

/// Writes a single character into the stream.
inline ostringstream& ostringstream::operator<< (u_char v)
{
    ostream::iwrite (v);
    return (*this);
}

/// Writes number \p v into the stream as text.
inline ostringstream& ostringstream::operator<< (short v)
{
    operator<< (long(v)); return (*this);
}

/// Writes number \p v into the stream as text.
inline ostringstream& ostringstream::operator<< (int v)
{
    operator<< (long(v)); return (*this);
}

/// Writes number \p v into the stream as text.
inline ostringstream& ostringstream::operator<< (u_short v)
{
    operator<< (u_long(v)); return (*this);
}

/// Writes number \p v into the stream as text.
inline ostringstream& ostringstream::operator<< (u_int v)
{
    operator<< (u_long(v)); return (*this);
}

/// Writes number \p v into the stream as text.
inline ostringstream& ostringstream::operator<< (float v)
{
    operator<< (double(v)); return (*this);
}

/// Writes number \p v into the stream as text.
inline ostringstream& ostringstream::operator<< (wchar_t v)
{
    operator<< (long(v)); return (*this);
}

/// Writes text pointed to by \p s into the stream.
inline ostringstream& ostringstream::operator<< (char* s)
{
    return (operator<< (const_cast<const char*>(s)));
}

/// Writes text pointed to by \p s into the stream.
inline ostringstream& ostringstream::operator<< (const u_char* s)
{
    return (operator<< (reinterpret_cast<const char*>(s)));
}

/// Writes text pointed to by \p s into the stream.
inline ostringstream& ostringstream::operator<< (u_char* s)
{
    return (operator<< (const_cast<const u_char*>(s)));
}

/// Writes pointer \p s into the stream as the text of its numeric value.
inline ostringstream& ostringstream::operator<< (const void* s)
{
    return (operator<< (reinterpret_cast<u_long>(s)));
}

/// Writes pointer \p s into the stream as the text of its numeric value.
inline ostringstream& ostringstream::operator<< (void* s)
{
    return (operator<< (const_cast<const void*>(s)));
}

}; // namespace ustl

#endif

