#include <eosiolib/eosio.hpp>

using namespace eosio;

class [[eosio::contract]] contractmngm : public eosio::contract {

 public:
  using contract::contract;
  
  contractmngm(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}

  [[eosio::action]]
    void upsert(name user, std::string contractor1, std::string contractor2, std::string title, std::string hashv, std::string oglink) {
    require_auth( user );
    contract_index contracts(_code, _code.value);
    auto iterator = contracts.find(user.value);
    if( iterator == contracts.end() )
      {
	contracts.emplace(user, [&]( auto& row ) {
	    row.key = user;
	    row.contractor1 = contractor1;
	    row.contractor2 = contractor2;
	    row.title = title;
	    row.hashv = hashv;
	    row.oglink = oglink;
	  });
      }
    else {
      std::string changes;
      contracts.modify(iterator, user, [&]( auto& row ) {
	  row.key = user;
	  row.contractor1 = contractor1;
	  row.contractor2 = contractor2;
	  row.title = title;
	  row.hashv = hashv;
	  row.oglink = oglink;
	});
    }
  }

  [[eosio::action]]
    void erase(name user) {
    require_auth(user);

    contract_index contracts(_self, _code.value);

    auto iterator = contracts.find(user.value);
    eosio_assert(iterator != contracts.end(), "기록이 존재하지 않습니다.");
    contracts.erase(iterator);
  }

 private:
  struct [[eosio::table]] contractinfo {
    name key;
    std::string contractor1;
    std::string contractor2;
    std::string title;
    std::string hashv;
    std::string oglink;
    uint64_t primary_key() const { return key.value; }
  };
  typedef eosio::multi_index<"contracts"_n, contractinfo> contract_index;

};

EOSIO_DISPATCH( contractmngm, (upsert)(erase))
