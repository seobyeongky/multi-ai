Num = GetN()

if Num == 0 then
	result = GetRandom() % 3
else
	Me, You = GetRecordData(Num-1)
	
	Before = GetFightData(Num - 1)
	
	if Before == 0 then
		result = (Me + 1) % 3
	elseif Before == -1 then
		result = (Me + 1) % 3
	elseif Before == 1 then
		result = Me
	end
	
	if Num == 100 then
		result = 0
	elseif Num == 200 then
		result = 1
	elseif Num == 300 then
		result = 2
	elseif Num == 400 then
		result = 0 
	elseif Num == 500 then
		result = 1
	elseif Num == 600 then
		result = 2
	elseif Num == 700 then
		result = 0
	elseif Num == 800 then
		result = 1
	elseif Num == 900 then
		result = 2
	end
end