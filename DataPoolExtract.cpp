// DataPoolExtract.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <windows.h>
#include <stdint.h>

struct Datapool_bvf
{
    uint32_t fixed_header[4];//@0
    uint32_t some_fixed_value[4];//@16
    uint32_t foofoo;//@32
    uint32_t should_be_1;//@36
    uint32_t file_size;//@40
    uint32_t should_be_ffff0000;//@44
    uint32_t NumberOfVariants;//@48
    uint32_t main_offset;//@52
    uint32_t groupIdOffset;//@56
    uint32_t variantIdOffset;//@60
    uint32_t variantNameOffset;//@64
    uint32_t variantLabelOffset;//@68
    uint32_t languageIdOffset;//@72
    uint32_t languageNameOffset;//@76
    uint32_t languageLabelOffset;//@80
    uint32_t languageTagOffset;//@84
    uint32_t offset_item_headers;
    uint32_t offset_items_per_lang;//items per language
    uint32_t item_id_offset;
    uint32_t offset12;
};

struct Datapool_bdf
{
    uint32_t fixed_header[4];//16 bytes fixed header - 52h, 0Dh, 68h, 0E0h, 5Eh, 20h, 44h, 0E3h, 9Eh, 0F8h, 0F0h, 61h, 87h, 0E0h, 10h, 1Bh
    uint32_t uint8_ram_size_maybe;
    uint32_t negative_one;
    uint32_t itemId;
    uint32_t field_1C;
    uint32_t should_be_ff00ff00;//@32
    uint32_t fileVersion;//should be 4 @36
    uint32_t file_size;//@40
    uint32_t numOfContexts;//@44
    uint32_t numOfItemDescriptors;//@48
    uint32_t m_numOfItems;//@52
    uint32_t numOfIoItems;//@56

    uint32_t pItemDescriptor;//uint16 @60
    uint32_t pIDT_ValueType;//unsigned int @64
    uint32_t pIDT_ImplType;//unsigned char @68
    uint32_t pIDT_ModeFlags;//unsigned char @72
    uint32_t pIDT_Group;//unsigned int @76
    uint32_t pIDT_ItemOffset;//unsigned int @80
    uint32_t pIDT_ReaderDescriptor;//unsigned char @84
    uint32_t pIDT_WriterDescriptor;//unsigned char @88
    uint32_t pCDT_Identifiers;//unsigned char @92
    uint32_t pCDT_AccessMap;//unsigned int @96
    uint32_t pCDT_AccessMapLength;//unsigned int @100
    uint32_t pCDT_NotifyMap;//unsigned int @104
    uint32_t pCDT_NotifyMapLength;//unsigned int @108

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

    uint16_t *pItemDescriptor;
    uint32_t *pIDT_ValueType;
    uint8_t* pIDT_ImplType;
    uint8_t* pIDT_ModeFlags;
    uint32_t* pIDT_Group;
    uint32_t* pIDT_ItemOffset;
    uint8_t* pIDT_ReaderDescriptor;
    uint8_t* pIDT_WriterDescriptor;
    uint8_t* pCDT_Identifiers;
    uint32_t* pCDT_AccessMap;
    uint32_t* pCDT_AccessMapLength;
    uint32_t* pCDT_NotifyMap;
    uint32_t* pCDT_NotifyMapLength;

    uint8_t* bdf_data_at_offset14;

	uint32_t numOfContexts;
	uint32_t numOfItemDescriptors;
	uint32_t m_numOfItems;
	uint32_t numOfIoItems;
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
        dloader.pItemDescriptor = (uint16_t*)(ddata + dheader->pItemDescriptor);
        dloader.pIDT_ValueType = (uint32_t*)(ddata + dheader->pIDT_ValueType);
        dloader.pIDT_ImplType = ddata + dheader->pIDT_ImplType;
        dloader.pIDT_ModeFlags = ddata + dheader->pIDT_ModeFlags;
        dloader.pIDT_Group = (uint32_t*)(ddata + dheader->pIDT_Group);
        dloader.pIDT_ItemOffset = (uint32_t*)(ddata + dheader->pIDT_ItemOffset);
        dloader.pIDT_ReaderDescriptor = ddata + dheader->pIDT_ReaderDescriptor;
        dloader.pIDT_WriterDescriptor = ddata + dheader->pIDT_WriterDescriptor;
        dloader.pCDT_Identifiers = ddata + dheader->pCDT_Identifiers;
        dloader.pCDT_AccessMap = (uint32_t*)(ddata + dheader->pCDT_AccessMap);
        dloader.pCDT_AccessMapLength = (uint32_t*)(ddata + dheader->pCDT_AccessMapLength);
        dloader.pCDT_NotifyMap = (uint32_t*)(ddata + dheader->pCDT_NotifyMap);
        dloader.pCDT_NotifyMapLength = (uint32_t*)(ddata + dheader->pCDT_NotifyMapLength);
        dloader.bdf_data_at_offset14 = ddata + dheader->offset14;

        dloader.numOfContexts = dheader->numOfContexts;
        dloader.numOfItemDescriptors = dheader->numOfItemDescriptors;
        dloader.m_numOfItems = dheader->m_numOfItems;
        dloader.numOfIoItems = dheader->numOfIoItems;

        for (uint32_t item_idx = 0; item_idx < header->NumberOfVariants; item_idx++)
        {
            uint32_t val1 = *(uint32_t*)(data + header->groupIdOffset + item_idx * 4);

            uint32_t tmp = *(uint32_t*)(data + header->variantLabelOffset + item_idx * 4);

            uint32_t val2 = *(uint32_t*)(data + header->main_offset + tmp);

            uint32_t val3 = *(uint32_t*)(data + header->variantIdOffset + item_idx * 4);

            tmp = *(uint32_t*)(data + header->variantNameOffset + item_idx * 4);
            char* val4 = (char*)(data + header->main_offset + tmp);

            tmp = *(uint32_t*)(data + header->variantLabelOffset + item_idx * 4);
            uint32_t val5 = *(uint32_t*)(data + header->main_offset + tmp);

            uint32_t val6 = *(uint32_t*)(data + header->variantIdOffset + item_idx * 4);

            tmp = *(uint32_t*)(data + header->variantLabelOffset + item_idx * 4);
            char* val7 = (char*)(data + header->main_offset + tmp);

            tmp = *(uint32_t*)(data + header->languageLabelOffset + item_idx * 4);

            uint32_t val8 = *(uint32_t*)(data + header->main_offset + tmp);

            uint32_t val9 = *(uint32_t*)(data + header->languageIdOffset + item_idx * 4);

            tmp = *(uint32_t*)(data + header->languageNameOffset + item_idx * 4);
            char* val10 = (char*)(data + header->main_offset + tmp);

            tmp = *(uint32_t*)(data + header->languageLabelOffset + item_idx * 4);
            char* val11 = (char*)(data + header->main_offset + tmp);

            tmp = *(uint32_t*)(data + header->languageTagOffset + item_idx * 4);
            char* val12 = (char*)(data + header->main_offset + tmp);

            printf("item %u: %u, %u, %u, %s, %u, %u, %s, %u, %u, %s, %s, %s\r\n", item_idx, val1, val2, val3, val4, val5, val6, val7, val8, val9, val10, val11, val12);
        }

        for(uint32_t lang_index = 0; lang_index < header->NumberOfVariants; lang_index++ )
        {
            uint32_t items_in_this_type = *(uint32_t*)(data + header->offset_items_per_lang + lang_index * 4);

            if (!items_in_this_type)
                continue;

            const char* currentLang = (char*)(data + header->main_offset + *(uint32_t*)(data + header->languageNameOffset + lang_index * 4));
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
                if (1)
                {
                    uint16_t itemDescriptor = dloader.pItemDescriptor[itemId];
                    uint8_t implementationType = dloader.pIDT_ImplType[itemDescriptor];
                    uint8_t valueType = dloader.pIDT_ValueType[itemDescriptor];

                    uint32_t value_offset = *(uint32_t*)(data + item_header_offset + header->offset12);

                    switch (valueType)
                    {
                    case 11://string						
                        {
							char* value = (char*)(data + header->offset12 + value_offset + item_header_offset);
							if (strlen(value) && !strchr(value, '_') && !strstr( value, "LATE") && !strstr(value, "TRANSLAT" ) && !strstr(value, "translat") && !strstr( value, "Xlate" ) && !strstr( value, "x'Late" ) && !strstr( value, "Font:" ) && !strstr( value, "Width:") )
								fprintf(f, "%u: %s\r\n", itemId, value);

                            //printf("string id: %u itemDescriptor: %u implementationType: %u value type: %u\r\n", itemId, itemDescriptor, implementationType, valueType);
                            //printf("string id: %u %s\r\n", itemId, value);
                        }
                        break;
                    case 26://string list
                        //printf("string list id: %u\r\n", itemId, itemDescriptor, implementationType, valueType);
                        //hex_dump(data + header->offset12 + value_offset + item_header_offset, 20);
                        {
                            uint32_t* dataPtr = (uint32_t*)(data + header->offset12 + value_offset + item_header_offset);
                            uint32_t itemCount = *dataPtr++;
                            dataPtr += *dataPtr / 4;
                            for (uint32_t listItem = 0; listItem < itemCount; listItem++)
                            {
                                char* value = (char*)dataPtr + *dataPtr;
                                dataPtr++;
								//printf("%u.%u: %s\r\n", itemId, listItem, value);

                                if (strlen(value) && !strchr(value, '_') && !strstr(value, "LATE") && !strstr(value, "TRANSLAT") && !strstr(value, "translat") && !strstr(value, "Xlate") && !strstr(value, "x'Late") && !strstr(value, "Font:") && !strstr(value, "Width:"))
									fprintf(f, "%u.%u: %s\r\n", itemId, listItem, value);
                            }
                        }
                        break;
                    default:
						printf("item id: %u itemDescriptor: %u implementationType: %u value type: %u\r\n", itemId, itemDescriptor, implementationType, valueType);
                    }
                }
                item_header_offset += 4;
            }

            //lang_index++;

            fclose(f);
        }

        delete[]data;
        delete[] ddata;
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
        dloader.pItemDescriptor = (uint16_t*)(ddata + dheader->pItemDescriptor);
        dloader.pIDT_ValueType = (uint32_t*)(ddata + dheader->pIDT_ValueType);
        dloader.pIDT_ImplType = ddata + dheader->pIDT_ImplType;
        dloader.pIDT_ModeFlags = ddata + dheader->pIDT_ModeFlags;
        dloader.pIDT_Group = (uint32_t*)(ddata + dheader->pIDT_Group);
        dloader.pIDT_ItemOffset = (uint32_t*)(ddata + dheader->pIDT_ItemOffset);
        dloader.pIDT_ReaderDescriptor = ddata + dheader->pIDT_ReaderDescriptor;
        dloader.pIDT_WriterDescriptor = ddata + dheader->pIDT_WriterDescriptor;
        dloader.pCDT_Identifiers = ddata + dheader->pCDT_Identifiers;
        dloader.pCDT_AccessMap = (uint32_t*)(ddata + dheader->pCDT_AccessMap);
        dloader.pCDT_AccessMapLength = (uint32_t*)(ddata + dheader->pCDT_AccessMapLength);
        dloader.pCDT_NotifyMap = (uint32_t*)(ddata + dheader->pCDT_NotifyMap);
        dloader.pCDT_NotifyMapLength = (uint32_t*)(ddata + dheader->pCDT_NotifyMapLength);
        dloader.bdf_data_at_offset14 = ddata + dheader->offset14;

        dloader.numOfContexts = dheader->numOfContexts;
        dloader.numOfItemDescriptors = dheader->numOfItemDescriptors;
        dloader.m_numOfItems = dheader->m_numOfItems;
        dloader.numOfIoItems = dheader->numOfIoItems;

        uint32_t lang_to_replace = _stricmp(argv[1], "Default") == 0 ? 0 : - 1;

        for (uint32_t item_idx = 1; item_idx < header->NumberOfVariants; item_idx++)
        {
            uint32_t val1 = *(uint32_t*)(data + header->groupIdOffset + item_idx * 4);

            uint32_t tmp = *(uint32_t*)(data + header->variantLabelOffset + item_idx * 4);

            uint32_t val2 = *(uint32_t*)(data + header->main_offset + tmp);

            uint32_t val3 = *(uint32_t*)(data + header->variantIdOffset + item_idx * 4);

            tmp = *(uint32_t*)(data + header->variantNameOffset + item_idx * 4);
            char* val4 = (char*)(data + header->main_offset + tmp);

            tmp = *(uint32_t*)(data + header->variantLabelOffset + item_idx * 4);
            uint32_t val5 = *(uint32_t*)(data + header->main_offset + tmp);

            uint32_t val6 = *(uint32_t*)(data + header->variantIdOffset + item_idx * 4);

            tmp = *(uint32_t*)(data + header->variantLabelOffset + item_idx * 4);
            char* val7 = (char*)(data + header->main_offset + tmp);

            tmp = *(uint32_t*)(data + header->languageLabelOffset + item_idx * 4);

            uint32_t val8 = *(uint32_t*)(data + header->main_offset + tmp);

            uint32_t val9 = *(uint32_t*)(data + header->languageIdOffset + item_idx * 4);

            tmp = *(uint32_t*)(data + header->languageNameOffset + item_idx * 4);
            char* val10 = (char*)(data + header->main_offset + tmp);

            tmp = *(uint32_t*)(data + header->languageLabelOffset + item_idx * 4);
            char* val11 = (char*)(data + header->main_offset + tmp);

            tmp = *(uint32_t*)(data + header->languageTagOffset + item_idx * 4);
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
                char* atmp = strchr(lang_string, '.');
                uint32_t listIndex = 0xFFFFFFFF;
                if (atmp)
                {
                    listIndex = atol(atmp + 1);
                }
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

					uint16_t itemDescriptor = dloader.pItemDescriptor[itemId];
					uint8_t implementationType = dloader.pIDT_ImplType[itemDescriptor];
					uint8_t valueType = dloader.pIDT_ValueType[itemDescriptor];

					uint32_t *value_offset = (uint32_t*)(data + item_header_offset + header->offset12);

                    switch (valueType)
                    {
                    case 11://string						
                        {
                            char* value = (char*)(data + header->offset12 + *value_offset + item_header_offset);

                            if (strlen(value) && !strchr(value, '_'))
                            {
                                if (strcmp(value, item_new_value) == 0)
                                {
                                    printf("%u is not changed - skipping\r\n", item_to_replace);
                                }
                                else
                                {
                                    num_changed_strings++;
                                    if (strlen(value) >= strlen(item_new_value))
                                    {
                                        //we can just replace in file
                                        printf("replacing\n%s\nwith\n%s\n", value, item_new_value);
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
                        }
                    break;
                    case 26://string list
                        {
                            uint32_t* dataPtr = (uint32_t*)(data + header->offset12 + *value_offset + item_header_offset);
                            uint32_t itemCount = *dataPtr++;
                            dataPtr += *dataPtr / 4;
                            for (uint32_t listItem = 0; listItem < itemCount; listItem++)
                            {
                                char* value = (char*)dataPtr + *dataPtr;

                                if (listItem == listIndex)
                                {
                                    //replacing this item
                                    if (strlen(value) && !strchr(value, '_'))
                                    {
                                        if (strcmp(value, item_new_value) == 0)
                                        {
                                            printf("%u.%u is not changed - skipping\r\n", item_to_replace, listIndex);
                                        }
                                        else
                                        {
                                            num_changed_strings++;
                                            if (strlen(value) >= strlen(item_new_value))
                                            {
                                                //we can just replace in file
                                                printf("replacing\n%s\nwith\n%s\n", value, item_new_value);
                                                strcpy(value, item_new_value);
                                            }
                                            else
                                            {
                                                uint32_t absolute_offset = (value - (char*)data);
                                                uint32_t absolute_offset_new = header->file_size + new_string_buffer_size;
                                                //adjust offset
                                                *dataPtr += absolute_offset_new - absolute_offset;

                                                //add new string to buffer
                                                strcpy(new_string_buffer + new_string_buffer_size, item_new_value);
                                                new_string_buffer_size += strlen(item_new_value) + 1;
                                            }
                                        }
                                    }
                                }

                                dataPtr++;
                            }
                        }
                    break;
                    }
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
