#pragma once

struct [[eosio::table, eosio::contract(CONTRACT_NAME)]] total_stake {
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
const symbol blockbunnies_symb;
