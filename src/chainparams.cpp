// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Copyright (c) 2014-2017 The Dash Core developers
// Copyright (c) 2017-2018 The GENIX Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "uint256.h"
#include "arith_uint256.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>
#include <boost/assign/list_of.hpp>
#include <limits>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1522190273, nBits=1e0ffff0, nNonce=28917698, vtx=1)
 *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
 *   vMerkleTree: e0028e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "GenixMain";
    const CScript genesisOutputScript = CScript() << ParseHex("03289c0933f7ed53fc996de0c252cd6bbf9e9b8161dcda7615c2503dbd5d48f02bdb72bd216af26b6815e0b2f50381100916a7eb7b1a88aeb8debb0803250d8401") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

const arith_uint256 maxUint = UintToArith256(uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));


class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 518400; // Note: actual number of blocks per calendar year with DGW v3 is ~200700 (for example 449750 - 249050)
        consensus.nMasternodePaymentsStartBlock = 780; // not true, but it's ok as long as it's less then nMasternodePaymentsIncreaseBlock
        consensus.nMasternodePaymentsIncreaseBlock = 100001; // Block 100,001 Approx 1 year
        consensus.nMasternodePaymentsIncreasePeriod = 100001; // Block 100,001 Approx 1 year
        consensus.nInstantSendKeepLock = 24;
        consensus.nBudgetPaymentsStartBlock = 9999999999; // Disabled
        consensus.nBudgetPaymentsCycleBlocks = 9999999999; // Disabled
        consensus.nBudgetPaymentsWindowBlocks = 9999999999; // Disabled
        consensus.nBudgetProposalEstablishingTime = 60*20*9999999999; // Disabled
        consensus.nSuperblockStartBlock = 99999999999; // Disabled
        consensus.nSuperblockCycle = 9999999999; // Disabled
        consensus.nGovernanceMinQuorum = 10;
        consensus.nGovernanceFilterElements = 20000;
        consensus.nMasternodeMinimumConfirmations = 15;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256S("0x000005c44e04128d9953a742860dbe3c10fb527a3605462cb6bd3e36dd6a285a");
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

        consensus.nZawyLwmaAveragingWindow = 65;
        consensus.nZawyLwmaAjustedWeight = 3927;

        consensus.nPowTargetTimespan = 24 * 60 * 60; // GENIX: 1 day
        consensus.nPowTargetSpacing = 2 * 60; // GENIX: 2 minutes
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.useDarkGravityWave = true;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing * 67.2
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1555135200; // Apr 13th, 2019 06:00AM
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1586757600; // Apr 13th, 2020 06:00AM

        // Deployment of DIP0001
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 1555135200; // Apr 13th, 2019 06:00AM
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 1586757600; // Apr 13th, 2020 06:00AM
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nWindowSize = 4032;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nThreshold = 3226; // 80% of 4032

        // The best chain should have at least this much work.
        //consensus.nMinimumChainWork = uint256S("0x000000000000000000000000000000000000000000000100a308553b4863b755"); // 782700

        // By default assume that the signatures in ancestors of this block are valid.
        //consensus.defaultAssumeValid = uint256S("0x000000000000001c172f518793c3b9e83f202284615592f87fe3506ce964dcd4"); // 782700

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000080874736ed6cd2");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00000007f658c1cdaeacb36b7f75894b42fb534c227685dbe660de13a27eca23");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xcf;
        pchMessageStart[1] = 0x4e;
        pchMessageStart[2] = 0x3b;
        pchMessageStart[3] = 0x49;
        nDefaultPort = 43649;
        nMaxTipAge = 6 * 60 * 60; // ~144 blocks behind -> 2 x fork detection time
        nDelayGetHeadersTime = 24 * 60 * 60;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1551279600, 2084647557, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
   /*
	 //////////////
        //////////////
                // calculate Genesis Block
                // Reset genesis
                consensus.hashGenesisBlock = uint256S("0x");
                std::cout << std::string("Begin calculating Mainnet Genesis Block:\n");
                if (true && (genesis.GetHash() != consensus.hashGenesisBlock)) {
                    LogPrintf("Calculating Mainnet Genesis Block:\n");
                    arith_uint256 hashTarget = arith_uint256().SetCompact(genesis.nBits);
                    uint256 hash;
                    genesis.nNonce = 0;
                    // This will figure out a valid hash and Nonce if you're
                    // creating a different genesis block:
                    // uint256 hashTarget = CBigNum().SetCompact(genesis.nBits).getuint256();
                    // hashTarget.SetCompact(genesis.nBits, &fNegative, &fOverflow).getuint256();
                    // while (genesis.GetHash() > hashTarget)
                    while (UintToArith256(genesis.GetHash()) > hashTarget)
                    {
                        ++genesis.nNonce;
                        if (genesis.nNonce == 0)
                        {
                            LogPrintf("NONCE WRAPPED, incrementing time");
                            std::cout << std::string("NONCE WRAPPED, incrementing time:\n");
                            ++genesis.nTime;
                        }
                        if (genesis.nNonce % 10000 == 0)
                        {
                            LogPrintf("Mainnet: nonce %08u: hash = %s \n", genesis.nNonce, genesis.GetHash().ToString().c_str());
                            // std::cout << strNetworkID << " nonce: " << genesis.nNonce << " time: " << genesis.nTime << " hash: " << genesis.GetHash().ToString().c_str() << "\n";
                        }
                    }
                    std::cout << "Mainnet ---\n";
                    std::cout << "  nonce: " << genesis.nNonce <<  "\n";
                    std::cout << "   time: " << genesis.nTime << "\n";
                    std::cout << "   hash: " << genesis.GetHash().ToString().c_str() << "\n";
                    std::cout << "   merklehash: "  << genesis.hashMerkleRoot.ToString().c_str() << "\n";
                    // Mainnet --- nonce: 296277 time: 1390095618 hash: 000000bdd771b14e5a031806292305e563956ce2584278de414d9965f6ab54b0
                }
                std::cout << std::string("Finished calculating Mainnet Genesis Block:\n");
     */
     //   assert(consensus.hashGenesisBlock == uint256S("0000038977617c01646209e33e354174ef916df8284346b29aecfbc98fa43dd0"));
     //   assert(genesis.hashMerkleRoot == uint256S("93113cc5a2df97b20bbe91731578c6722080355be0b4b83b60c6b9ac535d5d15"));

        vSeeds.push_back(CDNSSeedData("161.43.201.255", "161.43.201.255")); // farsider350
        
     //   vSeeds.push_back(CDNSSeedData(""));
        // vFixedSeeds.clear();
        // vSeeds.clear();

        // GENIX addresses start with 'G'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,38);
        // GENIX script addresses start with '6'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,13);
        // GENIX private keys start with 't'
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,127);
        // GENIX BIP32 pubkeys start with 'xpub' (GENIX defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        // GENIX BIP32 prvkeys start with 'xprv' (GENIX defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        // GENIX BIP44 coin type is '5'
        nExtCoinType = 5;

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 60*60; // fulfilled requests expire in 1 hour
        strSporkPubKey = "0207768ba840e023b0cca0200474e20f5892dc2916b66f29b3bfed1b73ad1904eb";

      //  checkpointData = (CCheckpointData) {
      //      boost::assign::map_list_of
      //      (  58382, uint256S("0x000000000029aec185b7462e1661e726bef920b41f1b728a1879c5d7527aae6d")),
      //      1561717114, // * UNIX timestamp of last checkpoint block
                // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
      //      25        // * estimated number of transactions per day after checkpoint
      //  };
    }
};

static CMainParams mainParams;



/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 210240;
        consensus.nMasternodePaymentsStartBlock = 401; // not true, but it's ok as long as it's less then nMasternodePaymentsIncreaseBlock
        consensus.nMasternodePaymentsIncreaseBlock = 2030;
        consensus.nMasternodePaymentsIncreasePeriod = 10;
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsStartBlock = 9999999999; // Disabled
        consensus.nBudgetPaymentsCycleBlocks = 9999999999; // Disabled
        consensus.nBudgetPaymentsWindowBlocks = 9999999999; // Disabled
        consensus.nBudgetProposalEstablishingTime = 60*20*9999999999; // Disabled
        consensus.nSuperblockStartBlock = 99999999999; // Disabled
        consensus.nSuperblockCycle = 9999999999; // Disabled
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 500;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 100;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x000006874678aa53f78b7676ced0f443cd22ae8917199b5ec14d0b7b7df7b93d");
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

        consensus.nZawyLwmaAveragingWindow = 65;
        consensus.nZawyLwmaAjustedWeight = 3927;

        consensus.nPowTargetTimespan = 24 * 60 * 60; // GENIX: 1 day
        consensus.nPowTargetSpacing = 2 * 60; // GENIX: 2 minutes
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.useDarkGravityWave = true;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1598524789; // 2020
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1630060789; // 2021

        // Deployment of DIP0001
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 1598524789; // 2020
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 1630060789; // 2021
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nWindowSize = 100;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nThreshold = 50; // 50% of 100

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0x2b;
        pchMessageStart[1] = 0x1c;
        pchMessageStart[2] = 0x2a;
        pchMessageStart[3] = 0x10;
        nDefaultPort = 31345;
     //   vAlertPubKey = ParseHex("046e05270f6d736f006cb682c9e15998e3505e6185475f7e1a9bc5a4965984b6933ed8988336006ad07d0b5ae408bf5805183c5a94ab376d0e50f0c51ab30f6662");
        nMaxTipAge = 0x7fffffff; // allow mining on top of old blocks for testnet
        nDelayGetHeadersTime = 24 * 60 * 60;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1551279600, 2084647557, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0000038977617c01646209e33e354174ef916df8284346b29aecfbc98fa43dd0"));
        assert(genesis.hashMerkleRoot == uint256S("93113cc5a2df97b20bbe91731578c6722080355be0b4b83b60c6b9ac535d5d15"));

        vSeeds.push_back(CDNSSeedData("139.180.162.209", "139.180.162.209"));
        vSeeds.push_back(CDNSSeedData("fixed-seeds.genixproject.org", "testnet.fixed-seeds.genixproject.org"));
        vFixedSeeds.clear();
        vSeeds.clear();

        // Testnet GENIX addresses start with 'g'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,98);
        // Testnet GENIX script addresses start with '9'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,11);
        // Testnet private keys start with '9' or 'c' (GENIX defaults)
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,127);
        // Testnet GENIX BIP32 pubkeys start with 'tpub' (GENIX defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        // Testnet GENIX BIP32 prvkeys start with 'tprv' (GENIX defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        // Testnet GENIX BIP44 coin type is '1' (All coin's testnet default)
        nExtCoinType = 1;

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes
        strSporkPubKey = "0207768ba840e023b0cca0200474e20f5892dc2916b66f29b3bfed1b73ad1904eb";

         checkpointData = (CCheckpointData) {
             boost::assign::map_list_of
             (  0, uint256S("0x0000038977617c01646209e33e354174ef916df8284346b29aecfbc98fa43dd0"))
        //     ( 22520, uint256S("00000002c95eb91831bb7cb51ba6f5f39311b76280720f3aa02f8fafbeda0993")),
             1551279600, // * UNIX timestamp of last checkpoint block
             1,       // * total number of transactions between genesis and last checkpoint
        //                 //   (the tx=... number in the SetBestChain debug.log lines)
             299         // * estimated number of transactions per day after checkpoint
         };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMasternodePaymentsStartBlock = 2;
        consensus.nMasternodePaymentsIncreaseBlock = 999;
        consensus.nMasternodePaymentsIncreasePeriod = 999;
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsStartBlock = 1000;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nBudgetProposalEstablishingTime = 60*20;
        consensus.nSuperblockStartBlock = 1500;
        consensus.nSuperblockCycle = 10;
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 100;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = -1; // BIP34 has not necessarily activated on regtest
        consensus.BIP34Hash = uint256();
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

        consensus.nZawyLwmaAveragingWindow = 65;
        consensus.nZawyLwmaAjustedWeight = 3927;

        consensus.nPowTargetTimespan = 30 * 60 * 2; // GENIX: 1 hour
        consensus.nPowTargetSpacing = 2 * 60; // GENIX: 2 minutes
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.useDarkGravityWave = false;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xf4;
        pchMessageStart[1] = 0xaf;
        pchMessageStart[2] = 0xbf;
        pchMessageStart[3] = 0xf5;
        nMaxTipAge = 6 * 60 * 60; // ~144 blocks behind -> 2 x fork detection time, was 24 * 60 * 60 in genix
        nDelayGetHeadersTime = 0; // never delay GETHEADERS in regtests
        nDefaultPort = 17978;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1549043100, 2084844903, 0x1e0ffff0, 1, 150000 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
       // assert(consensus.hashGenesisBlock == uint256S("0x000005ec6d48ac579d697448a82e93127b94403770629399cf561caa216a694b"));
        //assert(genesis.hashMerkleRoot == uint256S("0xd5dec0980d7b84cc1c048eb8706afe68bbbdb07fdefab76de8d176dfcb858ae8"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("000006874678aa53f78b7676ced0f443cd22ae8917199b5ec14d0b7b7df7b93d")),
            1549043100,
            0,
            0
        };
        // Regtest GENIX addresses start with 'o'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,115);
        // Regtest GENIX script addresses start with 'a'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,83);
        // Regtest private keys start with '9' or 'c' (GENIX defaults)
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        // Regtest GENIX BIP32 pubkeys start with 'tpub' (GENIX defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        // Regtest GENIX BIP32 prvkeys start with 'tprv' (GENIX defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        // Regtest GENIX BIP44 coin type is '1' (All coin's testnet default)
        nExtCoinType = 1;
   }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}
