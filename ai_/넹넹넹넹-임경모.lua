n = GetN()

if n == 0 then
	result = 1
elseif n == 1 then
	m, k = GetRecordData(0)
	result = k 
elseif n == 2 then
	result = GetRandom() % 3
else
	PreResult = GetFightData(n - 1)
end

if PreResult == -1 then --내가 전판에 짐

	if n >= 3 then
		ppResult = GetFightData(n - 2)
		pppResult = GetFightData(n - 3)
	
		if ppResult == -1 and pppResult == -1 then
			--result = GetRandom() % 3
			m, k = GetRecordData(n - 1)
			result = k
	
		elseif ppResult == -1 then --두판 연속으로 졌다.
		
			m, k = GetRecordData(n - 1)
			result = (m + 2) % 3
		else
			m, k = GetRecordData(n - 1)
			result = m --result
		end
		
	elseif n == 2 then
		
		ppResult = GetFightData(n - 2)
		
		if ppResult == -1 then --두판 연속으로 졌다.
		
			m, k = GetRecordData(n - 1)
			result = (m + 2) % 3
		else
			m, k = GetRecordData(n - 1)
			result = m --result
		end
	
	else
		m, k = GetRecordData(n - 1)
		result = m --result
		
	end


elseif PreResult == 0 then

	if n >= 3 then
		ppResult = GetFightData(n - 2)
		pppResult = GetFightData(n - 3)
	
		if ppResult == -1 and pppResult == -1 then
			--result = GetRandom() % 3
			m, k = GetRecordData(n - 1)
			result = k
	
		elseif ppResult == -1 then --두판 연속으로 졌다.
		
			m, k = GetRecordData(n - 1)
			result = (m + 2) % 3
		else
			m, k = GetRecordData(n - 1)
			result = m --result
		end
		
	elseif n == 2 then
		
		ppResult = GetFightData(n - 2)
		
		if ppResult == -1 then --두판 연속으로 졌다.
		
			m, k = GetRecordData(n - 1)
			result = (m + 2) % 3
		else
			m, k = GetRecordData(n - 1)
			result = m --result
		end
	
	else
		m, k = GetRecordData(n - 1)
		result = m --result
		
	end


else -- 내가 이김!
	m, k = GetRecordData(n - 1)
	
	if k == 0 then --상대가 가위로 졌다면
		result = 2
	
	
	elseif k == 1 then
		result = 0
	
	
	else
		result = 1
	end
	
end