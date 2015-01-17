n = GetN()
F = GetFightData(n-1)

if n<=5 then
	result=GetRandom()%3

else if n%100==0
	result=GetRandom()%3
	
else if F==-1 & n%11==6
	m, t = GetRecordData(n-1)
	result = (t+1) %3

else if F==-1 & n%11==4
	m, t = GetRecordData(n-1)
	result = (m+1) %3

else if F==-1 & n%11==2
	m, t = GetRecordData(n-1)
	result = (t+m) %3
	
else if F==-1
	m, t = GetRecordData(n-1)
	result = (t+1) %3

	
	
else if F==0 
	m, t = GetRecordData(n-1)
	m2, t2 = GetRecordData(n-1)
	result = (t *t2) %3

else if F==1 & n%7==2
	fr = GetFightData(n-2)
	fr2 = GetFightData (n-3)
	m, t = GetRecordData(n-1)
	result =(m+fr+fr2) %3
	
else if F==1 & n%7==4
	fr = GetFightData(n-2)
	fr2 = GetFightData (n-3)
	m, t = GetRecordData(n-1)
	result =(t+fr+fr2) %3

else if F==1
	m, t = GetRecordData(n-1)
	result = (t+1) %3



	