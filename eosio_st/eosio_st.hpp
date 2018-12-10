
#pragma once

#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>

#include <string>

using namespace eosio;
using namespace std;

class eosio_st : public contract {
public:
	eosio_st(account_name self) : contract(self) {}

	//@abi action
	void create(account_name issuer,
				asset maximum_supply);

	//@abi action
	void issue(account_name to, asset quantity, string memo);

	//@abi action
	void transfer(account_name from,
				  account_name to,
				  asset quantity,
				  string memo);

	//@abi action
	void blacklist(account_name name);

	//@abi action
	void dtxrestrict(account_name from, account_name to, uint64_t value);

	//@abi action
	void mtxrestrict(uint8_t restrictionCode);

	inline asset get_supply(symbol_name sym) const;

	inline asset get_balance(account_name owner, symbol_name sym) const;

private:
	struct account {
		asset balance;

		uint64_t primary_key() const { return balance.symbol.name(); }

		EOSLIB_SERIALIZE(account, (balance))
	};

	struct currency_stats {
		asset supply;
		asset max_supply;
		account_name issuer;

		uint64_t primary_key() const { return supply.symbol.name(); }

		EOSLIB_SERIALIZE(currency_stats, (supply)(max_supply)(issuer))
	};

	struct black_list {
		account_name name;

		account_name primary_key() const { return name; }
		EOSLIB_SERIALIZE(black_list, (name))
	};

	typedef eosio::multi_index<N(accounts), account> accounts;
	typedef eosio::multi_index<N(stat), currency_stats> stats;
	typedef eosio::multi_index<N(blacklist), black_list> tb_black;

	void sub_balance(account_name owner, asset value);

	void add_balance(account_name owner, asset value, account_name ram_payer);

public:
	struct transfer_args {
		account_name from;
		account_name to;
		asset quantity;
		string memo;
	};
};

asset eosio_st::get_supply(symbol_name sym) const {
	stats statstable(_self, sym);
	const auto &st = statstable.get(sym);
	return st.supply;
}

asset eosio_st::get_balance(account_name owner, symbol_name sym) const {
	accounts accountstable(_self, owner);
	const auto &ac = accountstable.get(sym);
	return ac.balance;
}

