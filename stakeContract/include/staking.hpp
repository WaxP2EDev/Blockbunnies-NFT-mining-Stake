#include <eosio/eosio.hpp>
#include <eosio/system.hpp>
#include <eosio/asset.hpp>
#include <map>
#include <string>
#include <iterator>
#include <vector>
#include <math.h>
// #include <totalstake.hpp>
using namespace eosio;
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
    vector<float> MiningcrewNFTs = {0.2,7.19,0.66, 258.29, 78.27, 2.18, 78.27, 7.19, 258.29};
    vector<uint8_t> MiningtoolNFTs = {1, 1, 1, 2, 4, 2, 4, 2, 4};
    vector<float> FarmingcharacterNFTs = {23.27, 0.66, 0.2, 7.19, 258.29, 78.27, 7.19};
    vector<uint8_t> FarmingtoolNFTs = {1, 1, 2, 4, 2, 1, 2};
    const uint32_t period = 4 * 60 * 60;

    ACTION regstaker(name username);

    ACTION banstaker (name username);

    ACTION addadmin (name username);
    
    ACTION unstake (name username);

    ACTION claim(name username, string memo);

    ACTION transfer( name from, name to, asset	quantity, string	memo ); 
    
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
        vector<id_type> nftid_staked;
        vector<id_type> toolnftid_staked;
        time_point_sec last_updated;
        time_point_sec next_run;
        assert collect_amount;
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

    TABLE timetable_data {
        uint64_t key;
        name from;
        name account;
        string action;
        uint32_t period;
        time_point_sec last_updated;
        time_point_sec next_run;
        bool active;

        uint64_t primary_key() const { return key;}
        uint64_t by_last_updated() const { return last_updated.utc_seconds; }
    };
    const symbol blockbunnies_symb;
    typedef eosio::multi_index<"timetable"_n,
                          timetable_data,
                          eosio::indexed_by<"lastupdated"_n,
                                            eosio::const_mem_fun<timetable_data,
                                                                uint64_t,
                                                                &timetable_data::by_last_updated>>> timetable;
    void sub_balance(name owner, asset value);
    void add_balance(name owner, asset value, name ram_payer);
    void in_contract_transfer(name recipient, asset amount, string msg);
    float getPower(vector<id_type> CommonNFTsID, vector<id_type> ToolNFTsID, bool Vip, string memo);
    asset getReward(name username, string memo);
    void stake(name username, name receiver, asset quantity, string msg);
    void transferNFT( name	from, name 	to, id_type	id, string	memo );


};