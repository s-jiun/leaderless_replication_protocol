# leaderless_replication_protocol

leaderless replication에서는 분산 스토리지에 저장된 값이 서로 상이한 경우가 발생할 수 있습니다. 이러한 경우 client가 올바른 값을 반환할 수 있도록 점검해주는 작업이 필요합니다.
해당 프로그램은 client에서 get 요청을 보낼 시 3대의 server에서 1~3의 숫자를 랜덤으로 client에 보내주고, client는 동일한 값이 2개 이상 넘어온 경우 TRUE를, 전부 다른 값이 넘어온 경우 FALSE를 반환합니다.

![client](https://github.com/s-jiun/leaderless_replication_protocol/assets/84860387/34112a20-a0e9-487a-b70f-6822975d6b69)
![server1](https://github.com/s-jiun/leaderless_replication_protocol/assets/84860387/fd1c90db-d4e9-4179-a778-f5531ae3b0a0)
![server2](https://github.com/s-jiun/leaderless_replication_protocol/assets/84860387/b16ca305-6923-443a-8ac7-664a8d135aa8)
![server3](https://github.com/s-jiun/leaderless_replication_protocol/assets/84860387/f09c228c-1463-4479-a0e4-a5c6af4fe5cb)
