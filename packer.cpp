#include "packer.h"
#include <math.H>

void inner_Encode(const void* inData, uint8_t* outData, const GeneratedItem_t* metadata, uint8_t lenmetadata)
{
	uint8_t bt = 0;
	uint8_t aix = 0;
	uint8_t cur5bt = 0;

	for (size_t i = 0; i < lenmetadata; i++)
	{
		uint8_t* p = (uint8_t *) inData;
		float fdata;
		GeneratedItem_t m = metadata[i];
		uint8_t sign = m.type <= FLOAT;

		switch (m.type)
		{
		case INT8:
			fdata = *(int8_t*)&p[m.offset];
			break;
		case INT16:
			fdata = *(int16_t*)&p[m.offset];
			break;
		case INT32:
			fdata = *(int32_t*)&p[m.offset];
			break;
		case FLOAT:
			fdata = *(float*)&p[m.offset];
			break;
		case UINT8:
			fdata = *(uint8_t*)&p[m.offset];
			break;
		case UINT16:
			fdata = *(uint16_t*)&p[m.offset];
			break;
		case UINT32:
			fdata = *(uint32_t*)&p[m.offset];
			break;
		case UFLOAT:
			fdata = *(ufloat*)&p[m.offset];
			break;
		default:
			fdata = 0;
			break;
		}

		fdata =  (fdata + m.delta) * m.kamp;

		if (sign)
		{
			float max = (1UL << (m.bits - 1)) - 1;
			float min =-((float) (1UL << (m.bits - 1)));
			if (fdata < min) fdata = min;
			if (fdata > max) fdata = max;
		}
		else
		{
			float max = (1UL << m.bits) - 1;
			if (fdata < 0) fdata = 0;
			if (fdata > max) fdata = max;
		}

		if (m.bits <= 8)
		{
			uint8_t d;
			d = sign ? (uint8_t)((int8_t)round(fdata)) : (uint8_t)round(fdata);
			d <<= (8 - m.bits);

			uint8_t n = m.bits;
			while (n)
			{
				cur5bt <<= 1;
				if (d & 0x80)
				{
					cur5bt |= 1;
				}
				d <<= 1;
				if (++bt == 5)
				{
					bt = 0;
					outData[aix++] = cur5bt;
					cur5bt = 0;
				}
				n--;
			}
		}
		else if (m.bits <= 16)
		{
			union 
			{
				uint16_t u16;
				uint8_t bt[2];
			} d;
			d.u16 = sign ? (uint16_t)((int16_t)round(fdata)) : (uint16_t)round(fdata);
			d.u16 <<= (16 - m.bits);

			uint8_t n = m.bits;
			while (n)
			{
				cur5bt <<= 1;
				if (d.bt[1] & 0x80)
				{
					cur5bt |= 1;
				}
				d.u16 <<= 1;
				if (++bt == 5)
				{
					bt = 0;
					outData[aix++] = cur5bt;
					cur5bt = 0;
				}
				n--;
			}
		}
		else
		{
			union
			{
				uint32_t u32;
				uint8_t bt[4];
			} d;
			d.u32 = sign ? (uint32_t)((int32_t)round(fdata)) : (uint32_t)round(fdata);
			d.u32 <<= (32 - m.bits);

			uint8_t n = m.bits;
			while (n)
			{
				cur5bt <<= 1;
				if (d.bt[3] & 0x80)
				{
					cur5bt |= 1;
				}
				d.u32 <<= 1;
				if (++bt == 5)
				{
					bt = 0;
					outData[aix++] = cur5bt;
					cur5bt = 0;
				}
				n--;
			}
		}
	}
}

void inner_Decode(const uint8_t* inData, void* outData, const GeneratedItem_t* metadata, uint8_t lenmetadata)
{
	uint8_t bt = 0;
	uint8_t aix = 0;
	uint8_t cur5bt = inData[0];
	uint8_t* p = (uint8_t*)outData;

	for (size_t i = 0; i < lenmetadata; i++)
	{
		float fdata;
		GeneratedItem_t m = metadata[i];
		uint8_t sign = m.type <= FLOAT;

		if (m.bits <= 8)
		{
			uint8_t d = 0;
			uint8_t n = m.bits;
			while (n)
			{
				d <<= 1;
				if (cur5bt & 0x10)
				{
					d |= 1;
				}
				cur5bt <<= 1;
				if (++bt == 5)
				{
					bt = 0;
					cur5bt = inData[++aix];
				}
				n--;
			}
			if (sign )
			{
				if (m.bits < 8)
				{
					uint8_t tsts = 0x80 >> (8 - m.bits);
					// ������������� �����
					if (d & tsts)
					{
						while (tsts <<= 1)
						{
							d |= tsts;
						}
					}				
				}
				fdata = (int8_t)d;
			}
			else
			{
				 fdata = (uint8_t)d;
			}			
		}
		else if (m.bits <= 16)
		{
			uint16_t d = 0;
			uint8_t n = m.bits;
			while (n)
			{
				d <<= 1;
				if (cur5bt & 0x10)
				{
					d |= 1;
				}
				cur5bt <<= 1;
				if (++bt == 5)
				{
					bt = 0;
					cur5bt = inData[++aix];
				}
				n--;
			}
			if (sign)
			{
				if (m.bits < 16)
				{
					uint16_t tsts = 0x8000 >> (16 - m.bits);
					// ������������� �����
					if (d & tsts)
					{
						while (tsts <<= 1)
						{
							d |= tsts;
						}
					}
				}
				fdata = (int16_t)d;
			}
			else
			{
				fdata = (uint16_t)d;
			}

		}
		else
		{
			uint32_t d = 0;
			uint8_t n = m.bits;
			while (n)
			{
				d <<= 1;
				if (cur5bt & 0x10)
				{
					d |= 1;
				}
				cur5bt <<= 1;
				if (++bt == 5)
				{
					bt = 0;
					cur5bt = inData[++aix];
				}
				n--;
			}
			if (sign)
			{
				if (m.bits < 32)
				{
					uint32_t tsts = 0x80000000 >> (32 - m.bits);
					// ������������� �����
					if (d & tsts)
					{
						while (tsts <<= 1)
						{
							d |= tsts;
						}
					}
				}
				fdata = (int32_t)d;
			}
			else
			{
				fdata = (uint32_t)d;
			}
		}

		fdata /= m.kamp;
		fdata -= m.delta;

		switch (m.type)
		{
		case INT8:
			*(int8_t*)&p[m.offset] = fdata;
			break;
		case INT16:
			 *(int16_t*)&p[m.offset] = fdata;
			break;
		case INT32:
			*(int32_t*)&p[m.offset] = fdata;
			break;
		case FLOAT:
			*(float*)&p[m.offset] = fdata;
			break;
		case UINT8:
			*(uint8_t*)&p[m.offset] = fdata;
			break;
		case UINT16:
			*(uint16_t*)&p[m.offset] = fdata;
			break;
		case UINT32:
			*(uint32_t*)&p[m.offset] = fdata;
			break;
		case UFLOAT:
			*(ufloat*)&p[m.offset] = fdata;
			break;
		}
	}
}

uint8_t inner_Quality(size_t idx, uint8_t* inQuality, const GeneratedItem_t* metadata, uint8_t lenmetadata)
{
	GeneratedItem_t m = metadata[idx];

	uint8_t q = 100;
	uint8_t* p = &inQuality[m.OffsetInArray];
	for (size_t j = 0; j < m.N5bit; j++)
	{
		if (q > *p) q = *p;
		p++;
	}
	return q;
}
