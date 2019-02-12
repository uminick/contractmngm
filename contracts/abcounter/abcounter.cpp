#include <eosiolib/eosio.hpp>

using namespace eosio;

class [[eosio::contract]] abcounter : public eosio::contract {
 public:
  using contract::contract;

  abcounter(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}

  [[eosio::action]]
    void count(name user, std::string type) {
    require_auth( name("contractmngm"));
    count_index counts(name(_code), _code.value);
    auto iterator = counts.find(user.value);
      
    if (iterator == counts.end()) {
      counts.emplace("contractmngm"_n, [&]( auto& row ) {
          row.key = user;
          row.emplaced = (type == "등록") ? 1 : 0;
          row.modified = (type == "수정") ? 1 : 0;
          row.erased = (type == "삭제") ? 1 : 0;
        });
    }
    else {
      counts.modify(iterator, "contractmngm"_n, [&]( auto& row ) {
          if(type == "등록") { row.emplaced += 1; }
          if(type == "수정") { row.modified += 1; }
          if(type == "삭제") { row.erased += 1; }
        });
    }
  }

 private:
  struct [[eosio::table]] counter {
    name key;
    uint64_t emplaced;
    uint64_t modified;
    uint64_t erased;
    uint64_t primary_key() const { return key.value; }
  };

  using count_index = eosio::multi_index<"counts"_n, counter>;
};

EOSIO_DISPATCH( abcounter, (count));
