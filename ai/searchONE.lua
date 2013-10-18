n = GetN()

if n == 0 then
	result = GetRandom() % 3
elseif n <= 400 then
	m, k = GetRecordData(n - 1)
	win = GetFightData(n - 1)
	if n < 999 then
	SaveData(n, k)
		end
	if win == -1 then
		if k == m then
		result = (k + 2) % 3
		elseif k == (m + 1) % 3 then
		result = k
		elseif k == (m + 2) % 3 then
		result = (k + 1) % 3
			end
	elseif win == 0 then
		result = m
	elseif win == 1 then
		result = k
			end
elseif n > 400 then
	m , k = GetRecordData(n - 1)
	win = GetFightData(n - 1)
	n0 = 0
	n1 = 0
	n2 = 0
	for i = 1, n - 1 do 
		if n < 999 then
		e = GetData(i)
				end
		if e == 0 then
		n0 = n0 + 1
		elseif e == 1 then
		n1 = n1 + 1
		elseif e == 2 then
		n2 = n2 + 1
			end
		end
	if n0 >= (n * 2) / 3 and win ~= 1 then
		result = 1
	elseif n1 >= (n * 2) / 3 and win ~= 1 then
		result = 2
	elseif n2 >= (n * 2) / 3 and win ~= 1 then
		result = 0
	else
		if n < 999 then
		SaveData(n, k)
			end
	if win == -1 then
		if k == m then
		result = (k + 2) % 3
		elseif k == (m + 1) % 3 then
		result = k
		elseif k == (m + 2) % 3 then
		result = (k + 1) % 3
			end
	elseif win == 0 then
		result = m
	elseif win == 1 then
		result = k
			end
		end
end
		
			
	