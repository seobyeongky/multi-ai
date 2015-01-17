n = GetN()


if n==0 then
	SaveData(0, GetRandom()%(GetRandom()/100+1))
	SaveData(1, GetRandom()%(GetRandom()%30+1))
	SaveData(2, GetRandom()%(GetRandom()%30+1))
	for i=3,300 do
		c=GetData(i-3)
		d=GetData(i-2)
		e=GetData(i-1)
		f=(c+d+e)%GetData(0)
		SaveData(i, f)
	end
	result=GetData(0)
elseif n<3 then
	result=GetData(n%300)%3
else
	mf, yf=GetRecordData(n-3)
	ms, ys=GetRecordData(n-2)
	mt, yt=GetRecordData(n-1)
	if yf==ys then
		if ys==yt then
			result=(ys+1)%3
		else
			result=GetData(n%300)%3
		end
	elseif ys==mf then
		if yt==ms then
			result=(mt+1)%3
		else
			result=GetData(n%300)%3
		end
	else
		result=GetData(n%300)%3
	end
end