/*
**  Copyright(C) 2017, StepToSky
**
**  Redistribution and use in source and binary forms, with or without
**  modification, are permitted provided that the following conditions are met:
**
**  1.Redistributions of source code must retain the above copyright notice, this
**    list of conditions and the following disclaimer.
**  2.Redistributions in binary form must reproduce the above copyright notice,
**    this list of conditions and the following disclaimer in the documentation
**    and / or other materials provided with the distribution.
**  3.Neither the name of StepToSky nor the names of its contributors
**    may be used to endorse or promote products derived from this software
**    without specific prior written permission.
**
**  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**  DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
**  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**  Contacts: www.steptosky.com
*/

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

inline wchar_t * toWchar(const char * ascii, size_t inCount) {
	assert(ascii);
	inCount += 1;

#ifdef _MSC_VER
	wchar_t * buffer = new wchar_t[inCount];
	buffer[0] = '\0';
	size_t res = 0;
	if (mbstowcs_s(&res, buffer, inCount, ascii, inCount - 1) == 0)
		buffer[inCount - 1] = '\0';
	return buffer;
#else
    wchar_t *buffer = new wchar_t[inCount];
    buffer[0] = '\0';
    if (mbstowcs(buffer, ascii, inCount) == inCount)
        buffer[inCount - 1] = '\0';
    return buffer;
#endif // _MSC_VER

}

inline char * toMbChar(const wchar_t * wide, size_t inCount) {
	assert(wide);
	inCount += 1;

#ifdef _MSC_VER
	char * buffer = new char[inCount];
	buffer[0] = '\0';
	size_t res = 0;
	if (wcstombs_s(&res, buffer, inCount, wide, inCount - 1) == 0)
		buffer[inCount - 1] = '\0';
	return buffer;
#else
    char *buffer = new char[inCount];
    buffer[0] = '\0';
    if (wcstombs(buffer, wide, inCount) == inCount)
        buffer[inCount - 1] = '\0';
    return buffer;
#endif // _MSC_VER

}

/****************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************************************************************************/

inline std::string toMbString(const std::wstring & str) {
	char * c = toMbChar(str.data(), str.length());
	std::string out(c);
	delete[]c;
	return out;
}

inline std::string toMbString(const wchar_t * str) {
	assert(str);
	char * c = toMbChar(str, wcslen(str));
	std::string out(c);
	delete[]c;
	return out;
}

inline std::string toMbString(const long double & v, uint8_t precision) {
	char buff[64];
	std::string prec = ("%.");
	prec.append(toMbString(precision));
	prec.append("Lf");
#ifdef _MSC_VER
	return (sprintf_s(buff, 64, prec.data(), v) < 0) ? std::string() : std::string(buff);
#else
    return (sprintf(buff, prec.data(), v) < 0) ? std::string() : std::string(buff);
#endif // _MSC_VER
}

inline std::string toMbString(const double & v, uint8_t precision) {
	char buff[64];
	std::string prec = ("%.");
	prec.append(toMbString(precision));
	prec.append("f");
#ifdef _MSC_VER
	return (sprintf_s(buff, 64, prec.data(), v) < 0) ? std::string() : std::string(buff);
#else
    return (sprintf(buff, prec.data(), v) < 0) ? std::string() : std::string(buff);
#endif // _MSC_VER
}

inline std::string toMbString(const float & v, uint8_t precision) {
	char buff[64];
	std::string prec = ("%.");
	prec.append(toMbString(precision));
	prec.append("f");
#ifdef _MSC_VER
	return (sprintf_s(buff, 64, prec.data(), v) < 0) ? std::string() : std::string(buff);
#else
    return (sprintf(buff, prec.data(), v) < 0) ? std::string() : std::string(buff);
#endif // _MSC_VER
}

/****************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************************************************************************/

inline std::wstring toWString(const std::string & str) {
	wchar_t * c = toWchar(str.c_str(), str.length());
	std::wstring out(c);
	delete[]c;
	return out;
}

inline std::wstring toWString(const char * str) {
	assert(str);
	wchar_t * c = toWchar(str, strlen(str));
	std::wstring out(c);
	delete[]c;
	return out;
}

inline std::wstring toWString(const long double & v, uint8_t precision) {
	wchar_t buff[64];
	std::wstring prec = (L"%.");
	prec.append(toWString(precision));
	prec.append(L"Lf");
#ifdef _MSC_VER
	return (swprintf_s(buff, 64, prec.data(), v) < 0) ? std::wstring() : std::wstring(buff);
#else
    return (swprintf(buff, 64, prec.data(), v) < 0) ? std::wstring() : std::wstring(buff);
#endif // _MSC_VER
}

inline std::wstring toWString(const double & v, uint8_t precision) {
	wchar_t buff[64];
	std::wstring prec = (L"%.");
	prec.append(toWString(precision));
	prec.append(L"f");
#ifdef _MSC_VER
	return (swprintf_s(buff, 64, prec.data(), v) < 0) ? std::wstring() : std::wstring(buff);
#else
    return (swprintf(buff, 64, prec.data(), v) < 0) ? std::wstring() : std::wstring(buff);
#endif // _MSC_VER
}

inline std::wstring toWString(const float & v, uint8_t precision) {
	wchar_t buff[64];
	std::wstring prec = (L"%.");
	prec.append(toWString(precision));
	prec.append(L"f");
#ifdef _MSC_VER
	return (swprintf_s(buff, 64, prec.data(), v) < 0) ? std::wstring() : std::wstring(buff);
#else
    return (swprintf(buff, 64, prec.data(), v) < 0) ? std::wstring() : std::wstring(buff);
#endif // _MSC_VER	
}

/****************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************************************************************************/
