 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_MCP_JSON
#define H_OBJSMOOTH_MCP_JSON

namespace smooth
{
	namespace MCP
	{
		class JSON;
		class JSONWriter;
	};
};

#include "../../definitions.h"
#include "../string.h"
#include "../../templates/array.h"

namespace smooth
{
	namespace MCP
	{
		/* A JSON value with parsing and UTF-8 serialization.
		 */
		class JSON
		{
			public:
				enum Type
				{
					TypeNull = 0,
					TypeBoolean,
					TypeNumber,
					TypeString,
					TypeArray,
					TypeObject
				};
			private:
				Type				 type;

				Bool				 boolean;
				Float				 number;
				String				 string;

				Array<JSON *, Void *>		 items;
				Array<String>			 keys;

				Void				 Clear();
				Void				 Copy(const JSON &);

				Void				 Serialize(JSONWriter &) const;
			public:
								 JSON();
								 JSON(Bool);
								 JSON(Int);
								 JSON(Float);
								 JSON(const String &);
								 JSON(const char *);
								 JSON(const JSON &);

								~JSON();

				JSON &operator			 =(const JSON &);

				static JSON			 NewArray();
				static JSON			 NewObject();

				/* Parse UTF-8 encoded JSON text. Returns
				 * False on syntax errors.
				 */
				static Bool			 Parse(const char *, JSON &);

				/* Serialize to UTF-8 encoded JSON text. The
				 * result must be deleted with delete [].
				 */
				char				*Serialize() const;

				Type				 GetType() const		{ return type; }

				Bool				 IsNull() const			{ return type == TypeNull; }
				Bool				 IsBoolean() const		{ return type == TypeBoolean; }
				Bool				 IsNumber() const		{ return type == TypeNumber; }
				Bool				 IsString() const		{ return type == TypeString; }
				Bool				 IsArray() const		{ return type == TypeArray; }
				Bool				 IsObject() const		{ return type == TypeObject; }

				Bool				 GetBoolean() const		{ return boolean; }
				Float				 GetNumber() const		{ return number; }
				const String			&GetString() const		{ return string; }

				/* Array and object access. Missing elements
				 * are returned as null values.
				 */
				Int				 Length() const			{ return items.Length(); }
				const JSON			&GetNth(Int) const;

				const String			&GetNthKey(Int n) const		{ return keys.GetNth(n); }

				Bool				 Has(const String &) const;
				const JSON			&Get(const String &) const;

				JSON				&Add(const JSON &);
				JSON				&Set(const String &, const JSON &);
		};
	};
};

#endif
