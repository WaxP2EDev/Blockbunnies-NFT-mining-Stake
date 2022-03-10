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

    ACTION getPower(vector<id_type> CommonNFTsID, vector<id_type> ToolNFTsID, bool Vip, string memo);
    ACTION regstaker(name username);

    ACTION banstaker (name username);

    ACTION addadmin (name username);
    void stake(name username, name receiver, asset quantity, string msg);
    ACTION reward (name from, name to, )

    ACTION unstake (name username);

    ACTION transfer( name 	from,
                      name 	to,
                      asset	quantity,
                      string	memo ) {
      check( from != to, "cannot transfer to self" );
      require_auth( from );
      check( is_account( to ), "to account does not exist");
      check( memo.size() <= 256, "memo has more than 256 bytes" );
      check( quantity.amount == 1, "cannot transfer quantity, not equal to 1" );
      auto itr = _staker_list.find(from.value);
      check(itr != _staker_list.end(), "You cannot stake, you are not yet registered");
      auto symbl = tokens.get_index<"bysymbol"_n>();
      auto it = symbl.lower_bound(quantity.symbol.code().raw());
      bool found = false;
      id_type id = 0;
      for(; it!=symbl.end(); ++it){
        if( it->value.symbol == quantity.symbol && it->owner == from) {
          id = it->id;
          found = true;
          break;
        }
      }
      if(memo == "startcommon") {
        _staker_list.modify(itr, from, [&](auto& row){
          row.fund_staked = quantity;
          row.nftid_staked.push_back(id);
          row.isstaked = true;
        });
      }
      else if(memo == "starttool") {
        _staker_list.modify(itr, from, [&](auto& row){
          row.fund_staked = quantity;
          row.toolnftid_staked.push_back(id);
          row.isstaked = true;
        });
      }
      // else if(memo == "increment"){
      //   _staker_list.modify(itr, from, [&](auto& row){
      //     row.fund_staked += quantity;
      //     row.nftid_staked.push_back(id);
      //   });
      // }
      check(found, "token is not found or is not owned by account");
      require_recipient( from );
      require_recipient( to );
      // SEND_INLINE_ACTION( *this, transferNFT, {from, "active"_n}, {from, to, id, memo} );
      transferNFT(from, to, id, memo);
    }
    
    void transferNFT( name	from, name 	to, id_type	id, string	memo ) {
        // Ensure authorized to send from account
        check( from != to, "cannot transfer to self" );
        require_auth( from );

        // Ensure 'to' account exists
        check( is_account( to ), "to account does not exist");

	      // Check memo size and print
        check( memo.size() <= 256, "memo has more than 256 bytes" );

        // Ensure token ID exists
        auto send_token = tokens.find( id );
        check( send_token != tokens.end(), "token with specified ID does not exist" );

	      // Ensure owner owns token
        check( send_token->owner == from, "sender does not own token with specified ID");

	      const auto& st = *send_token;

      	// Notify both recipients
        require_recipient( from );
        require_recipient( to );

        // Transfer NFT from sender to receiver
        tokens.modify( send_token, from, [&]( auto& token ) {
	        token.owner = to;
        });

        // Change balance of both accounts
        sub_balance( from, st.value );
        add_balance( to, st.value, from );
      }
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
            vector<id_type> nftid_staked;
            vector<id_type> toolnftid_staked;
            time_point_sec last_updated;
            time_point_sec next_run;
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
};