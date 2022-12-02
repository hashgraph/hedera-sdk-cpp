/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include "helper/HexConverter.h"
#include "openssl/crypto.h"

#include <iomanip>
#include <iostream>
#include <sstream>

namespace Hedera {
    std::string HexConverter::base64ToHex(const std::vector<unsigned char> &bytes) {
        size_t stringLength;

        if (OPENSSL_buf2hexstr_ex(nullptr, 0, &stringLength, &bytes.front(), bytes.size(), '\0') <= 0) {
            std::cout << "buf2hexstr_ex determine size error" << std::endl;
        }

        std::unique_ptr<char> charArray(new char[stringLength]);
        std::string charString;
        charString.reserve(stringLength);

        if (OPENSSL_buf2hexstr_ex(charArray.get(), stringLength, &stringLength, &bytes.front(), bytes.size(), '\0') <=
            0) {

            std::cout << "buf2hexstr_ex generate string error" << std::endl;
        }

        charString.append(charArray.get());

        return charString;
    }

    std::vector<unsigned char> HexConverter::hexToBase64(const std::string &inputString) {
        size_t bufferLength;
        if (OPENSSL_hexstr2buf_ex(nullptr, 0, &bufferLength, inputString.c_str(), '\0') <= 0) {
            std::cout << "hexstr2buf_ex determine size error" << std::endl;
        }

        std::vector<unsigned char> outputBytes = std::vector<unsigned char>(bufferLength);

        if (OPENSSL_hexstr2buf_ex(&outputBytes.front(), bufferLength, &bufferLength, inputString.c_str(), '\0') <= 0) {
            std::cout << "hexstr2buf_ex generate string error" << std::endl;
        }

        return outputBytes;
    }

    std::string HexConverter::bytesToHex(const std::vector<unsigned char> &bytes) {
        std::stringstream stream;
        stream << std::hex;

        for (unsigned char byte: bytes) {
            stream << std::setw(2) << std::setfill('0') << (int) byte;
        }

        return stream.str();
    }
}
