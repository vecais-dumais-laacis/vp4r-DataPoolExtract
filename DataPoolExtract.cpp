// DataPoolExtract.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <windows.h>
#include <stdint.h>

struct Datapool_bvf
{
    uint32_t fixed_header[4];
    uint32_t some_fixed_value[4];
    uint32_t foofoo;
    uint32_t should_be_1;
    uint32_t file_size;
    uint32_t should_be_ffff0000;
    uint32_t language_count;
    uint32_t main_offset;
    uint32_t offset1;
    uint32_t offset2;
    uint32_t offset3;
    uint32_t offset4;
    uint32_t offset5;
    uint32_t offset6;
    uint32_t offset7;
    uint32_t offset8;
    uint32_t offset_item_headers;
    uint32_t offset_items_per_lang;//items per language
    uint32_t item_id_offset;
    uint32_t offset12;
};

struct Datapool_bdf
{
    uint32_t fixed_header[4];
    uint32_t uint8_ram_size_maybe;
    uint32_t negative_one;
    uint32_t itemId;
    uint32_t field_1C;
    uint32_t should_be_ff00ff00;
    uint32_t should_be_four;
    uint32_t file_size;
    uint32_t counter1;
    uint32_t counter2;
    uint32_t counter3;
    uint32_t counter4;
    uint32_t offset1;
    uint32_t offset2;
    uint32_t offset3;
    uint32_t offset4;
    uint32_t offset5;
    uint32_t offset6;
    uint32_t offset7;
    uint32_t offset8;
    uint32_t offset9;
    uint32_t offset10;
    uint32_t offset11;
    uint32_t offset12;
    uint32_t offset13;
    uint32_t offset14;
    uint32_t field_74;
    uint32_t field_78;
    uint32_t field_2C;
};

struct BdfLoaderStruct
{
    uint32_t field_0;
    uint32_t field_4e;
    uint32_t ptr_to_8_bytes;
    uint32_t ptr_to_chunkdata;
    uint32_t ptr_to_8_bytes2;
    uint8_t *bdf_data;
    uint16_t *bdf_data_at_offset1;
    uint8_t *bdf_data_at_offset2;
    uint8_t* bdf_data_at_offset3;
    uint8_t* bdf_data_at_offset4;
    uint8_t* bdf_data_at_offset5;
    uint8_t* bdf_data_at_offset6;
    uint8_t* bdf_data_at_offset7;
    uint8_t* bdf_data_at_offset8;
    uint8_t* bdf_data_at_offset9;
    uint32_t* bdf_data_at_offset10;
    uint32_t* bdf_data_at_offset11;
    uint8_t* bdf_data_at_offset12;
    uint8_t* bdf_data_at_offset13;
    uint8_t* bdf_data_at_offset14;

	uint32_t counter1;
	uint32_t counter2;
	uint32_t counter3;
	uint32_t counter4;
};

struct sStruct
{
    uint16_t value_0x100_or_0x101;
    uint32_t store_data_from_bdf_or_0x100_or_0x101;
    uint32_t itemId;
    uint32_t negative_one_2;
    uint32_t should_be_zero;
};

void hex_dump(uint8_t* data, int size)
{
    for (int k = 0; k < size; k++)
    {
        if (k)
            printf(" ");
        printf("%02X", data[k]);
    }
    printf("\r\n");
	for (int k = 0; k < size; k++)
	{
		if (k)
			printf(" ");
		printf(" %c", data[k]);
	}
	printf("\r\n");
}

wchar_t* DbUtf8ToWide(char* src, wchar_t* dst)
{
	if (src[0]) {
		dst[MultiByteToWideChar(CP_UTF8, 0, src, (int)strlen(src), dst, (int)strlen(src) * 2)] = 0;
	}
	else {
		dst[0] = 0;
	}
	return dst;
}

int main( int argc, char *argv[] )
{
    SetConsoleOutputCP(CP_UTF8);

    if (argc == 1)
    {
        //no params - just extract datapool
        FILE* f = fopen("datapool.bvf", "rb");
        if (!f)
        {
            printf("Could not open datapool.bvf\r\n");
            return 1;
        }
        fseek(f, 0, SEEK_END);
        int fs = ftell(f);
        fseek(f, 0, SEEK_SET);
        uint8_t* data = new uint8_t[fs];
        uint8_t* usage_data = new uint8_t[fs];
        memset(usage_data, 0, fs);
        fread(data, 1, fs, f);
        fclose(f);
        Datapool_bvf* header = (Datapool_bvf*)data;

        f = fopen("datapool.bdf", "rb");
        if (!f)
        {
            printf("Could not open datapool.bdf\r\n");
            return 1;
        }
        fseek(f, 0, SEEK_END);
        int dfs = ftell(f);
        fseek(f, 0, SEEK_SET);
        uint8_t* ddata = new uint8_t[dfs];
        fread(ddata, 1, dfs, f);
        fclose(f);

        Datapool_bdf* dheader = (Datapool_bdf*)ddata;

        BdfLoaderStruct dloader;
        dloader.bdf_data = ddata;
        dloader.bdf_data_at_offset1 = (uint16_t*)(ddata + dheader->offset1);
        dloader.bdf_data_at_offset2 = ddata + dheader->offset2;
        dloader.bdf_data_at_offset3 = ddata + dheader->offset3;
        dloader.bdf_data_at_offset4 = ddata + dheader->offset4;
        dloader.bdf_data_at_offset5 = ddata + dheader->offset5;
        dloader.bdf_data_at_offset6 = ddata + dheader->offset6;
        dloader.bdf_data_at_offset7 = ddata + dheader->offset7;
        dloader.bdf_data_at_offset8 = ddata + dheader->offset8;
        dloader.bdf_data_at_offset9 = ddata + dheader->offset9;
        dloader.bdf_data_at_offset10 = (uint32_t*)(ddata + dheader->offset10);
        dloader.bdf_data_at_offset11 = (uint32_t*)(ddata + dheader->offset11);
        dloader.bdf_data_at_offset12 = ddata + dheader->offset12;
        dloader.bdf_data_at_offset13 = ddata + dheader->offset13;
        dloader.bdf_data_at_offset14 = ddata + dheader->offset14;

        dloader.counter1 = dheader->counter1;
        dloader.counter2 = dheader->counter2;
        dloader.counter3 = dheader->counter3;
        dloader.counter4 = dheader->counter4;

        for (uint32_t item_idx = 0; item_idx < header->language_count; item_idx++)
        {
            uint32_t val1 = *(uint32_t*)(data + header->offset1 + item_idx * 4);
            *(uint32_t*)(usage_data + header->offset1 + item_idx * 4) = 0xFEFEFEFE;

            uint32_t tmp = *(uint32_t*)(data + header->offset4 + item_idx * 4);
            *(uint32_t*)(usage_data + header->offset4 + item_idx * 4) = 0xFDFDFDFD;

            uint32_t val2 = *(uint32_t*)(data + header->main_offset + tmp);
            *(uint32_t*)(usage_data + header->main_offset + tmp) = 0xFCFCFCFC;

            uint32_t val3 = *(uint32_t*)(data + header->offset2 + item_idx * 4);
            *(uint32_t*)(usage_data + header->offset2 + item_idx * 4) = 0xFBFBFBFB;

            tmp = *(uint32_t*)(data + header->offset3 + item_idx * 4);
            *(uint32_t*)(usage_data + header->offset3 + item_idx * 4) = 0xFAFAFAFA;
            char* val4 = (char*)(data + header->main_offset + tmp);
            *(uint32_t*)(usage_data + header->main_offset + tmp) = 0xF9F9F9F9;

            tmp = *(uint32_t*)(data + header->offset4 + item_idx * 4);
            uint32_t val5 = *(uint32_t*)(data + header->main_offset + tmp);

            uint32_t val6 = *(uint32_t*)(data + header->offset2 + item_idx * 4);

            tmp = *(uint32_t*)(data + header->offset4 + item_idx * 4);
            char* val7 = (char*)(data + header->main_offset + tmp);

            tmp = *(uint32_t*)(data + header->offset7 + item_idx * 4);
            *(uint32_t*)(usage_data + header->offset7 + item_idx * 4) = 0xF8F8F8F8;

            uint32_t val8 = *(uint32_t*)(data + header->main_offset + tmp);
            *(uint32_t*)(usage_data + header->main_offset + tmp) = 0xF7F7F7F7;

            uint32_t val9 = *(uint32_t*)(data + header->offset5 + item_idx * 4);
            *(uint32_t*)(usage_data + header->offset5 + item_idx * 4) = 0xF6F6F6F6;

            tmp = *(uint32_t*)(data + header->offset6 + item_idx * 4);
            *(uint32_t*)(usage_data + header->offset6 + item_idx * 4) = 0xF5F5F5F5;
            char* val10 = (char*)(data + header->main_offset + tmp);
            *(uint32_t*)(usage_data + header->main_offset + tmp) = 0xF4F4F4F4;

            tmp = *(uint32_t*)(data + header->offset7 + item_idx * 4);
            char* val11 = (char*)(data + header->main_offset + tmp);

            tmp = *(uint32_t*)(data + header->offset8 + item_idx * 4);
            *(uint32_t*)(usage_data + header->offset8 + item_idx * 4) = 0xF3F3F3F3;
            char* val12 = (char*)(data + header->main_offset + tmp);
            *(uint32_t*)(usage_data + header->main_offset + tmp) = 0xF2F2F2F2;

            printf("item %u: %u, %u, %u, %s, %u, %u, %s, %u, %u, %s, %s, %s\r\n", item_idx, val1, val2, val3, val4, val5, val6, val7, val8, val9, val10, val11, val12);
        }

        uint32_t lang_index = 1;
        while (true)
        {
            uint32_t items_in_this_type = *(uint32_t*)(data + header->offset_items_per_lang + lang_index * 4);
            *(uint32_t*)(usage_data + header->offset_items_per_lang + lang_index * 4) = 0xF1F1F1F1;

            if (!items_in_this_type)
                break;

            const char* currentLang = (char*)(data + header->main_offset + *(uint32_t*)(data + header->offset6 + lang_index * 4));
            if (!currentLang[0])
            {
                currentLang = "Default";
            }
            char f_name[1024];
            sprintf(f_name, "%s.txt", currentLang);
            f = fopen(f_name, "wb");
            if (!f)
            {
                printf("Could not create output file %s\r\n", f_name);
                break;
            }

            printf("type %u have %u items\r\n", lang_index, items_in_this_type);

            uint32_t item_header_offset = *(uint32_t*)(data + header->offset_item_headers + lang_index * 4) * 4;

            for (uint32_t item_index = 0; item_index < items_in_this_type; item_index++)
            {
                //maybe item id
                uint32_t itemId = *(uint32_t*)(data + item_header_offset + header->item_id_offset);
                *(uint32_t*)(usage_data + item_header_offset + header->item_id_offset) = 0xF0F0F0F0;
                if (1)
                {
                    uint16_t valueType = *(uint16_t*)(ddata + dheader->offset1 + itemId * 2);
                    uint8_t implementationType = *(uint8_t*)(ddata + dheader->offset3 + valueType);
                    uint8_t value_type_value2 = *(uint32_t*)(ddata + dheader->offset2 + 4 * implementationType);

                    //if ( (valueType != 22) && (value_type_value2 != 7 ) )
                    {
                        uint32_t value_offset = *(uint32_t*)(data + item_header_offset + header->offset12);
                        *(uint32_t*)(usage_data + item_header_offset + header->offset12) = 0xefefefef;

                        bool isString = false;
                        if ((valueType == 22) && (implementationType == 1) && (value_type_value2 == 7))
                            isString = true;
						if ((valueType == 23) && (implementationType == 1) && (value_type_value2 == 7))
							isString = true;
                        if ((valueType == 3) && (implementationType == 1) && (value_type_value2 == 7))
                            isString = true;
                        if ((valueType == 21) && (implementationType == 1) && (value_type_value2 == 7))
                            isString = true;
                        if ((valueType == 46) && (implementationType == 1) && (value_type_value2 == 7))
                            isString = true;

                        if (isString)
                        {
                            char* value = (char*)(data + header->offset12 + value_offset + item_header_offset);
                            if(strlen(value))
                                fprintf(f, "%u: %s\r\n", itemId, value);
                        }
                        if(false)//else
                        {
                            //printf("item %u: %u %u, %u, %u %u\r\n", item_index, val1, valueType, implementationType, value_type_value2, value_offset + initial_type_offset + header->offset12 );
                            //hex_dump(data + header->offset12 + initial_type_offset, 8 );

                            if ((valueType == 23) || (valueType == 24) || (valueType == 2))
                            {
                                uint32_t* data_ptr = &value_offset;
                                printf("item idx %u: id: %u valueType: %u, impType: %u, vtype2: %u offset: %u data: %u\r\n", item_index, itemId, valueType, implementationType, value_type_value2, value_offset + item_header_offset + header->offset12, *data_ptr);
                                //(*(uint32_t*)(usage_data + item_header_offset + header->offset12 + value_offset))++;
                            }
                            else
                            {
                                //item idx 2086: id: 5856 valueType: 22, impType: 1, vtype2: 7 offset: 733149 data0: 2228856792 data1: 3017320408
                                if ((value_type_value2 == 7) && (valueType == 22) && (implementationType == 1))
                                {
                                    /*
                                    uint32_t* data_ptr = (uint32_t*)(data + header->offset12 + value_offset + item_header_offset);
                                    printf("item idx %u: id: %u valueType: %u, impType: %u, vtype2: %u offset: %u data0: %u data1: %u\r\n", item_index, itemId, valueType, implementationType, value_type_value2, value_offset + item_header_offset + header->offset12, *data_ptr, *(data_ptr + 1));
                                    char* value = (char*)data_ptr;// data + header->offset12 + value_offset + item_header_offset;
                                    memset(usage_data + header->offset12 + value_offset + item_header_offset, 0xFF, strlen(value) + 1);
                                    if (value[0])
                                    {
                                        printf("value: %s\r\n", value);
                                    }
                                    */
                                    /*
                                    printf("item %u: %u %u, %u, %u %u\r\n", item_index, val1, valueType, implementationType, value_type_value2, value_offset + initial_type_offset + header->offset12);
                                    char* value = (char*)data + initial_type_offset + header->offset12 + value_offset;
                                    memset(usage_data + initial_type_offset + header->offset12 + value_offset, 0xFF, strlen(value) + 1 );
                                    if (value[0])
                                        printf("value: %s\r\n", value);
                                    */
                                }
                                else
                                {
                                    uint32_t* data_ptr = (uint32_t*)(data + header->offset12 + item_header_offset + value_offset);
                                    uint32_t* data_ptr2 = (uint32_t*)(((uint8_t*)data_ptr) + *data_ptr);
                                    printf("item idx %u: id: %u valueType: %u, impType: %u, vtype2: %u offset: %u data0: %u data1: %u\r\n", item_index, itemId, valueType, implementationType, value_type_value2, value_offset + item_header_offset + header->offset12, *data_ptr, *(data_ptr + 1));
                                    if ((uint8_t*)data_ptr2 < data + header->file_size)
                                    {
                                        printf("Jumped to: %u with value: %u %u\r\n", (uint8_t*)data_ptr2 - (uint8_t*)data, *data_ptr2, *(data_ptr2 + 1));
                                    }
                                    //(*(uint32_t*)(usage_data + item_header_offset + header->offset12 + value_offset))++;

                                    char* value = (char*)data_ptr;// data + header->offset12 + value_offset + item_header_offset;
                                    //memset(usage_data + header->offset12 + value_offset + item_header_offset, 0xFF, strlen(value) + 1);
                                    if (value[0])
                                    {
                                        printf("value: %s\r\n", value);
                                    }
                                }
                            }
                        }

                        //hex_dump(data + initial_type_offset + header->offset12 + val2, 8);

                        //uint32_t val3 = *(uint32_t*)(data + initial_type_offset + header->offset12 + 4 );

                        //hex_dump(data + val3, 8);
                    }
                }
                item_header_offset += 4;
            }

            lang_index++;

            fclose(f);
        }

        delete[]data;
        delete[] ddata;

        f = fopen("usage.data", "wb");
        if (f)
        {
            fwrite(usage_data, 1, fs, f);
            fclose(f);
        }

        delete[] usage_data;
    }
    else
    {
        if (argc != 3)
        {
            printf("usage: %s name_of_lang_to_edit file_to_replace_with\r\n", argv[0]);
            return 1;
        }
        FILE* f = fopen("datapool.bvf", "rb");
        if (!f)
        {
            printf("Could not open datapool.bvf\r\n");
            return 1;
        }
        fseek(f, 0, SEEK_END);
        int fs = ftell(f);
        fseek(f, 0, SEEK_SET);
        uint8_t* data = new uint8_t[fs];
        fread(data, 1, fs, f);
        fclose(f);
        Datapool_bvf* header = (Datapool_bvf*)data;

        f = fopen("datapool.bdf", "rb");
        if (!f)
        {
            printf("Could not open datapool.bdf\r\n");
            return 1;
        }
        fseek(f, 0, SEEK_END);
        int dfs = ftell(f);
        fseek(f, 0, SEEK_SET);
        uint8_t* ddata = new uint8_t[dfs];
        fread(ddata, 1, dfs, f);
        fclose(f);

        Datapool_bdf* dheader = (Datapool_bdf*)ddata;

        BdfLoaderStruct dloader;
        dloader.bdf_data = ddata;
        dloader.bdf_data_at_offset1 = (uint16_t*)(ddata + dheader->offset1);
        dloader.bdf_data_at_offset2 = ddata + dheader->offset2;
        dloader.bdf_data_at_offset3 = ddata + dheader->offset3;
        dloader.bdf_data_at_offset4 = ddata + dheader->offset4;
        dloader.bdf_data_at_offset5 = ddata + dheader->offset5;
        dloader.bdf_data_at_offset6 = ddata + dheader->offset6;
        dloader.bdf_data_at_offset7 = ddata + dheader->offset7;
        dloader.bdf_data_at_offset8 = ddata + dheader->offset8;
        dloader.bdf_data_at_offset9 = ddata + dheader->offset9;
        dloader.bdf_data_at_offset10 = (uint32_t*)(ddata + dheader->offset10);
        dloader.bdf_data_at_offset11 = (uint32_t*)(ddata + dheader->offset11);
        dloader.bdf_data_at_offset12 = ddata + dheader->offset12;
        dloader.bdf_data_at_offset13 = ddata + dheader->offset13;
        dloader.bdf_data_at_offset14 = ddata + dheader->offset14;

        dloader.counter1 = dheader->counter1;
        dloader.counter2 = dheader->counter2;
        dloader.counter3 = dheader->counter3;
        dloader.counter4 = dheader->counter4;

        uint32_t lang_to_replace = _stricmp(argv[1], "Default") == 0 ? 0 : - 1;

        for (uint32_t item_idx = 1; item_idx < header->language_count; item_idx++)
        {
            uint32_t val1 = *(uint32_t*)(data + header->offset1 + item_idx * 4);

            uint32_t tmp = *(uint32_t*)(data + header->offset4 + item_idx * 4);

            uint32_t val2 = *(uint32_t*)(data + header->main_offset + tmp);

            uint32_t val3 = *(uint32_t*)(data + header->offset2 + item_idx * 4);

            tmp = *(uint32_t*)(data + header->offset3 + item_idx * 4);
            char* val4 = (char*)(data + header->main_offset + tmp);

            tmp = *(uint32_t*)(data + header->offset4 + item_idx * 4);
            uint32_t val5 = *(uint32_t*)(data + header->main_offset + tmp);

            uint32_t val6 = *(uint32_t*)(data + header->offset2 + item_idx * 4);

            tmp = *(uint32_t*)(data + header->offset4 + item_idx * 4);
            char* val7 = (char*)(data + header->main_offset + tmp);

            tmp = *(uint32_t*)(data + header->offset7 + item_idx * 4);

            uint32_t val8 = *(uint32_t*)(data + header->main_offset + tmp);

            uint32_t val9 = *(uint32_t*)(data + header->offset5 + item_idx * 4);

            tmp = *(uint32_t*)(data + header->offset6 + item_idx * 4);
            char* val10 = (char*)(data + header->main_offset + tmp);

            tmp = *(uint32_t*)(data + header->offset7 + item_idx * 4);
            char* val11 = (char*)(data + header->main_offset + tmp);

            tmp = *(uint32_t*)(data + header->offset8 + item_idx * 4);
            char* val12 = (char*)(data + header->main_offset + tmp);

            printf("item %u: %u, %u, %u, %s, %u, %u, %s, %u, %u, %s, %s, %s\r\n", item_idx, val1, val2, val3, val4, val5, val6, val7, val8, val9, val10, val11, val12);

            if (_stricmp(val10, argv[1]) == 0)
            {
                lang_to_replace = item_idx;
            }
        }

        if (lang_to_replace == -1)
        {
            printf("Could not find lang: %s\r\n", argv[1]);
            return 9;
        }

		f = fopen(argv[2], "rb");
		if (!f)
		{
			printf("Could not open lang file %s\r\n", argv[2]);
			return 3;
		}

        char* new_string_buffer = new char[ 1024 * 1024 ];
        uint32_t new_string_buffer_size = 0;
        uint32_t num_changed_strings = 0;
        char* lang_string = new char[1024 * 1024];
        while (fgets(lang_string, 1024 * 1024, f))
        {
            char* tmp = strchr(lang_string, 0x0A);
            if (tmp)
                tmp[0] = 0;
			tmp = strchr(lang_string, 0x0D);
			if (tmp)
				tmp[0] = 0;

            tmp = strchr(lang_string, ':');
            if (tmp)
            {
                tmp[0] = 0;
                uint32_t item_to_replace = atol(lang_string);
                char* item_new_value = tmp + 2;

                //find item in db
                uint32_t items_in_this_type = *(uint32_t*)(data + header->offset_items_per_lang + lang_to_replace * 4);

                if (!items_in_this_type)
                    continue;

                uint32_t item_header_offset = *(uint32_t*)(data + header->offset_item_headers + lang_to_replace * 4) * 4;

                for (uint32_t item_index = 0; item_index < items_in_this_type; item_index++, item_header_offset += 4)
                {
                    //maybe item id
                    uint32_t itemId = *(uint32_t*)(data + item_header_offset + header->item_id_offset);
                    if (itemId != item_to_replace)
                        continue;

                    uint16_t valueType = *(uint16_t*)(ddata + dheader->offset1 + itemId * 2);
                    uint8_t implementationType = *(uint8_t*)(ddata + dheader->offset3 + valueType);
                    uint8_t value_type_value2 = *(uint32_t*)(ddata + dheader->offset2 + 4 * implementationType);
                    bool isString = false;
                    if ((valueType == 22) && (implementationType == 1) && (value_type_value2 == 7))
                        isString = true;
					if ((valueType == 23) && (implementationType == 1) && (value_type_value2 == 7))
						isString = true;
                    if ((valueType == 3) && (implementationType == 1) && (value_type_value2 == 7))
                        isString = true;
                    if ((valueType == 21) && (implementationType == 1) && (value_type_value2 == 7))
                        isString = true;
                    if ((valueType == 46) && (implementationType == 1) && (value_type_value2 == 7))
                        isString = true;

                    if (isString)
                    {
                        uint32_t* value_offset = (uint32_t*)(data + item_header_offset + header->offset12);
                        char* value = (char*)(data + header->offset12 + *value_offset + item_header_offset);
                        if (strcmp(value, item_new_value) == 0)
                        {
                            printf("%u is not changed - skipping\r\n", item_to_replace);
                        }
                        else
                        {
                            num_changed_strings++;
                            if (strlen(value) <= strlen(item_new_value))
                            {
                                //we can just replace in file
                                strcpy(value, item_new_value);
                            }
                            else
                            {
								uint32_t absolute_offset = (value - (char*)data);
								uint32_t absolute_offset_new = header->file_size + new_string_buffer_size;
								//adjust offset
								*value_offset += absolute_offset_new - absolute_offset;
								//add new string to buffer
								strcpy(new_string_buffer + new_string_buffer_size, item_new_value);
								new_string_buffer_size += strlen(item_new_value) + 1;
                            }
                        }
                    }
                    else
                    {
                        printf("%u is not a string - skipping\r\n", item_to_replace);
                    }
                    //found/processed item
                    break;
                }
            }
        }
        fclose(f);

        if (!num_changed_strings)
        {
            printf("No strings edited - no new output file\r\n");
        }
        else
        {
            int original_size = header->file_size;
            header->file_size += new_string_buffer_size;
            f = fopen("datapool_out.bvf", "wb");
            if (!f)
            {
                printf("Could not open output file\r\n");
                return 5;
            }
            fwrite(data, 1, original_size, f);
            if(new_string_buffer_size)
                fwrite(new_string_buffer, 1, new_string_buffer_size, f);
            fclose(f);
            printf("all done\r\n");
        }

        delete[]data;
        delete[] ddata;
    }
    

    return 0;
}
