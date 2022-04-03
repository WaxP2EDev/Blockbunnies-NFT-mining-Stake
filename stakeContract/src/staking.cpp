#include <blockbunnies.hpp>
// #include <cron.hpp>


ACTION blockbunnies::regstaker (name username, vector<imeta> nftid_staked, vector<imeta> toolnftid_staked, bool Vip, string place, string selectLand){
  require_auth(username);
  auto itr_banned = _banned_list.find(username.value);
  check(itr_banned == _banned_list.end(), "You where banned, please see your administrator");
  for(uint8_t i = 0; i < nftid_staked.size() ; i++) {
    check(nftid_staked[i].collection_name == "blockbunnie1", "This NFT not allowed in this game");
    if(place == "mining") {
      if(selectLand == "pirate") {
        for(uint8_t j = 0; j < crewPirateTemID.size() + 1; j++) {
          check(j ==  crewPirateTemID.size(), "This NFT not allowed in this game")
          if(crewPirateTemID[j] == nftid_staked[i].template_id) {
            break;
          } 

        }
      }
      else if(selectLand == "zombie") {
        for(uint8_t j = 0; j < crewZombieTemID.size() + 1; j++) {
          check(j ==  crewZombieTemID.size(), "This NFT not allowed in this game")
          if(crewZombieTemID[j] == nftid_staked[i].template_id) {
            break;
          } 

        }
      }
      else {
        check(false, "Please choose type of mining or farming");
      }
    }
    else if(place == "farming") {
        if(selectLand == "space") {
          for(uint8_t j = 0; j < chaSpaceTemID.size() + 1; j++) {
            check(j ==  chaSpaceTemID.size(), "This NFT not allowed in this game")
            if(chaSpaceTemID[j] == nftid_staked[i].template_id) {
              break;
            } 

          }
        }
        else if(selectLand == "kill") {
          for(uint8_t j = 0; j < chaKillTemID.size() + 1; j++) {
            check(j ==  chaKillTemID.size(), "This NFT not allowed in this game")
            if(chaKillTemID[j] == nftid_staked[i].template_id) {
              break;
            } 

          } 
        }
        else if(selectLand == "dec") {
          for(uint8_t j = 0; j < chaDecTemID.size() + 1; j++) {
            check(j ==  chaDecTemID.size(), "This NFT not allowed in this game")
            if(chaDecTemID[j] == nftid_staked[i].template_id) {
              break;
            } 

          } 
        }
        else if(selectLand == "ska") {
          for(uint8_t j = 0; j < chaSkaTemID.size() + 1; j++) {
            check(j ==  chaSkaTemID.size(), "This NFT not allowed in this game")
            if(chaSkaTemID[j] == nftid_staked[i].template_id) {
              break;
            } 

          } 
        }
        else if(selectLand == "sam") {
          for(uint8_t j = 0; j < chaSamTemID.size() + 1; j++) {
            check(j ==  chaSamTemID.size(), "This NFT not allowed in this game")
            if(chaSamTemID[j] == nftid_staked[i].template_id) {
              break;
            } 

          } 
        }
        else if(selectLand == "sto") {
          for(uint8_t j = 0; j < chaStoTemID.size() + 1; j++) {
            check(j ==  chaStoTemID.size(), "This NFT not allowed in this game")
            if(chaStoTemID[j] == nftid_staked[i].template_id) {
              break;
            } 

          } 
        }
        else {
          check(false, "Please choose type of mining or farming");
        }
      } 
  }
  for(uint8_t i = 0 ; i < ToolNFTsID.size() ; i++) {
    if(place == "mining") {
      for(uint8_t j = 0; j < miningToolTemID.size() + 1; j++) {
        check(j ==  miningToolTemID.size(), "This NFT not allowed in this game")
        if(miningToolTemID[j] == toolnftid_staked[i].template_id) {
          break;
        } 
      } 
    }
    else if(place == "farming") {
      for(uint8_t j = 0; j < farmingToolTemID.size() + 1; j++) {
        check(j ==  farmingToolTemID.size(), "This NFT not allowed in this game")
        if(farmingToolTemID[j] == toolnftid_staked[i].template_id) {
          break;
        } 
      }
    }
    else {
      check(false, "Please choose type of mining or farming");
    }     
  }
  auto itr = _staker_list.find(username.value);
  if(itr == _staker_list.end()) {
    itr = _staker_list.emplace(username, [&](auto& row){
      row.username = username;
      vector<id_type> assetIDNFT1 = {};
      vector<id_type> assetIDNFT2 = {};
      if(place == 'mining') {
        row.collect_amount.symbol = blockbunnies_symb;
      }
      else {
        row.collect_amount.symbol = blockcarrots_symb;
      }
      row.collect_amount.amount = 0;
      for(uint8_t i = 0 ; i < nftid_staked.size() ; i++) {
        row.nftid_staked.push_back(nftid_staked[i]);
        assetIDNFT1.push_back(nftid_staked[i].assets_id); 
      }
      stake(username, contractowner, assetIDNFT1, "start");
      for(uint8_t i = 0 ; i < toolnftid_staked.size() ; i++) {
        row.toolnftid_staked.push_back(toolnftid_staked[i]);
        assetIDNFT2.push_back(toolnftid_staked[i].assets_id);
      }
      stake(username, contractowner, assetIDNFT2, "start");
      time_point_sec         current_time         = eosio::current_time_point();
      row.last_updated = current_time;
      row.next_run = row.last_updated + period;
      row.place = place;
      row.collect_amount.amount = getPower(assetIDNFT1, assetIDNFT2, Vip, place, selectLand);
      row.isstaked = true;
      claim(username, place);
    });
  }
  else {
      claim(username, place);
  }
}
float blockbunnies::getPower(vector<id_type> CommonNFTsID, vector<id_type> ToolNFTsID,  bool Vip, string memo, string selectLand) {

    float result = 0;
    for(uint8_t i = 0 ; i < CommonNFTsID.size() ; i++) {
      if(memo == "mining") {
        if(selectLand == "pirate") {
          result += MiningcrewNFTs[std::find(crewPirateTemID.begin(), crewPirateTemID.end(), CommonNFTsID[i])[0]]; 
        }
        else if(selectLand == "zombie") {
          result += MiningcrewNFTs[std::find(crewZombieTemID.begin(), crewZombieTemID.end(), CommonNFTsID[i])[0]]; 

        }
      }
      else if(memo == "farming") {
        if(selectLand == "space") {
          // result += FarmingcharacterNFTs[std::find(CommonNFTsID[i]).chaSpaceTemID]; 
          result += FarmingcharacterNFTs[std::find(chaSpaceTemID.begin(), chaSpaceTemID.end(), CommonNFTsID[i])[0]]; 
        }
        else if(selectLand == "kill") {
          result += FarmingcharacterNFTs[std::find(chaKillTemID.begin(), chaKillTemID.end(), CommonNFTsID[i])[0]]; 
        }
        else if(selectLand == "dec") {
          result += FarmingcharacterNFTs[std::find(chaDecTemID.begin(), chaDecTemID.end(), CommonNFTsID[i])[0]]; 
        }
        else if(selectLand == "ska") {
          result += FarmingcharacterNFTs[std::find(chaSkaTemID.begin(), chaSkaTemID.end(), CommonNFTsID[i])[0]]; 
        }
        else if(selectLand == "sam") {
          result += FarmingcharacterNFTs[std::find(chaSamTemID.begin(), chaSamTemID.end(), CommonNFTsID[i])[0]]; 
        }
        else if(selectLand == "sto") {
          result += FarmingcharacterNFTs[std::find(chaStoTemID.begin(), chaStoTemID.end(), CommonNFTsID[i])[0]]; 
        }
      }    
    }
    float plusPower = 0;
    for(uint8_t i = 0 ; i < ToolNFTsID.size() ; i++) {
      if(memo == "mining") {
          plusPower += MiningtoolNFTs[std::find(miningToolTemID.begin(), miningToolTemID.end(), ToolNFTsID[i])[0]]; 
      }
      else if(memo == "farming") {
          plusPower += FarmingtoolNFTs[std::find(farmingToolTemID.begin(), farmingToolTemID.end(), ToolNFTsID[i])[0]]; 
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
  // assets_t assets("atomicassets"_n, get_self().value);
  
  require_auth(username);
  

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
void blockbunnies::stake(name username, name receiver, vector<id_type> assets_id, string msg){
  require_auth(username);
  if (receiver != get_self() || username == get_self()) return;
  // check(quantity.symbol == blockbunnies_symb, "wrong NFT used");
  check( msg.size() <= 256, "msg has more than 256 bytes" );
  check(msg == "increment" || msg =="start", "Please use \"increment\" to increase your stake or \"start\" to deposit your first stake");
  
  if(msg == "start"){
    // check(quantity.amount == 1, "Staked amount of NFT not enough, stake at least 1");
    // transfer(username, receiver, quantity, msg);
    action(
      permission_level{_self , "active"_n},
      "atomicassets"_n,              
      "transfer"_n,
      std::make_tuple(username, get_self(), assets_id, msg)
    ).send();
  }
  //  && itr->isstaked == true
  else if (msg == "increment"){
    action(
      permission_level{_self , "active"_n},
      "atomicassets"_n,              
      "transfer"_n,
      std::make_tuple(username, get_self(), assets_id, msg)
    ).send();    
  }
  else check(false, "Error with staking options, please check you status");

  require_recipient(username);
}


ACTION blockbunnies::unstake (name username,assets_id){
  require_auth(username);
  auto itr = _staker_list.find(username.value);
  check(itr != _staker_list.end(), "Your are not a staker, please register first");
  vector<id_type> total_NFTs = {};
  for(uint8_t i = 0; i < itr->nftid_staked.size() ; i++) {
    total_NFTs.push_back(itr->nftid_staked[i]);
  }
  for(uint8_t i = 0; i < itr->toolnftid_staked.size() ; i++) {
    total_NFTs.push_back(itr->toolnftid_staked[i]);
  }
  in_contract_transfer(username, total_NFTs, string("Your NFT_staked have been unstaked"));    

  _staker_list.erase(itr);
  require_recipient(username);
}
// ACTION blockbunnies::transfer( name 	from,
//                       name 	to,
//                       asset	quantity,
//                       string	memo ) {
//   check( from != to, "cannot transfer to self" );
//   require_auth( from );
//   check( is_account( to ), "to account does not exist");
//   check( memo.size() <= 256, "memo has more than 256 bytes" );
//   check( quantity.amount == 1, "cannot transfer quantity, not equal to 1" );
//   auto itr = _staker_list.find(from.value);
//   check(itr != _staker_list.end(), "You cannot stake, you are not yet registered");
//   auto symbl = tokens.get_index<"bysymbol"_n>();
//   auto it = symbl.lower_bound(quantity.symbol.code().raw());
//   bool found = false;
//   id_type id = 0;
//   for(; it!=symbl.end(); ++it){
//     if( it->value.symbol == quantity.symbol && it->owner == from) {
//       id = it->id;
//       found = true;
//       break;
//     }
//   }
//   if(memo == "startcommon") {
//     _staker_list.modify(itr, from, [&](auto& row){
//       row.fund_staked = quantity;
//       row.nftid_staked.push_back(quantity);
//       row.isstaked = true;
//     });
//   }
//   else if(memo == "starttool") {
//     _staker_list.modify(itr, from, [&](auto& row){
//       row.fund_staked = quantity;
//       row.toolnftid_staked.push_back(quantity);
//       row.isstaked = true;
//     });
//   }
//   // else if(memo == "increment"){
//   //   _staker_list.modify(itr, from, [&](auto& row){
//   //     row.fund_staked += quantity;
//   //     row.nftid_staked.push_back(id);
//   //   });
//   // }
//   check(found, "token is not found or is not owned by account");
//   require_recipient( from );
//   require_recipient( to );
//   // SEND_INLINE_ACTION( *this, transferNFT, {from, "active"_n}, {from, to, id, memo} );
//   transferNFT(from, to, id, memo);
// }
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

	account_index to_accounts( _self, owner.value  _self, owner.value );
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
void blockbunnies::in_contract_transfer(name recipient, vector<id_type> assets_id, string msg){
  action(
      permission_level{_self , "active"_n},
      "atomicassets"_n,              
      "transfer"_n,
      std::make_tuple(get_self(), recipient, assets_id, msg)
    ).send();
}
// void blockbunnies::transferNFT( name	from, name 	to, id_type	id, string	memo ) {
//         // Ensure authorized to send from account
//   check( from != to, "cannot transfer to self" );
//   require_auth( from );

//   // Ensure 'to' account exists
//   check( is_account( to ), "to account does not exist");

//   // Check memo size and print
//   check( memo.size() <= 256, "memo has more than 256 bytes" );

//   // Ensure token ID exists
//   auto send_token = tokens.find( id );
//   check( send_token != tokens.end(), "token with specified ID does not exist" );

//   // Ensure owner owns token
//   check( send_token->owner == from, "sender does not own token with specified ID");

//   const auto& st = *send_token;

//   // Notify both recipients
//   require_recipient( from );
//   require_recipient( to );

//   // Transfer NFT from sender to receiver
//   tokens.modify( send_token, from, [&]( auto& token ) {
//     token.owner = to;
//   });

//   // Change balance of both accounts
//   sub_balance( from, st.value );
//   add_balance( to, st.value, from );
// }
asset blockbunnies::getReward(name username, string memo, string selectLand) {
  require_auth(get_self());
  asset reward;
  auto itr = _staker_list.find(username.value);
  check(itr == _staker_list.end(), "You don't register in stakerlist");
  if(memo == "mining") {
    reward.amount = getPower(itr->nftid_staked, itr->toolnftid_staked, false, memo, selectLand);
    reward.symbol =  blockbunnies_symb;
  }
  else if(memo == "farming") {
    reward.amount = getPower(itr->nftid_staked, itr->toolnftid_staked, false, memo, selectLand);
    reward.symbol =  blockcarrots_symb;  }
  _staker_list.modify( itr, _self, [&]( auto& a ) {

    a.collect_amount.amount += reward.amount;
    a.collect_amount.symbol = reward.symbol;
  }
  return reward;

}
ACTION blockbunnies::claim(name username, string memo) {
  require_auth(username);
  asset claim_amount;
  static const time_point_sec         current_time         = eosio::current_time_point();
  for(auto& item : _staker_list) {
    if (item.next_run < current_time) {
      _staker_list.modify( item, _self, [&]( auto& a ) {
        a.last_updated = a.next_run;
        a.next_run = a.last_updated + period;
        claim_amount =  getReward(a.username, memo);
        action(
          permission_level{_self , "active"_n},
          "eosio.token"_n,              
          "transfer"_n,
          std::make_tuple(get_self(), username, claim_amount, "Sent reward to staker")
        ).send();
        a.collect_amount.amount = a.collect_amount - claim_amount.amount;
      });
    }
   
  }
  // auto itr = _staker_list.find(username.value);

  // check(itr == _staker_list.end(), "Not staker");
  // _staker_list.modify( itr, _self, [&]( auto& a ) {
  //     action(
  //       permission_level{_self , "active"_n},
  //       "eosio.token"_n,              
  //       "transfer"_n,
  //       std::make_tuple(contractowner, username, a.collect_amount, "Prizepayout bonus")
  //     ).send();
  //     a.collect_amount.amount = 0;
  // });
  

}
EOSIO_DISPATCH(blockbunnies, (regstaker))
