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
// fdostringstream.cc
//

#include "fdostream.h"
#include "ustring.h"
#include "uexception.h"
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdarg.h>

namespace ustl {

//----------------------------------------------------------------------

fdistringstream cin  (STDIN_FILENO);
fdostringstream cout (STDOUT_FILENO);
fdostringstream cerr (STDERR_FILENO);

//----------------------------------------------------------------------

fdostringstream::fdostringstream (int fd, size_t bufSize)
: ostringstream (m_Buffer),
  m_Buffer (bufSize),
  m_Fd (fd)
{
    link (m_Buffer);
}

fdostringstream::~fdostringstream (void)
{
    flush();
}

void fdostringstream::flush (void)
{
    while (pos() && overflow());
}

size_t fdostringstream::overflow (void)
{
    size_t bw = 0;
    while (!bw) {
	errno = 0;
	ssize_t bwn = ::write (m_Fd, cdata() + bw, pos() - bw);
	if (bwn < 0) {
	    if (errno != EAGAIN && errno != EINTR)
		throw libc_exception ("write");
	} else if (bwn == 0)
	    break;
	else
	    bw += bwn;
    }
    erase (begin(), bw);
    return (remaining());
}

/// Equivalent to a sprintf on the string.
int fdostringstream::format (const char* fmt, ...)
{
    if (remaining() < strlen(fmt) * 5)
	flush();
    va_list args;
    va_start (args, fmt);
    char* dest = reinterpret_cast<char*>(begin()) + pos();
    int rv = vsnprintf (dest, remaining(), fmt, args);
    if (rv > 0)
	skip (rv);
    va_end (args);
    return (rv);
}

//----------------------------------------------------------------------

fdistringstream::fdistringstream (int fd, size_t bufSize)
: istringstream (m_Buffer),
  m_Buffer (bufSize),
  m_Fd (fd)
{
    link (m_Buffer, 0);
}

fdistringstream::~fdistringstream (void)
{
}

size_t fdistringstream::underflow (void)
{
    m_Buffer.erase (m_Buffer.begin(), pos());
    size_t br = 0;
    while (!br) {
	errno = 0;
	ssize_t brn = ::read (m_Fd, m_Buffer.begin() + br, m_Buffer.size() - br);
	if (brn < 0) {
	    if (errno != EAGAIN && errno != EINTR)
		throw libc_exception ("read");
	} else if (brn == 0)
	    break;
	else
	    br += brn;
    }
    link (m_Buffer, br);
    return (remaining());
}

//----------------------------------------------------------------------

}; // namespace ustl

