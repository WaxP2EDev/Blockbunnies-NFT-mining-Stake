#include <staking.hpp>

ACTION blockbunnies::regstaker (name username){
  require_auth(username);
  
  auto itr_banned = _banned_list.find(username.value);
  check(itr_banned == _banned_list.end(), "You where banned, please see your administrator");

  auto itr = _staker_list.find(username.value);
  check(itr == _staker_list.end(), "You are already registered, you can stake your TRPM");

  asset temp_asset(0, blockbunnies_symb);
  itr = _staker_list.emplace(username, [&](auto& row){
    row.username = username;
    row.fund_staked = temp_asset;
    row.isstaked = false;
  });

}
ACTION getPower(vector<id_type> CommonNFTsID, vector<id_type> ToolNFTsID, string memo) {
  requires(_self(), 0);
  check(NFTsID.getlength() > 0, "Not allowed getPower");
  check(memo, "Not allowed getPower");
  // NFTsID.map((r, idx) => {
    float result = 0;
    for(unit32_t i in CommonNFTsID) {
      if(memo == "mining") {
        if(CommonNFTsID < 100) {
          result += MiningcrewNFTs[0]; 
        }
        else if(CommonNFTsID < 200) {
          result += MiningcrewNFTs[1];          
        }
        else if(CommonNFTsID < 250) {
          result += MiningcrewNFTs[2];
        }
        else if(CommonNFTsID < 280) {
          result += MiningcrewNFTs[3];
        }
        else if(CommonNFTsID < 300) {
          result += MiningcrewNFTs[4];
        }
        else if(CommonNFTsID < 320) {
          result += MiningcrewNFTs[5];
        }
        else if(CommonNFTsID < 330) {
          result += MiningcrewNFTs[6];
        }
        else if(CommonNFTsID < 340) {
          result += MiningcrewNFTs[7];
        }
        else {
          result += MiningcrewNFTs[8];
        }

      }
      else if(memo == "farming") {
        if(CommonNFTsID < 100) {
          result += FarmingcharacterNFTs[0]; 
        }
        else if(CommonNFTsID < 200) {
          result += FarmingcharacterNFTs[1];          
        }
        else if(CommonNFTsID < 250) {
          result += FarmingcharacterNFTs[2];
        }
        else if(CommonNFTsID < 280) {
          result += FarmingcharacterNFTs[3];
        }
        else if(CommonNFTsID < 300) {
          result += FarmingcharacterNFTs[4];
        }
        else if(CommonNFTsID < 320) {
          result += FarmingcharacterNFTs[5];
        }
        else if(CommonNFTsID < 330) {
          result += FarmingcharacterNFTs[6];
        }
      }    
    }
    for(unit32_t i in ToolNFTsID) {
      if(memo == "mining") {
        if(ToolNFTsID < 100) {
          result += MiningcrewNFTs[0]; 
        }
        else if(ToolNFTsID < 200) {
          result += MiningcrewNFTs[1];          
        }
        else if(ToolNFTsID < 250) {
          result += MiningcrewNFTs[2];
        }
        else if(ToolNFTsID < 280) {
          result += MiningcrewNFTs[3];
        }
        else if(ToolNFTsID < 300) {
          result += MiningcrewNFTs[4];
        }
        else if(ToolNFTsID < 320) {
          result += MiningcrewNFTs[5];
        }
        else if(ToolNFTsID < 330) {
          result += MiningcrewNFTs[6];
        }
        else if(ToolNFTsID < 340) {
          result += MiningcrewNFTs[7];
        }
        else {
          result += MiningcrewNFTs[8];
        }

      }
      else if(memo == "farming") {
        if(ToolNFTsID < 100) {
          result += FarmingcharacterNFTs[0]; 
        }
        else if(ToolNFTsID < 200) {
          result += FarmingcharacterNFTs[1];          
        }
        else if(ToolNFTsID < 250) {
          result += FarmingcharacterNFTs[2];
        }
        else if(ToolNFTsID < 280) {
          result += FarmingcharacterNFTs[3];
        }
        else if(ToolNFTsID < 300) {
          result += FarmingcharacterNFTs[4];
        }
        else if(ToolNFTsID < 320) {
          result += FarmingcharacterNFTs[5];
        }
        else if(ToolNFTsID < 330) {
          result += FarmingcharacterNFTs[6];
        }
      }    
    }
      
}

ACTION blockbunnies::banstaker(name username){
  require_auth(get_self());

  auto itr = _staker_list.find(username.value);
  check(itr != _staker_list.end(), "That user is not a staker");

  //registering user in the banned list
  _banned_list.emplace(get_self(), [&](auto& row){
  row.username = username;
  row.fund_held = itr->fund_staked;
  });

  //remove staker from staker list without reimbursment
  _staker_list.erase(itr);
}

ACTION blockbunnies::addadmin(name username){
  require_auth(get_self());

  check(is_account(username), "Given username is not an account on the network");

  auto itr_admin = _admin_list.find(username.value);
  check(itr_admin == _admin_list.end(), "That account is already an administrator in the system");

  _admin_list.emplace(get_self(),[&](auto& row){
    row.username = username;
  });
}
void blockbunnies::stake(name username, name receiver, asset quantity, string msg){
  require_auth(username);
  if (receiver != get_self() || username == get_self()) return;
  check(quantity.symbol == blockbunnies_symb, "wrong NFT used");
  //check( msg.size() <= 256, "msg has more than 256 bytes" );
  check(msg == "increment" || msg =="start", "Please use \"increment\" to increase your stake or \"start\" to deposit your first stake");
  
  if(msg == "start"){
    check(quantity.amount >= 1, "Staked amount of NFT not enough, stake at least 1");
    transfer(username, receiver, quantity, msg);
  }
  //  && itr->isstaked == true
  else if (msg == "increment"){
    transfer(username, receiver, quantity, msg);
    
  }
  else check(false, "Error with staking options, please check you status");

  require_recipient(username);
}


ACTION blockbunnies::unstake (name username){
  require_auth(username);
  auto itr = _staker_list.find(username.value);
  check(itr != _staker_list.end(), "Your are not a staker, please register first");
  print(get_self());

  in_contract_transfer(username,itr->fund_staked,string("Your funds have been transfered"));    

  _staker_list.erase(itr);
  require_recipient(username);
}
void blockbunnies::sub_balance( name owner, asset value ) {

	account_index from_acnts( _self, owner.value );
    const auto& from = from_acnts.get( value.symbol.code().raw(), "no balance object found" );
    check( from.balance.amount >= value.amount, "overdrawn balance" );


    if( from.balance.amount == value.amount ) {
        from_acnts.erase( from );
    } else {
        from_acnts.modify( from, owner, [&]( auto& a ) {-
            a.balance -= value;
        });
    }
}

void blockbunnies::add_balance( name owner, asset value, name ram_payer ) {

	account_index to_accounts( _self, owner.value );
        auto to = to_accounts.find( value.symbol.code().raw() );
        if( to == to_accounts.end() ) {
            to_accounts.emplace( ram_payer, [&]( auto& a ){
                a.balance = value;
            });
        } else {
            to_accounts.modify( to, _self, [&]( auto& a ) {
                a.balance += value;
            });
        }
}
void blockbunnies::in_contract_transfer(name recipient, asset amount, string msg){

  //TODO: Chance contract name from test1 to eosio.token during deployment
  action{
        permission_level{get_self(), "active"_n},
        "test1"_n,
        "transfer"_n,
        std::make_tuple(get_self(), recipient, amount, msg)
      }.send();
}
EOSIO_DISPATCH(blockbunnies, (stake))
