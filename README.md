# 계약정보 관리

#### 계약정보 구조
 > 계정(키값) \
  계약자A \
  계약자B \
  계약명 \
  계약서 스캔본 해시값 \
  계약서 스캔본 주소
 
contractmngm 계정의 contractmngm 컨트랙트에 push 될 때마다 인라인 액션 출력되고 \
```cleos get actions <계정명>``` 을 통해 액션 확인 가능
abcounter123 계정의 abcounter 컨트랙트는 contractmngm에 의해서만 실행되고 테이블에 데이터가 등록 , 수정, 삭제 될때마다 카운터 저장 \
```cleos get table abcounter123 abcounter123 counts``` 을 통해 등록, 수정, 삭제 횟수 확인 
  
  
#### 체인에 컨트랙트 등록
##### 컴파일
```
#contractmngm 폴더안에서
eosio-cpp -o contractmngm.wasm contractmngm.cpp --abigen
#abcounter 폴더안에서
eosio-cpp -o abcounter.wasm abcounter.cpp --abigen
```
##### 컨트랙트 셋
ex)
```
cleos set countract contractmngm /...저장경로/contractmngm 
cleos set countract abcounter123 /...저장경로/abcounter123
```

#### 계약정보 입력
ex)
```
cleos push action contractmngm upsert '["devuminick13","ymk","jtk","partership","C57B26863DAC212A5E3B6312B89FFCF36583CF1D","drive.google.com"]' -p devuminick13
```

#### 계약정보 삭제
ex)
```
cleos push action contractmngm erase '["devuminick13"]' -p devuminick13
```
cleos에 연결된 지갑에 키가 있는 상태에서 테이블에 저장된 키 계정과 실행하는 계정이 같아야지만 삭제 가능

#### 전체 계약 테이블 조회
ex)
```
cleos get table contractmngm contractmngm contracts
```
