// :mode=c++:
/*
encode.h - c++ wrapper for a base64 encoding algorithm

This is part of the libb64 project, and has been placed in the public domain.
For details, see http://sourceforge.net/projects/libb64
*/
#ifndef BASE64_ENCODE_H
#define BASE64_ENCODE_H

#include <iostream>

namespace base64
{
	extern "C" 
	{
		#include "cencode.h"
	}

	struct encoder
	{
		enum { BUFFERSIZE = 65536 };

		base64_encodestate _state;
		int _buffersize;

		encoder(int buffersize_in = BUFFERSIZE)
		: _buffersize(buffersize_in)
		{
			base64_init_encodestate(&_state);
		}

		int encode(char value_in)
		{
			return base64_encode_value(value_in);
		}

		int encode(const char* plaintext_in, int length_in, char* code_out)
		{
			return base64_encode_block(plaintext_in, length_in, code_out, &_state);
		}

		int encode_end(char* plaintext_out)
		{
			return base64_encode_blockend(plaintext_out, &_state);
		}

		void encode(std::istream& istream_in, std::ostream& ostream_in)
		{
			const int N = _buffersize;
			char* plaintext = new char[N];
			char* code = new char[2*N];
			int plainlength;
			int codelength;

			do
			{
				istream_in.read(plaintext, N);
				plainlength = istream_in.gcount();
				//
				codelength = encode(plaintext, plainlength, code);
				ostream_in.write(code, codelength);
			}
			while (istream_in.good() && plainlength > 0);

			codelength = encode_end(code);
			ostream_in.write(code, codelength);
			//
			base64_init_encodestate(&_state);

			delete [] code;
			delete [] plaintext;
		}
	};

} // namespace base64

#endif // BASE64_ENCODE_H

