 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/misc/mcp/json.h>
#include <smooth/misc/math.h>

#include <stdio.h>
#include <string.h>
#include <math.h>

namespace smooth
{
	namespace MCP
	{
		/* Growable UTF-8 byte buffer.
		 */
		class JSONWriter
		{
			private:
				char	*data;
				Int	 size;
				Int	 capacity;
			public:
					 JSONWriter() : data(NIL), size(0), capacity(0)	{ Append("", 0); }
					~JSONWriter()					{ delete [] data; }

				Void	 Append(const char *bytes, Int length)
				{
					if (size + length + 1 > capacity)
					{
						Int	 nCapacity = Math::Max(Math::Max(capacity * 2, size + length + 1), 256);
						char	*nData	   = new char [nCapacity];

						if (data != NIL) memcpy(nData, data, size);

						delete [] data;

						data	 = nData;
						capacity = nCapacity;
					}

					memcpy(data + size, bytes, length);

					size += length;
					data[size] = 0;
				}

				Void	 Append(const char *string)	{ Append(string, strlen(string)); }
				Void	 Append(char c)			{ Append(&c, 1); }

				const char *GetData() const		{ return data; }

				char	*Detach()			{ char *result = data; data = NIL; size = capacity = 0; return result; }
		};
	};
};

using namespace smooth;
using namespace smooth::MCP;

namespace
{
	const Int	 maxDepth = 64;

	Void SkipWhitespace(const char *&p)
	{
		while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') p++;
	}

	Int ParseHex(const char *&p)
	{
		Int	 value = 0;

		for (Int i = 0; i < 4; i++, p++)
		{
			value <<= 4;

			if	(*p >= '0' && *p <= '9') value |= *p - '0';
			else if (*p >= 'a' && *p <= 'f') value |= *p - 'a' + 10;
			else if (*p >= 'A' && *p <= 'F') value |= *p - 'A' + 10;
			else				 return -1;
		}

		return value;
	}

	Void AppendUTF8(JSONWriter &writer, Int code)
	{
		char	 bytes[4];

		if	(code < 0x80)	 { bytes[0] = code;											  writer.Append(bytes, 1); }
		else if (code < 0x800)	 { bytes[0] = 0xC0 | (code >>  6); bytes[1] = 0x80 | (code & 0x3F);					  writer.Append(bytes, 2); }
		else if (code < 0x10000) { bytes[0] = 0xE0 | (code >> 12); bytes[1] = 0x80 | (code >>  6 & 0x3F); bytes[2] = 0x80 | (code & 0x3F); writer.Append(bytes, 3); }
		else			 { bytes[0] = 0xF0 | (code >> 18); bytes[1] = 0x80 | (code >> 12 & 0x3F); bytes[2] = 0x80 | (code >> 6 & 0x3F); bytes[3] = 0x80 | (code & 0x3F); writer.Append(bytes, 4); }
	}

	Bool ParseString(const char *&p, String &result)
	{
		if (*p++ != '"') return False;

		JSONWriter	 writer;

		while (True)
		{
			char	 c = *p++;

			if	(c == '"')		      break;
			else if (c == 0 || (unsigned char) c < 0x20) return False;
			else if (c != '\\')		      { writer.Append(c); continue; }

			switch (*p++)
			{
				case '"':  writer.Append('"');	break;
				case '\\': writer.Append('\\'); break;
				case '/':  writer.Append('/');	break;
				case 'b':  writer.Append('\b'); break;
				case 'f':  writer.Append('\f'); break;
				case 'n':  writer.Append('\n'); break;
				case 'r':  writer.Append('\r'); break;
				case 't':  writer.Append('\t'); break;
				case 'u':
					{
						Int	 code = ParseHex(p);

						if (code < 0) return False;

						/* Combine surrogate pairs.
						 */
						if (code >= 0xD800 && code <= 0xDBFF && p[0] == '\\' && p[1] == 'u')
						{
							const char	*next = p + 2;
							Int		 low  = ParseHex(next);

							if (low >= 0xDC00 && low <= 0xDFFF) { code = 0x10000 + ((code - 0xD800) << 10) + (low - 0xDC00); p = next; }
						}

						AppendUTF8(writer, code);
					}

					break;
				default:
					return False;
			}
		}

		result = NIL;

		if (writer.GetData()[0] != 0) result.ImportFrom("UTF-8", writer.GetData());

		return True;
	}

	Bool ParseNumber(const char *&p, Float &result)
	{
		/* Parse numbers manually, as strtod depends on the locale.
		 */
		Float	 sign	  = 1;
		Float	 value	  = 0;
		Bool	 digits	  = False;

		if (*p == '-') { sign = -1; p++; }

		while (*p >= '0' && *p <= '9') { value = value * 10 + (*p++ - '0'); digits = True; }

		if (*p == '.')
		{
			Float	 scale = 0.1;

			for (p++; *p >= '0' && *p <= '9'; p++, scale /= 10) { value += (*p - '0') * scale; digits = True; }
		}

		if (!digits) return False;

		if (*p == 'e' || *p == 'E')
		{
			Int	 exponentSign = 1;
			Int	 exponent     = 0;

			p++;

			if	(*p == '-') { exponentSign = -1; p++; }
			else if (*p == '+') {			 p++; }

			if (*p < '0' || *p > '9') return False;

			while (*p >= '0' && *p <= '9') exponent = Math::Min(exponent * 10 + (*p++ - '0'), 9999);

			value *= pow(10.0, exponentSign * exponent);
		}

		result = sign * value;

		return True;
	}

	Bool ParseValue(const char *&p, JSON &value, Int depth)
	{
		if (depth > maxDepth) return False;

		SkipWhitespace(p);

		if (*p == '{')
		{
			value = JSON::NewObject();

			p++;

			SkipWhitespace(p);

			if (*p == '}') { p++; return True; }

			while (True)
			{
				String	 key;
				JSON	 item;

				SkipWhitespace(p);

				if (!ParseString(p, key)) return False;

				SkipWhitespace(p);

				if (*p++ != ':') return False;

				if (!ParseValue(p, item, depth + 1)) return False;

				value.Set(key, item);

				SkipWhitespace(p);

				if (*p == ',') { p++; continue; }
				if (*p == '}') { p++; return True; }

				return False;
			}
		}
		else if (*p == '[')
		{
			value = JSON::NewArray();

			p++;

			SkipWhitespace(p);

			if (*p == ']') { p++; return True; }

			while (True)
			{
				JSON	 item;

				if (!ParseValue(p, item, depth + 1)) return False;

				value.Add(item);

				SkipWhitespace(p);

				if (*p == ',') { p++; continue; }
				if (*p == ']') { p++; return True; }

				return False;
			}
		}
		else if (*p == '"')
		{
			String	 string;

			if (!ParseString(p, string)) return False;

			value = JSON(string);

			return True;
		}
		else if (strncmp(p, "true", 4) == 0)  { p += 4; value = JSON(True);  return True; }
		else if (strncmp(p, "false", 5) == 0) { p += 5; value = JSON(False); return True; }
		else if (strncmp(p, "null", 4) == 0)  { p += 4; value = JSON();	     return True; }
		else
		{
			Float	 number = 0;

			if (!ParseNumber(p, number)) return False;

			value = JSON(number);

			return True;
		}
	}

	Void AppendString(JSONWriter &writer, const String &string)
	{
		writer.Append('"');

		if (string.Length() > 0)
		{
			const char	*bytes = string.ConvertTo("UTF-8");

			for (const char *c = bytes; *c != 0; c++)
			{
				switch (*c)
				{
					case '"':  writer.Append("\\\"", 2); break;
					case '\\': writer.Append("\\\\", 2); break;
					case '\n': writer.Append("\\n", 2);  break;
					case '\r': writer.Append("\\r", 2);  break;
					case '\t': writer.Append("\\t", 2);  break;
					default:
						if ((unsigned char) *c < 0x20)
						{
							char	 escape[8];

							sprintf(escape, "\\u%04x", *c);

							writer.Append(escape);
						}
						else
						{
							/* Copy runs of unescaped bytes at once.
							 */
							const char	*end = c;

							while (end[1] != 0 && end[1] != '"' && end[1] != '\\' && (unsigned char) end[1] >= 0x20) end++;

							writer.Append(c, end - c + 1);

							c = end;
						}
				}
			}
		}

		writer.Append('"');
	}

	Void AppendNumber(JSONWriter &writer, Float number)
	{
		char	 text[32];

		if	(number != number || fabs(number) > 1e300) { writer.Append("null"); return; }
		else if (number == floor(number) && fabs(number) < 1e15) sprintf(text, "%.0f", number);
		else
		{
			sprintf(text, "%.15g", number);

			/* Use a decimal point regardless of the locale.
			 */
			for (char *c = text; *c != 0; c++) if (*c == ',') *c = '.';
		}

		writer.Append(text);
	}
};

S::MCP::JSON::JSON() : type(TypeNull), boolean(False), number(0)
{
}

S::MCP::JSON::JSON(Bool value) : type(TypeBoolean), boolean(value), number(0)
{
}

S::MCP::JSON::JSON(Int value) : type(TypeNumber), boolean(False), number(value)
{
}

S::MCP::JSON::JSON(Float value) : type(TypeNumber), boolean(False), number(value)
{
}

S::MCP::JSON::JSON(const String &value) : type(TypeString), boolean(False), number(0), string(value)
{
}

S::MCP::JSON::JSON(const char *value) : type(TypeString), boolean(False), number(0), string(value)
{
}

S::MCP::JSON::JSON(const JSON &value) : type(TypeNull), boolean(False), number(0)
{
	Copy(value);
}

S::MCP::JSON::~JSON()
{
	Clear();
}

S::MCP::JSON &S::MCP::JSON::operator =(const JSON &value)
{
	if (&value == this) return *this;

	/* Copy first, as the value may be part of this one.
	 */
	JSON	 copy(value);

	Clear();
	Copy(copy);

	return *this;
}

S::Void S::MCP::JSON::Clear()
{
	for (Int i = 0; i < items.Length(); i++) delete items.GetNth(i);

	items.RemoveAll();
	keys.RemoveAll();

	type	= TypeNull;
	boolean = False;
	number	= 0;
	string	= NIL;
}

S::Void S::MCP::JSON::Copy(const JSON &value)
{
	type	= value.type;
	boolean = value.boolean;
	number	= value.number;
	string	= value.string;

	for (Int i = 0; i < value.items.Length(); i++)
	{
		items.Add(new JSON(*value.items.GetNth(i)));

		if (type == TypeObject) keys.Add(value.keys.GetNth(i));
	}
}

S::MCP::JSON S::MCP::JSON::NewArray()
{
	JSON	 value;

	value.type = TypeArray;

	return value;
}

S::MCP::JSON S::MCP::JSON::NewObject()
{
	JSON	 value;

	value.type = TypeObject;

	return value;
}

S::Bool S::MCP::JSON::Parse(const char *text, JSON &value)
{
	const char	*p = text;

	if (!ParseValue(p, value, 0)) return False;

	SkipWhitespace(p);

	return (*p == 0);
}

char *S::MCP::JSON::Serialize() const
{
	JSONWriter	 writer;

	Serialize(writer);

	return writer.Detach();
}

S::Void S::MCP::JSON::Serialize(JSONWriter &writer) const
{
	switch (type)
	{
		case TypeNull:	  writer.Append("null");			 break;
		case TypeBoolean: writer.Append(boolean ? "true" : "false");	 break;
		case TypeNumber:  AppendNumber(writer, number);			 break;
		case TypeString:  AppendString(writer, string);			 break;
		case TypeArray:
			writer.Append('[');

			for (Int i = 0; i < items.Length(); i++)
			{
				if (i > 0) writer.Append(',');

				items.GetNth(i)->Serialize(writer);
			}

			writer.Append(']');

			break;
		case TypeObject:
			writer.Append('{');

			for (Int i = 0; i < items.Length(); i++)
			{
				if (i > 0) writer.Append(',');

				AppendString(writer, keys.GetNth(i));

				writer.Append(':');

				items.GetNth(i)->Serialize(writer);
			}

			writer.Append('}');

			break;
	}
}

const S::MCP::JSON &S::MCP::JSON::GetNth(Int n) const
{
	static const JSON	 null;

	if (n < 0 || n >= items.Length()) return null;

	return *items.GetNth(n);
}

S::Bool S::MCP::JSON::Has(const String &key) const
{
	for (Int i = 0; i < keys.Length(); i++) if (keys.GetNth(i) == key) return True;

	return False;
}

const S::MCP::JSON &S::MCP::JSON::Get(const String &key) const
{
	static const JSON	 null;

	for (Int i = 0; i < keys.Length(); i++) if (keys.GetNth(i) == key) return *items.GetNth(i);

	return null;
}

S::MCP::JSON &S::MCP::JSON::Add(const JSON &value)
{
	type = TypeArray;

	items.Add(new JSON(value));

	return *this;
}

S::MCP::JSON &S::MCP::JSON::Set(const String &key, const JSON &value)
{
	type = TypeObject;

	for (Int i = 0; i < keys.Length(); i++)
	{
		if (keys.GetNth(i) != key) continue;

		JSON	*item = new JSON(value);

		delete items.GetNth(i);

		items.SetNth(i, item);

		return *this;
	}

	items.Add(new JSON(value));
	keys.Add(key);

	return *this;
}
