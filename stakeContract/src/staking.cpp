#include <staking.hpp>
#include <cron.hpp>

ACTION blockbunnies::regstaker (name username, vector<id_type> nftid_staked, vector<id_type> toolnftid_staked, string place){
  require_auth(username);
  
  auto itr_banned = _banned_list.find(username.value);
  check(itr_banned == _banned_list.end(), "You where banned, please see your administrator");

  auto itr = _staker_list.find(username.value);
  claim(username, place);
  check(itr == _staker_list.end(), "You are already registered, you can stake your NFTs");
  time_point_sec current_time(now());
  // asset temp_asset(0, blockbunnies_symb);
  stake(username, CONTRACT_ADDRESS, nftid_staked.length(), "startcommon");
  stake(username, CONTRACT_ADDRESS, toolnftid_staked.length(), "starttool");
  itr = _staker_list.emplace(username, [&](auto& row){
    row.username = username;
    for(id_type i in nftid_staked) {
      row.nftid_staked.push_back(i);  
    }
    for(id_type i in toolnftid_staked) {
      row.toolnftid_staked.push_back(i);  
    }
    row.last_updated = current_time;
    row.next_run = row.last_updated + period;
    row.place = place;
    row.collect_amount = getReward(username, place);
    row.isstaked = true;
  });
}
float blockbunnies::getPower(vector<id_type> CommonNFTsID, vector<id_type> ToolNFTsID, bool Vip, string memo) {
  requires(_self(), 0);
  check(NFTsID.getlength() > 0, "Not allowed getPower");
  check(memo, "Not allowed getPower");
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
    float plusPower = 0;
    for(unit32_t i in ToolNFTsID) {
      if(memo == "mining") {
        if(ToolNFTsID < 100) {
          plusPower += MiningtoolNFTs[0]; 
        }
        else if(ToolNFTsID < 200) {
          plusPower += MiningtoolNFTs[1];          
        }
        else if(ToolNFTsID < 250) {
          plusPower += MiningtoolNFTs[2];
        }
        else if(ToolNFTsID < 280) {
          plusPower += MiningtoolNFTs[3];
        }
        else if(ToolNFTsID < 300) {
          plusPower += MiningtoolNFTs[4];
        }
        else if(ToolNFTsID < 320) {
          plusPower += MiningtoolNFTs[5];
        }
        else if(ToolNFTsID < 330) {
          plusPower += MiningtoolNFTs[6];
        }
        else if(ToolNFTsID < 340) {
          plusPower += MiningtoolNFTs[7];
        }
        else {
          plusPower += MiningtoolNFTs[8];
        }

      }
      else if(memo == "farming") {
        if(ToolNFTsID < 100) {
          plusPower += FarmingtoolNFTs[0]; 
        }
        else if(ToolNFTsID < 200) {
          plusPower += FarmingtoolNFTs[1];          
        }
        else if(ToolNFTsID < 250) {
          plusPower += FarmingtoolNFTs[2];
        }
        else if(ToolNFTsID < 280) {
          plusPower += FarmingtoolNFTs[3];
        }
        else if(ToolNFTsID < 300) {
          plusPower += FarmingtoolNFTs[4];
        }
        else if(ToolNFTsID < 320) {
          plusPower += FarmingtoolNFTs[5];
        }
        else if(ToolNFTsID < 330) {
          plusPower += FarmingtoolNFTs[6];
        }
      }    
    }
    if(Vip) {
      result = result + result*((plusPower+10)/100);
    }
    else
    {
      result = result + result*((plusPower)/100);
    }
    return result;
      
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
asset blockbunnies::getReward(name username, string memo) {
  require_auth(get_self());
  asset reward;
  if(memo == "mining") {
    reward  = "100.0000 BUNNY"; 
  }
  else if(memo == "farming") {
    reward  = "100.0000 CARROT";
  }
  auto itr = _staker_list.find(username.value);
  _staker_list.modify( itr, _self, [&]( auto& a ) {
      a.collect_amount.amount += reward.amount;
  });
  return reward;

}
ACTION blockbunnies::claim(name username, string memo) {
  require_auth(get_self());
  time_point_sec current_time(now());
  for(auto& item : _staker_list) {
    if (item.next_run < current_time) {
        item.last_updated = item.next_run;
        item.next_run = last_updated + period;
        getReward(item.username, memo);
    //     if(memo == "mining") {
    //     }
    //     else if(memo == "farming") {
    //       transfer( CONTRACT_ADDRESS, item.username, "100.0000 CARROT", "Prizepayout bonus");   
    //     }
    // }
  }
  auto itr = _staker_list.find(username.value);

  check(itr == _staker_list.end(), "Not staker");
  _staker_list.modify( itr, _self, [&]( auto& a ) {
      a.collect_amount = "";
      transfer( CONTRACT_ADDRESS, item.username, a.collect_amount, "Prizepayout bonus");   
  });

}
EOSIO_DISPATCH(blockbunnies, (stake))
