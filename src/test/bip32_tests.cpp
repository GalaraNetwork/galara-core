// Copyright (c) 2013-present The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/test/unit_test.hpp>

#include <clientversion.h>
#include <key.h>
#include <key_io.h>
#include <streams.h>
#include <test/util/setup_common.h>
#include <util/strencodings.h>

#include <string>
#include <vector>

namespace {

struct TestDerivation {
    std::string pub;
    std::string prv;
    unsigned int nChild;
};

struct TestVector {
    std::string strHexMaster;
    std::vector<TestDerivation> vDerive;

    explicit TestVector(std::string strHexMasterIn) : strHexMaster(strHexMasterIn) {}

    TestVector& operator()(std::string pub, std::string prv, unsigned int nChild) {
        vDerive.emplace_back();
        TestDerivation &der = vDerive.back();
        der.pub = pub;
        der.prv = prv;
        der.nChild = nChild;
        return *this;
    }
};

TestVector test1 =
  TestVector("000102030405060708090a0b0c0d0e0f")
    ("Gpub1QRVXbbsAA1mw1pHsNY9zv76yY16Rp1AybFzm9zw7uJafaJTZocV9i2Y3div259vRfWndsyYiwCok9GBAS9mH2L1VVZv6xSKdwqCyXXDjwJ",
     "Gprv5BS9864yKnTUiXjpmM19dnANRWAc2MHKcNLPxmbKZZmbnmyK2GJEbui4CN5cWBjrW8cr8vxQ5drVwd52sCK7ezKvhnqgwRvisqETwG4QBVj",
     0x80000000)
    ("Gpub1SguXJuLFR8m5cx158QnsxDDJMZSotVBfCgtjFP466Vs2rbdz39aGrNwpuHcmP4TRsoMW7WyTaGtiYEK7Tt9Lunjh8NB2GLY9sxmHBZ4g5T",
     "Gprv5DhZ7oNSR3aTs8sXy6snWpGUkKixQRmLHymHvrySXkxtA4GVSVqKj44TydRwuu7pDBWYWEHGTzhNfP6kWnbJT4QSmfWqBPdcaqsKNRRBjhu",
     1)
    ("Gpub1Us2j6TDe81pv4zSoPJfUh6sMjYYovDUbXMjKX9s2jJ5GQ6mhqZtERHBEykCBKmoPbRQHXy5aAmNJqy8ao432ZcfzLD2VuLoncVYaACp2NA",
     "Gprv5FsgKavKokTXhauyhMmf7ZA8ohi4QTVdEJS8X8kFUPm6PbmdAJFdgcxhPgd1WptrfGGEK3shYhenoKcye8PHEDttHSUZF9QUFeBguH428Bi",
     0x80000002)
    ("Gpub1XUJmdH5LzsEnEnY3yi2qh91atR55ToTw4bQZrT2CbfWgFHtWbQK8rq6nzw2dCD7k7X8tuwgx7LLmB7RHtba61mWaEBAarQVWJQgfKuS4xP",
     "Gprv5JUxN7kBWdJwZki4wxB2UZCH2raag15cZqfomU3QeG8XoSxjy464b4WcwiqdV4QMAjHcuWpcy7Uh2XMjE9JUMKLYe437Q5eQruvuXqzQE4f",
     2)
    ("Gpub1Zhhc4Q2XUYDriGFkEv7jCJKfhhoi47fNYUhpoyVX3E83LvFaN6gLSJ2UKvR2zMrDReK2vRdzrETKbTRPC2mocEmV1jMR3JKWzKXowQKP5p",
     "Gprv5LiMCYs8h6yveEBneDP7N4Mb7fsKJbPp1KZ72RZsxhh9AYb72pnRndyYd3F5wionEXRKXS4BsvvrViGBRZqpYhvb3J8CxAU81tQ53So6SQ1",
     1000000000)
    ("Gpub1bRU5k1GebvRP43hrnUr2uJKTwH7XYvhAnsG56Mdj7cckUKe6nMLbYT679qMtFSZh7vrYGMSY3w3v1rSV3XcesPtMZ9SDWsxKwwT49FheCP",
     "Gprv5NS7gEUNpEN8AZyEkkwqfmMauuSd86CqoZwfGhx2An5dsfzVZF363k8cFu2HrdvrqvpaJo38iyTyVtGYxdrYffa8NntS55DqJDxRnVjMP7s",
     0);

TestVector test2 =
  TestVector("fffcf9f6f3f0edeae7e4e1dedbd8d5d2cfccc9c6c3c0bdbab7b4b1aeaba8a5a29f9c999693908d8a8784817e7b7875726f6c696663605d5a5754514e4b484542")
    ("Gpub1QRVXbbsAA1mvdKcyXcST7psDj9X7SFzfB3j46jm6HRSxZKejxPRacC8PjotjKWtLDkaYYdi81i4TvzQHd4vQwKxMRmNwSvyxKWKFVZyxjk",
     "Gprv5BS9864yKnTUi9F9sW5S5yt8fhK2hyY9Hx88FiL9XwtU5kzWCR5B2oseYRrfghHwkBNGtDQrMjth39o8uchpGMehACuw5mqTT5rhJ9ybcdi",
     0)
    ("Gpub1ThEoKHpeAqrStuv7JzKWvEPagtmpQkHDLo6DumDAhHQeXmdKDjVjSmNyCmnXQXKYYEkQmdTX6Z2d6R6AmmLkXzEdUW3qo5N5ocygjFixzA",
     "Gprv5EhtPokvooHZEQqT1HTK9nHf2f4HQx2Rr7sVRXMbcMkRmjSUmgRFBeSu7w8UKm8n9qc6nDpanuJpPfRxTuCsCap6APKzUG1CaVmFfTGx8EW",
     0xFFFFFFFF)
    ("Gpub1UrJ3vKM251xcm86XkyxzFuwyS7gMtuyJVsd9JfypS3o6q15w79owWYyab1uH98RL4AJ4cV4pJydHmHRwgFTUgSsAZ87CXLNxBtaXTPcPjY",
     "Gprv5FrweQnTBhTfQH3dRjSxd7yDRQHBxSC7wGx2LvGNG6WpE2fwPZqZPiEVjHcT41sG44vzZF2DquovpuAjCGozmtFeb7ZLEmjTNH6nRqJEBbK",
     1)
    ("Gpub1XfGTwJHBiK9iNoEbR8XxCrBJxzBU5qEcwNUzNLK8KQEmesFEcgWqQwMRPD351phvEcquFaJTSHTNZYSragQk1umkqKxR4masxZXmsfeens",
     "Gprv5Jfv4RmPMLkrVtimVPbXb4uSkw9h4d7PFiStByvhZysFtrY6h5NGHccsa5p55j4xN2HhJY6cFTV99bwtFALNnN4wit695shqr5XWuX379HF",
     0xFFFFFFFE)
    ("Gpub1YqJNuEdo7Gs1L2qKtx5KiYNDhTYX4Tjee6NX62iy5fVE1CBLHcXMwJT43LYGPM9sgGUhke1W3cK2ChqYTnvTNcE2jeSyMrzkBe7JtSwxvB",
     "Gprv5KqwyPhjxjiZnqxNDsR4xabdffd47bjtHRAmihd7Qk8WMCs2nkJGp8yyCnYTi9TEubaPcMsbdq4ixj3Fm7EsUCk7YpdTE5jUBfY2yLk75Nn",
     2)
    ("Gpub1aCLLLT9KHb6kbDj3y5mJTvLckzemPNpJoiFvsPDbsX5JCXkVvYHYWFT8SQpPxhrPjUhyg9RPkdJWdmLNZUpmdTofuUXuj9jRUqHyWwL2wB",
     "Gprv5MCyvpvFUv2oY79FwwYkwKyc4jAAMvexwanf8Uyc3Xz6RQCbxPE2zhvyHCDTsAYJwuS7u7WR5vuR1GvmVRc6XBUqMg8xHVsBt2sb3tHLDNS",
     0);

TestVector test3 =
  TestVector("4b381541583be4423346c643850da4b320e46a87ae3d2a4e6da11eba819cd4acba45d239319ac14f863b8d5ab5a0d0c64d2e8a1e7d1457df2e5a3c51c73235be")
    ("Gpub1QRVXbbsAA1mugmnVv7Ef75u6NUcsJRGNWpHTPmKMyWEvoYvQ6dPXsResjLsYv6XWJWSNYWiWgXuuEdK8L7MqGz6Uffbg1Npk321mzCVUCJ",
     "Gprv5BS9864yKnTUhChKPtaEHy9AYLe8TqhR1Htgf1MhodyG41DmrZK8z57B2RaqdiXGpt32T96xGAdQPoQT93rF3q9wgVUaHDBRjoX3x8uaQ67",
      0x80000000)
    ("Gpub1SnhGZRzqXP87DsBj4SjiMqZnj6129qAn14EGdTAJHRcBpgNybQFcLDNBTBPHGhGhg1H9WGTMdjQXhZf4a3YQXG39mcVDrsdEDNvSJN9xAQ",
     "Gprv5DoLs3u719pptjnid2ujMDtqEhFWch7KQn8dUF3YjwtdK2MES4614XttLBwByfrCH2gSU5tpuuFs3M3biGRzobMa7dPd2NTErpUhuZ32jEE",
      0);

TestVector test4 =
  TestVector("3ddd5602285899a946114506157c7997e5444528f3003f6134712147db19b678")
    ("Gpub1QRVXbbsAA1mwkHMLeU3TrYnic4nnYq1ceWgq8SuEWLvmhNFc1E9k5ykZrbYaMgb5qdzXDjuTTJei7QSYY8Gv8r1F5xgEKLdedhErkZNaqu",
     "Gprv5BS9864yKnTUjGCtEcw36ic4AaEJP67AFRb62k3HgAowtu374TuuCHfGiarxeSvTQLzeZKz6CVqB2p2jo97A8epLBjLcL6561wPCiF4H81Q",
     0x80000000)
    ("Gpub1TabuyiXXv8DadJUxyPpUFeZAXx4VrMZMx8MB6cssxQuGERNALest8f6jSsJbz3iVwQMUG8X4PEuF36WFKag1Su3HQPe78S1hSFHfDWuNEU",
     "Gprv5EbFWUBdhYZvN9E1rwrp77hpcW7a6PdhzjCkNiDGKcsvPS6DcoLdLLLct8nChG6pNL265uAKWwwFyUDqQWmo9HhG37eHY1X61CR1T88pMbh",
     0x80000001)
    ("Gpub1ViHZy7Y3eApFnA87FJia8EQLU88zn4N1tBAwr95Y9WM3Kjt2q8b7FcydPdqTAgQ5j1iN34dWqatDXRQhvPpijqQdCEh4ruxZrcBigzfJWF",
     "Gprv5GiwATaeDGcX3J5f1DmiCzHfnSHebKLWefFa9TjTyoyNAXQjVHpLZTJVn6kHkf8eT6RCGVzHHb38gS3YoSivRgAVHTMJe3mQrsxhYjFAXQv",
     0);

const std::vector<std::string> TEST5 = {
    "Gpub1QRVXbbsAA1mufikNQBx9FZW1TPXCCoZZrqfKG3tdqjzYU2zKAGtkrtKawmVkwVgA5LsEawPurYKssaUTQWKrBrmbAeDpdAkmAsHQk9si3D",
    "Gprv5BS9864yKnTUhBeHGNewn7cmTRZ2nk5iCdv4WseH5WD1ffhqmcxeD4Zqjt35z2TiJvauqXKuiQzmnJpt5xrpouaoVxKnk7RjedWweMokWio",
    "Gpub1QRVXbbsAA1mufikNQBx9FZW1TPXCCoZZrqfKG3tdqjzYU2zKAGtkrtKb5YUTsHoZn3mmsGDYB2Kh9ya7ryCT1EGjiveoDvLjntqPJ45zuU",
    "Gprv5BS9864yKnTUhBeHGNewn7cmTRZ2nk5iCdv4WseH5WD1ffhqmcxeD4ZqjtQCNh9DA3bmsR9HsRGs5iRnECUeh9e7DicGwu7How1bdBrKNjD",
    "Gpub1QRVXbbsAA1mufikNQBx9FZW1TPXCCoZZrqfKG3tdqjzYU2zKAGtkrtKayiEwAwi1W26NQX6pSAKqCBFNX83VtheNoiL4mrekpsfuNe2eTd",
    "Gprv5BS9864yKnTUhBeHGNewn7cmTRZ2nk5iCdv4WseH5WD1ffhqmcxeD4ZqjnZxqzo7bma6TxQB9gQsDkdTUrdVk37UroPxDT3bpxzS9HL9eHh",
    "Gprv5BSZwpZ4isummPkCvduzxZ311rRXY2orguy9DFgaCypyrzqFtwHJfWguAK92mcg8NvsPdp6UZCMVFQVMMYYKB9AXXB4q1QExk5fmRAXU76y",
    "Gpub1QRvML5xZFU4yspg2fT1KgyjZtG1wVXi48tk1e6BmKMxjoAQSUbZDK1P1dhBB3wMMF1S3eiqhGJrNiqLmRsuea4tTyP9dY83VG5uApYkbnC",
    "Gprv5BS9864yPaUdYqdYQdCT3uJNtHAbKbWGbLaaHJm6arFurpWx6r9tnkc5TwdXT31ZGF5TQYRbFjYoR7JDKRLWvwjps1uN5DRNs8ED7RNYoBP",
    "Gpub1QRVXbbsDx2vmKi1WejTR3F7SK15j4E7xZWB5hAi9Bntjcr6ePU9LYvZKGBfrUGnEZDVpP3xPoWAYReCjJg7QNeBopDghMJTcJeLs7ERXLT",
    "DMwo58pR1QLEFihHiXPVykYB6fJmsTeHvyTp7hRThAtCX8CvYzgPcn8XnmdfHGMQzT7ayAmfo4z3gY5KfbrZWZ6St24UVf2Qgo6oujFktLHdHY4",
    "DMwo58pR1QLEFihHiXPVykYB6fJmsTeHvyTp7hRThAtCX8CvYzgPcn8XnmdfHPmHJiEDXkTiJTVV9rHEBUem2mwVbbNfvT2MTcAqj3nesx8uBf9",
    "Gprv5BS9864yKnTUhBeHGNewn7cmTRZ2nk5iCdv4WseH5WD1ffhqmcxeD4ZqjkdDfmM5kLtsL8pUF6nsGS2gZk1n6LGc5AKqyJMhqJz3dQ82JWW",
    "Gprv5BS9864yKnTUhBeHGNewn7cmTRZ2nk5iCdv4WseH5WD1ffhqmcxeD4ZqjnZxqzo7bma6TxQB9gQsDkdNwzGbD1rhQ68xxbLH8fy4McmjfvS",
    "Gpub1QRVXbbsAA1mufikNQBx9FZW1TPXCCoZZrqfKG3tdqjzYU2zKAGtkrtKb1ez7QPjrvhKWE6oj1nKnWn2Hdjm9bYXASnSJvYYkUt4PQSoB7D",
    "xprv9s21ZrQH143K3QTDL4LXw2F7HEK3wJUD2nW2nRk4stbPy6cq3jPPqjiChkVvvNKmPGJxWUtg6LnF5kejMRNNU3TGtRBeJgk33yuGBxrMPHL"
};

void RunTest(const TestVector& test)
{
    std::vector<std::byte> seed{ParseHex<std::byte>(test.strHexMaster)};
    CExtKey key;
    CExtPubKey pubkey;
    key.SetSeed(seed);
    pubkey = key.Neuter();
    for (const TestDerivation &derive : test.vDerive) {
        unsigned char data[74];
        key.Encode(data);
        pubkey.Encode(data);

        // Test private key
        BOOST_CHECK(EncodeExtKey(key) == derive.prv);
        BOOST_CHECK(DecodeExtKey(derive.prv) == key); //ensure a base58 decoded key also matches

        // Test public key
        BOOST_CHECK(EncodeExtPubKey(pubkey) == derive.pub);
        BOOST_CHECK(DecodeExtPubKey(derive.pub) == pubkey); //ensure a base58 decoded pubkey also matches

        // Derive new keys
        CExtKey keyNew;
        BOOST_CHECK(key.Derive(keyNew, derive.nChild));
        CExtPubKey pubkeyNew = keyNew.Neuter();
        if (!(derive.nChild & 0x80000000)) {
            // Compare with public derivation
            CExtPubKey pubkeyNew2;
            BOOST_CHECK(pubkey.Derive(pubkeyNew2, derive.nChild));
            BOOST_CHECK(pubkeyNew == pubkeyNew2);
        }
        key = keyNew;
        pubkey = pubkeyNew;
    }
}

}  // namespace

BOOST_FIXTURE_TEST_SUITE(bip32_tests, BasicTestingSetup)

BOOST_AUTO_TEST_CASE(bip32_test1) {
    RunTest(test1);
}

BOOST_AUTO_TEST_CASE(bip32_test2) {
    RunTest(test2);
}

BOOST_AUTO_TEST_CASE(bip32_test3) {
    RunTest(test3);
}

BOOST_AUTO_TEST_CASE(bip32_test4) {
    RunTest(test4);
}

BOOST_AUTO_TEST_CASE(bip32_test5) {
    for (const auto& str : TEST5) {
        auto dec_extkey = DecodeExtKey(str);
        auto dec_extpubkey = DecodeExtPubKey(str);
        BOOST_CHECK_MESSAGE(!dec_extkey.key.IsValid(), "Decoding '" + str + "' as xprv should fail");
        BOOST_CHECK_MESSAGE(!dec_extpubkey.pubkey.IsValid(), "Decoding '" + str + "' as xpub should fail");
    }
}

BOOST_AUTO_TEST_CASE(bip32_max_depth) {
    CExtKey key_parent{DecodeExtKey(test1.vDerive[0].prv)}, key_child;
    CExtPubKey pubkey_parent{DecodeExtPubKey(test1.vDerive[0].pub)}, pubkey_child;

    // We can derive up to the 255th depth..
    for (auto i = 0; i++ < 255;) {
        BOOST_CHECK(key_parent.Derive(key_child, 0));
        std::swap(key_parent, key_child);
        BOOST_CHECK(pubkey_parent.Derive(pubkey_child, 0));
        std::swap(pubkey_parent, pubkey_child);
    }

    // But trying to derive a non-existent 256th depth will fail!
    BOOST_CHECK(key_parent.nDepth == 255 && pubkey_parent.nDepth == 255);
    BOOST_CHECK(!key_parent.Derive(key_child, 0));
    BOOST_CHECK(!pubkey_parent.Derive(pubkey_child, 0));
}

BOOST_AUTO_TEST_SUITE_END()
