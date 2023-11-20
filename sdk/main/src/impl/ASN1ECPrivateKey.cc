/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License")
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

#include "exceptions/BadKeyException.h"

#include "impl/ASN1ECPrivateKey.h"
#include "impl/HexConverter.h"

namespace Hedera::internal::asn1
{
    ASN1ECPrivateKey::ASN1ECPrivateKey(const std::vector<std::byte>& bytes)
    {
        decode(bytes);
    }

    const std::vector<std::byte> ASN1ECPrivateKey::getKey() const
    {   
        std::vector<std::byte> privateKey = get(OCTET_STRING);
        if(privateKey.size() > ECDSA_KEY_LENGTH) // remove redundant padded bytes if any
            privateKey = std::vector<std::byte>(privateKey.end() - ECDSA_KEY_LENGTH, privateKey.end());

        return privateKey;
    }

    void ASN1ECPrivateKey::print() const
    {
        for(auto entry : asn1KeyData)
        {
            std::cout<<internal::HexConverter::bytesToHex({entry.first}) 
                << " : " << internal::HexConverter::bytesToHex(entry.second)<<std::endl;
        }
    }

    void ASN1ECPrivateKey::decode(const std::vector<std::byte>& bytes)
    {
        int currentByteIndex = 0;
        while(currentByteIndex < bytes.size() - 1)
        {
            std::byte asn1Tag = bytes[currentByteIndex++];
            int asn1TagSize = static_cast<int>(bytes[currentByteIndex++]);

            // Ignore sequence as ASN1 for EC Key is in basic format
            // TODO: Add further ignores for ASN1 tags so that we don`t
            // populate the map data with useless values
            if(asn1Tag == SEQUENCE) continue; 

            std::vector<std::byte> asn1DataAtTag(bytes.begin() + currentByteIndex, bytes.begin() + currentByteIndex + asn1TagSize);
            currentByteIndex += asn1TagSize;

            asn1KeyData.insert({asn1Tag, asn1DataAtTag});
        }
    }

    const std::vector<std::byte> ASN1ECPrivateKey::get(const std::byte tag) const
    {
        auto entry = asn1KeyData.find(tag);
        if(entry != asn1KeyData.end())
            return entry->second;
        else throw BadKeyException("Data not decoded properly for input PEM/DER EC KEY bytes!");
    }

} // namespace Hedera::internal:asn1
