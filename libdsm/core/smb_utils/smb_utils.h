//
//  smb_utils.h
//  libdsm
//
//  Created by trekvn on 4/4/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#ifndef smb_utils_h
#define smb_utils_h

#include <stdio.h>
#include <stdint.h>

/*!Converts a string from current locale encoding to UCS2-LE
 * It allocates the output string on the heap, you have to free it !
 *\param[in] src The input string. It is considere to be in the current locale
 *\param[in] src_len The length in byte of the input string (strlen will do
 * the job for utf-8 or single byte encoding)
 *\param[out] dst Will be set to the newly allocated encoded string, you have
 * to free() it after usage unless the function returns 0, in that case, it'll
 * be set to NULL
 *\returns The size of the encoded string in bytes
 */
size_t smb_to_utf16(const char *src, size_t src_len, char **dst);

/*!Converts from UCS2-LE to local encoding (fetched using setlocale())
 * It returns a newly allocated string that you need to free
 * after usage
 *\param[in] src The UCS2-LE string to be converved to local encoding
 *\param[in] src_len The size in bytes of src
 *\param[out] dst will be set to the newly allocated and converted string.
 * You have to free() it unless the function returns 0, in that case, it'll
 * be set to NULL
 *\returns The size of the decoded string in bytes
 */
size_t      smb_from_utf16(const char *src, size_t src_len, char **dst);

#endif /* smb_utils_h */
