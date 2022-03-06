#include <eosio/eosio.hpp>
#include <eosio/system.hpp>
#include <eosio/asset.hpp>
#include <map>
#include <string>
#include <iterator>
#include <vector>
#include <math.h>
#include <tables/totalstake/totalstake.hpp>
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
      blockbunnies_symb("TRPM",0){} //thre precision of the symbol is the decimal precision of

    ACTION regstaker(name username);

    ACTION banstaker (name username);

    ACTION addadmin (name username);

    ACTION transfer( name 	from,
                      name 	to,
                      asset	quantity,
                      string	memo ) {
      eosio_assert( from != to, "cannot transfer to self" );
      require_auth( from );
      eosio_assert( is_account( to ), "to account does not exist");
      eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );
      eosio_assert( quantity.amount == 1, "cannot transfer quantity, not equal to 1" );
      auto itr = _staker_list.find(username.value);
      eosio_assert(itr != _staker_list.end(), "You cannot stake, you are not yet registered");
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
      if(memo == "start") {
        _staker_list.modify(itr, username, [&](auto& row){
          row.fund_staked = quantity;
          nftid_staked.push_back(id);
          row.isstaked = true;
        });
      }
      else if(memo == "increment"){
        _staker_list.modify(itr, username, [&](auto& row){
          row.fund_staked += quantity;
          nftid_staked.push_back(id);
        });
      }
      eosio_assert(found, "token is not found or is not owned by account");
      require_recipient( from );
      require_recipient( to );
      SEND_INLINE_ACTION( *this, transferNFT, {from, "active"_n}, {from, to, id, memo} );
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
    
    void sub_balance(name owner, asset value);
    void add_balance(name owner, asset value, name ram_payer);
};