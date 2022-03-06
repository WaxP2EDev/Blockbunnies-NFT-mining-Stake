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