스크립트가 남긴 로그와 도중에 발생한 에러는 gamelog.txt에 저장됨

print(args...)
로그용 함수

GetN()
지금까지 한 판수

GetRecordData(i)
i번째 경기 결과이고 내가 낸 거, 상대가 낸 거를 반환한다.
참고로 0(가위) 1(바위) 2(보)

GetFightData(i)
i번쨰 경기 결과이고 -1, 0또는 1을반환한다.
참고로 -1은 패배 0은 무승부 1은 승리다.

GetRandom()
rand()값을 반환한다. 0부터 MAX_RAND값까지의 랜덤값을 반환한다.
그냥 math.random()쓰면 안좋은 점이 상대가 math.randomseed()로 초기화해버리면 패턴파악당함.

SaveData(i, x)
int값의 데이터를 저장해준다. i번째 배열에 x값을 저장해준다.

GetData(i)
저장해 둔 i번째 데이터를 가져온다.