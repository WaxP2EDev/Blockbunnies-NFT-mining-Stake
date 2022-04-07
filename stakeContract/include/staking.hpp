#include <eosio/eosio.hpp>
#include <eosio/system.hpp>
#include <eosio/asset.hpp>
// #include <../../../atomicassets-contract/include/atomicassets.hpp>
#include <map>
#include <string>
#include <iterator>
#include <vector>
#include <math.h>
// #include <totalstake.hpp>
using namespace eosio;
using namespace std;

using std::string;
using std::vector;

#define CONTRACT_NAME "blockbunnies"
#define CONTRACT_ADDRESS "contractAddress"

typedef uint128_t uuid;
typedef uint64_t id_type;
typedef string uri_type;
#define BLOCKBUNNIES_SYMB ""

CONTRACT blockbunnies : public eosio::contract {
  public:
    using contract::contract;
    blockbunnies(eosio::name receiver, eosio::name code, eosio::datastream<const char *> ds) : 
      contract(receiver, code, ds),
      _staker_list(receiver, receiver.value),
      _banned_list(receiver, receiver.value),
      _admin_list(receiver, receiver.value),
      tokens(receiver, receiver.value),

    blockbunnies_symb("TRPM",0){} //thre precision of the symbol is the decimal precision of
    vector<uint64_t> crewPirateTemID = {204519, 204521, 204523, 204523, 204528, 204529, 204531, 204532, 204535};
    vector<uint64_t> crewZombieTemID = {232860, 232861, 232862, 232864, 232887, 232892, 232893, 232894, 233274};
    vector<uint64_t> miningToolTemID = {232860, 232861, 232862, 232864, 232887, 232892, 232893, 232894, 233274};

    vector<uint64_t> chaSpaceTemID = {253566, 253567, 253569, 253570, 253572, 253573, 253574};
    vector<uint64_t> chaKillTemID = {253583, 253584, 253585, 253586, 253587, 253588, 253589};
    vector<uint64_t> chaDecTemID = {253594, 253595, 253596, 253597, 253598, 253599, 253601};
    vector<uint64_t> chaSkaTemID = {280422, 280423, 280424, 280425, 280426, 280427, 280428};
    vector<uint64_t> chaSamTemID = {280429, 280430, 280431, 280432, 280433, 280434, 280435};
    vector<uint64_t> chaStoTemID = {280436, 280438, 280439, 280440, 280441, 280442, 280443};
    vector<uint64_t> farmingToolTemID = {280436, 280438, 280439, 280440, 280441, 280442, 280443};

    vector<float> MiningcrewNFTs = {0.2, 0.2, 0.66, 0.66, 2.18, 7.19, 23.72, 78.27, 258.29};
    vector<uint8_t> MiningtoolNFTs = {1, 1, 1, 2, 4, 2, 4, 2, 4};
    vector<float> FarmingcharacterNFTs = {0.2, 0.66, 2.18, 7.19, 23.72, 78.27, 258.29 };
    vector<uint8_t> FarmingtoolNFTs = {1, 1, 2, 4, 2, 1, 2};
    const uint32_t period = 4 * 60 * 60;
    name contractowner;
    struct imeta {
      name username;
      string collection_name;
      id_type template_id;
      id_type assets_id;
    };
    
    ACTION regstaker (name username, vector<imeta> nftid_staked, vector<imeta> toolnftid_staked, bool Vip, string place, string selectLand);

    ACTION banstaker (name username);

    ACTION addadmin (name username);
    
    ACTION unstake (name username, id_type assets_id);

    ACTION claim(name username, string memo);

    // ACTION transfer( name from, name to, asset	quantity, string	memo ); 
    
  private:
    
    TABLE total_stake {
      eosio::name account;
      eosio::asset to_self;
      eosio::asset to_others;
      eosio::asset locked;
      eosio::asset available;

      uint64_t primary_key() const { return account.value; }
    };

    typedef eosio::multi_index<"totalstake"_n, total_stake> total_stake_index;

    TABLE staker{
        name username; //name of the staking user
        asset fund_staked; // funds to be staked
        string place;
        vector<imeta> nftid_staked ;
        vector<imeta> toolnftid_staked;
        time_point_sec last_updated;
        time_point_sec next_run;
        asset collect_amount;
        bool isstaked; //if the users has already staked funds or not
        uint64_t primary_key() const {return username.value;}
    };
    typedef eosio::multi_index<name("stakerlist"), staker> stakers;
    stakers _staker_list;

    TABLE banned{
        name username;
        asset fund_held; //funds that were staked by the user
        uint64_t primary_key() const {return username.value;}
    };
    typedef eosio::multi_index<name("bannedlist"), banned> banned_stakers;
    banned_stakers _banned_list; 

    TABLE admin{
        name username;
        uint64_t primary_key() const {return username.value;}
    };
    typedef eosio::multi_index<name("adminlist"), admin> admins;
    admins _admin_list;

    TABLE account {

        asset balance;

        uint64_t primary_key() const { return balance.symbol.code().raw(); }
    };
    using account_index = eosio::multi_index<"accounts"_n, account>;

    TABLE token {
        id_type id;          // Unique 64 bit identifier,
        uri_type uri;        // RFC 3986
        name owner;  	 // token owner
        asset value;         // token value (1 SYS)
        string tokenName;	 // token name

        id_type primary_key() const { return id; }
        uint64_t get_owner() const { return owner.value; }
        string get_uri() const { return uri; }
        asset get_value() const { return value; }
        uint64_t get_symbol() const { return value.symbol.code().raw(); }
        string get_name() const { return tokenName; }

        // generated token global uuid based on token id and
        // contract name, passed in the argument
        uuid get_global_id(name self) const
        {
          uint128_t self_128 = static_cast<uint128_t>(self.value);
          uint128_t id_128 = static_cast<uint128_t>(id);
          uint128_t res = (self_128 << 64) | (id_128);
          return res;
        }

        string get_unique_name() const
        {
          string unique_name = tokenName + "#" + std::to_string(id);
          return unique_name;
        }
    };

    using token_index = eosio::multi_index<"token"_n, token,
                  indexed_by< "byowner"_n, const_mem_fun< token, uint64_t, &token::get_owner> >,
      indexed_by< "bysymbol"_n, const_mem_fun< token, uint64_t, &token::get_symbol> > >;
    token_index tokens;

    TABLE assets_s {
          uint64_t         asset_id;
          name             collection_name;
          name             schema_name;
          int32_t          template_id;
          name             ram_payer;
          vector <asset>   backed_tokens;
          vector <uint8_t> immutable_serialized_data;
          vector <uint8_t> mutable_serialized_data;

          uint64_t primary_key() const { return asset_id; };
      };

      typedef multi_index <name("assets"), assets_s> assets_t;

    const symbol blockbunnies_symb;
    const symbol blockcarrots_symb;

    void sub_balance(name owner, asset value);
    void add_balance(name owner, asset value, name ram_payer);
    void in_contract_transfer(name recipient, vector<id_type> assets_id, string msg);
    // float getPower(vector<id_type> CommonNFTsID, vector<id_type> ToolNFTsID, bool Vip, string memo);
    float getPower(vector<id_type> CommonNFTsID, vector<id_type> ToolNFTsID,  bool Vip, string memo, string selectLand);
    eosio::asset getReward(name username, string selectLand);
    void stake(name username, name receiver, vector<uint64_t> asset_ids, string msg);
    void transferNFT( name	from, name 	to, id_type	id, string	memo );


};