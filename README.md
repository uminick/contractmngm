# 계약정보 관리

#### 계약정보 구조
 > 계정(키값) \
  계약자A \
  계약자B \
  계약명 \
  계약서 스캔본 해시값 \
  계약서 스캔본 주소
  
  
  

#### 계약정보 입력
ex)
```
cleos push action contractmngm upsert '["devuminick13","ymk","jtk","partership","C57B26863DAC212A5E3B6312B89FFCF36583CF1D","drive.google.com"]' -p devuminick13
```

#### 전체 계약 테이블 조회
ex)
```
cleos get table contractmngm contractmngm contracts
```
