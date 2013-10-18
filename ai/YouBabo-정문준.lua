n = GetN()

if n<=1 then
	result=GetRandom()%3

elseif n==2 then
	result=GetRandom()%3
	a=GetRandom()%6+1
	b=GetRandom()%2+1
	c=GetRandom()%3+3
	dddd=GetFightData(1)

	for i=1,30 do
		SaveData(i,(result+(dddd+2)*(a+b*i))*b+i*c+(dddd+3)*i)
	end


elseif n%30==0 then

	tempn=n
	nn=0

	while tempn>30 do
		nn=nn+1
		tempn=tempn-30
	end
	
	result=GetData(n)%3

	data1=0
	for i=1, 29 do
		data1=data1+GetFightData(n-i)
	end
	

	if data1==0 then

		for i=1,30 do
			b=GetRandom()%2+1
			tempdata=GetData(i)
			SaveData(n+i,tempdata+b)
		end
	elseif data1<0 then
		for i=1,30 do
			b=GetRandom()%2+1
			tempdata=GetData(i)
			SaveData(n+i,tempdata*(b)*(result+1)+2)
		end
	else
		for i=1,30 do
			tempdata=GetData(i)
			SaveData(n+i,tempdata)
		end
		
	end

elseif n>1000 then
	result=GetData(n%100)%3

else
	result=GetData(n)%3
end