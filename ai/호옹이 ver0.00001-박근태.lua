n = GetN()
lcount = 0
ecount = 0
m = 0
e = 0
fr = 0

if n <= 0 then
	result = 1
else
	fr = GetFightData(n-1)
	m, e = GetRecordData(n-1)
	if fr == 1 then
		SaveData(n , m)
		result = m
	elseif fr == 0 then
		if ecount <= 1 then
			ecount = ecount + 1
			SaveData(n, (e+1)%3)
			result = m
		elseif ecount == 2 then
			ecount = 0
			SaveData(n, (e+1)%3)
			result = (e+1)%3
		end
	elseif fr == -1 then
		if lcount <= 1 then
			lcount = lcount + 1
			SaveData(n , (e+1)%3)
			result = (e+1)%3
		elseif lcount == 2 then
			lcount = 0
			SaveData(n , (e+1)%3)
			result = GetData(n-3)
		end
	end
end