#include <eosiolib/eosio.hpp>

using namespace eosio;

class [[eosio::contract]] contractmngm : public eosio::contract {

 public:
  using contract::contract;
  
  contractmngm(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}

  [[eosio::action]]
  void upsert(name user, uint64_t idx,std::string contractor1, std::string contractor2, std::string title, std::string hashv, std::string oglink) {
    require_auth( user );
    contract_index contracts(_code, _code.value);
    auto iterator = contracts.find(user.value);
    if( iterator == contracts.end() )
      {
	contracts.emplace(user, [&]( auto& row ) {
	    row.key = user;
	    row.idx = idx;
	    row.contractor1 = contractor1;
	    row.contractor2 = contractor2;
	    row.title = title;
	    row.hashv = hashv;
	    row.oglink = oglink;
	    
	    send_summary(user, "기록되었습니다.");
	    increment_counter(user, "등록");
	  });
      }
    else {
      std::string changes;
      contracts.modify(iterator, user, [&]( auto& row ) {

	  if(row.idx != idx) {
	    row.idx = idx;
	    changes += "idx ";
	  }
	  if(row.contractor1 != contractor1) {
	    row.contractor1 = contractor1;
	    changes += "contractor1 ";
	  }
	  if(row.title != title) {
	    row.title = title;
	    changes += "title ";
	  }
	  if(row.hashv != hashv) {
	    row.hashv = hashv;
	    changes += "hashv ";
	  }
	  if(row.oglink != oglink) {
	    row.oglink = oglink;
	    changes += "oglink ";
	  }
	});

      if(changes.length() > 0) {
          send_summary(user, "수정되었습니다. 변경된 필드는 " + changes +"입니다.");
	  increment_counter(user, "수정");
	} else {
	  send_summary(user, "수정되었지만 변경된 필드가 없습니다.");
	}
    }
  }

  [[eosio::action]]
    void erase(name user) {
    require_auth(user);

    contract_index contracts(_self, _code.value);

    auto iterator = contracts.find(user.value);
    eosio_assert(iterator != contracts.end(), "기록이 존재하지 않습니다.");
    contracts.erase(iterator);
    send_summary(user, "삭제되었습니다.");
    increment_counter(user, "삭제");
  }

  [[eosio::action]]
  void notify(name user, std::string message) {
    require_auth(get_self());
    require_recipient(user);
  }

 private:
  void send_summary(name user, std::string message){
    action(
      permission_level{get_self(),"active"_n},
      get_self(),
      "notify"_n,
      std::make_tuple(user, name{user}.to_string() + message)
    ).send();
  };

  void increment_counter(name user, std::string type) {
    action counter = action(
      permission_level{get_self(),"active"_n},
      "abcounter123"_n,
      "count"_n,
      std::make_tuple(user, type)
    );

    counter.send();
  }

  struct [[eosio::table]] contractinfo {
    name key;
    uint64_t idx;
    std::string contractor1;
    std::string contractor2;
    std::string title;
    std::string hashv;
    std::string oglink;
    uint64_t primary_key() const { return key.value; }
    uint64_t get_secondary_1() const {return idx;}
  };
  typedef eosio::multi_index<"contracts"_n, contractinfo, indexed_by<"byidx"_n, const_mem_fun<contractinfo, uint64_t, &contractinfo::get_secondary_1>> > contract_index;

};

EOSIO_DISPATCH( contractmngm, (upsert)(erase)(notify))
