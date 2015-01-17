num = GetN()
rock = 0
scissors = 0
paper = 0
data = 0
me = 0
you = 0
if num <= 2 then
	result = GetRandom()%3
else
	me, you = GetRecordData(num-1)
	SaveData(num, you)
	for i=0, num do
		data = GetData(i)
		if GetFightData(num-1) == 0 then
		else
			if data == 0 then
				scissors = scissors + 1
			elseif data == 1 then
				rock = rock + 1
			else
				paper = paper + 1
			end
		end
	end
	if scissors < rock then 
		if scissors < paper then
			result = 1
		else
			result = 0
		end
	elseif scissors == rock then
		if rock < paper then
			result = 1
		elseif rock == paper then
			result = GetRandom()%3
		else
			result = 0
		end
	else
		if rock <= paper then
			result = 2
		else 
			reslut = 0
		end
	end
end